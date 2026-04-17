#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

char *fetch_html(const char *hostname, const char *path) {
  struct hostent *server = gethostbyname(hostname);
  if (server == NULL) {
    fprintf(stderr, "Error: Could not resolve hostname %s\n", hostname);
    return NULL;
  }

  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    fprintf(stderr, "Error: Could not create socket\n");
    return NULL;
  }

  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(80);
  memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

  if (connect(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    fprintf(stderr, "Error: Could not connect to server\n");
    close(fd);
    return NULL;
  }

  char request[512];
  snprintf(request, sizeof(request),
           "GET %s HTTP/1.1\r\n"
           "Host: %s\r\n"
           "Connection: close\r\n\r\n",
           path, hostname);

  write(fd, request, strlen(request));

  /* TODO: Make response allocation dynamic */
  int buffer_size = 8192;
  char *response = malloc(buffer_size);
  memset(response, 0, buffer_size);

  read(fd, response, buffer_size - 1);

  close(fd);
  return response;
}
