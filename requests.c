#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

/* Compute a GET request, given the `host`, `url`, `cookie` and the `token` */
char *compute_get_request(char *host, char *url, char *cookie, char *token)
{
    char *message = calloc(BUFLEN,  sizeof(char));
    DIE(message == NULL, "[ERROR]: Allocation error!\n");
    char *line = calloc(LINELEN, sizeof(char));
    DIE(line    == NULL, "[ERROR]: Allocation error!\n");

    // Write the method name, URL and protocol type
    sprintf(line, "GET %s HTTP/1.1", url);
    compute_message(message, line);

    // Add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // Add cookie if exists
    if (cookie != NULL)
    {
        sprintf(line, "Cookie: %s", cookie);
        compute_message(message, line);
    }

    // Add token if exists
    if (token != NULL)
    {
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line);
    }

    // Add final new line
    compute_message(message, "");

    free(line);
    return message;
}

/*
    Compute a POST or DELETE request, given the `host`, `url`,
    the actual payload `json_data`, a `cookie` and the `token`
*/
char *compute_post_or_delete_request(bool post_req, char *host, char *url,
                                    char *json_data, char *cookie, char *token)
{
    char *message = calloc(BUFLEN,  sizeof(char));
    DIE(message == NULL, "[ERROR]: Allocation error!\n");
    
    char *line = calloc(LINELEN, sizeof(char));
    DIE(line    == NULL, "[ERROR]: Allocation error!\n");

    // Write the method name, URL and protocol type
    if (post_req)
        sprintf(line, "POST %s HTTP/1.1", url);
    else
        sprintf(line, "DELETE %s HTTP/1.1", url);
    compute_message(message, line);
    
    // Add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    if (json_data != NULL)
    {
        // Add Content-Type
        sprintf(line, "Content-Type: application/json");
        compute_message(message, line);

        // Add Content-Length
        sprintf(line, "Content-Length: %zu", strlen(json_data));
        compute_message(message, line);
    }

    // Add cookie (if exists)
    if (cookie != NULL)
    {
        // memset(line, 0, LINELEN);
        sprintf(line, "Cookie: %s", cookie);
        compute_message(message, line);
    }

    // Add token (if exists)
    if (token != NULL)
    {
        // memset(line, 0, LINELEN);
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line);
    }

    // Add new line at end of header
    compute_message(message, "");

    // Add the actual payload JSON data (if exists)
    if (json_data != NULL)
        compute_message(message, json_data);

    free(line);
    return message;
}
