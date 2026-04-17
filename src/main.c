#include "rust_bridge.h"
#include <stdio.h>

int main(void) {
  const char *target_url = "http://cracked-dev.com/engine.html";

  printf("C HOST: Sending URL to Rust worker...\n");

  ParsedUrl url = parse_url(target_url);

  printf("C HOST: Received parsed data!\n");
  printf("  -> Host: %s\n", url.host);
  printf("  -> Path: %s\n", url.path);

  printf("C HOST: Returning memory to Rust for cleanup...\n");
  free_parsed_url(url);

  printf("C HOST: Success. Exiting.\n");
  return 0;
}
