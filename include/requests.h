#ifndef _REQUESTS_
#define _REQUESTS_

#include <stdbool.h>

// Computes and returns a GET request string (`cookie` and `token` can be set to NULL if not needed)
char *compute_get_request(char *host, char *url, char *cookie, char *token);

// Computes and returns a POST or DELETE request string (`cookie` and `token` can be set to NULL if not needed)
char *compute_post_or_delete_request(bool post_req, char *host, char *url, char *json_data, char *cookie, char *token);


// // Computes and returns a POST request string (`cookie` and `token` can be set to NULL if not needed)
// char *compute_post_request(char *host, char *url, char *json_data, char *cookie, char *token);
// // Computes and returns a DELETE request string (`cookie` and `token` can be set to NULL if not needed)
// char *compute_delete_request(char *host, char *url, char *json_data, char *cookie, char *token);

#endif
