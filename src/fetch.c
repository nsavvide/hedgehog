#include "fetch.h"
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

char *fetch_html(const char *hostname, const char *path) {
  struct addrinfo hints;
  struct addrinfo *result;
  struct addrinfo *rp;
  int fd = -1;
  int gai_status;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  gai_status = getaddrinfo(hostname, "80", &hints, &result);
  if (gai_status != 0) {
    fprintf(stderr, "Error: Could not resolve hostname %s: %s\n", hostname,
            gai_strerror(gai_status));
    return NULL;
  }

  for (rp = result; rp != NULL; rp = rp->ai_next) {
    fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (fd < 0) {
      continue;
    }

    if (connect(fd, rp->ai_addr, rp->ai_addrlen) == 0) {
      break;
    }

    close(fd);
    fd = -1;
  }

  freeaddrinfo(result);

  if (fd < 0) {
    fprintf(stderr, "Error: Could not connect to server\n");
    return NULL;
  }

  int request_len = snprintf(NULL, 0,
                             "GET %s HTTP/1.1\r\n"
                             "Host: %s\r\n"
                             "Connection: close\r\n\r\n",
                             path, hostname);
  if (request_len < 0) {
    fprintf(stderr, "Error: Could not format request\n");
    close(fd);
    return NULL;
  }

  char *request = malloc((size_t)request_len + 1);
  if (request == NULL) {
    fprintf(stderr, "Error: Could not allocate request buffer\n");
    close(fd);
    return NULL;
  }

  if (snprintf(request, (size_t)request_len + 1,
               "GET %s HTTP/1.1\r\n"
               "Host: %s\r\n"
               "Connection: close\r\n\r\n",
               path, hostname) != request_len) {
    fprintf(stderr, "Error: Could not format request\n");
    free(request);
    close(fd);
    return NULL;
  }

  // TODO: Could create a partial write
  write(fd, request, (size_t)request_len);
  free(request);
  /* TODO: Make response allocation dynamic */
  int buffer_size = 8192;
  char *response = malloc(buffer_size);
  if (response == NULL) {
    fprintf(stderr, "Error: Could not allocate response buffer\n");
  size_t buffer_size = 8192;
  size_t total_read = 0;
  ssize_t bytes_read;
  char *response = malloc(buffer_size);
  if (response == NULL) {
    fprintf(stderr, "Error: Could not allocate response buffer\n");
    close(fd);
    return NULL;
  }

  while (1) {
    if (total_read == buffer_size - 1) {
      size_t new_buffer_size = buffer_size * 2;
      char *new_response = realloc(response, new_buffer_size);
      if (new_response == NULL) {
        fprintf(stderr, "Error: Could not grow response buffer\n");
        free(response);
        close(fd);
        return NULL;
      }
      response = new_response;
      buffer_size = new_buffer_size;
    }

    bytes_read = read(fd, response + total_read, buffer_size - total_read - 1);
    if (bytes_read <= 0) {
      break;
    }
    total_read += (size_t)bytes_read;
  }

  if (bytes_read < 0) {
    fprintf(stderr, "Error: Could not read response\n");
    free(response);
    close(fd);
    return NULL;
  }

  response[total_read] = '\0';
  close(fd);
  return response;
}
