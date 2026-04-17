#include "fetch.h"
#include "rust_bridge.h"
#include <stdio.h>

int main() {
  const char *target_url = "http://cracked-dev.com/engine.html";

  printf("C HOST: Sending URL to Rust worker...\n");

  ParsedUrl url = parse_url(target_url);

  char *html = fetch_html(url.host, url.path);
  if (html == NULL) {
    fprintf(stderr, "C HOST: Failed to fetch HTML content.\n");
    free_parsed_url(url);
    return 1;
  }

  printf("Received:\n %s\n", html);

  free_parsed_url(url);
  return 0;
}
