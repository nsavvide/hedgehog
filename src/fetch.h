#ifndef FETCH_H
#define FETCH_H

/*
 * Fetches HTML content for the given hostname and path.
 *
 * Returns: a newly allocated, NUL-terminated buffer containing the fetched
 * HTML on success. The caller owns the returned buffer and must release it
 * with free().
 */
char *fetch_html(const char *hostname, const char *path);

#endif // FETCH_H
