#ifndef RUST_BRIDGE_H
#define RUST_BRIDGE_H

typedef struct {
  char *host;
  char *path;
} ParsedUrl;

ParsedUrl parse_url(const char *raw_url);
void free_parsed_url(ParsedUrl url);

#endif // RUST_BRIDGE_H
