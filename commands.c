#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "helpers.h"
#include "commands.h"
#include "requests.h"
#include "parson.h"

/* External general usage buffers */
extern char *command;

extern char *message;
extern char *response;

extern char *cookie;
extern char *token;


/* Helper function which prints all the valid commands */
void usage()
{
    printf("The following are valid commands:\n");
    printf("`register`\n");
    printf("`login`\n");
    printf("`logout`\n");
    printf("`enter_library`\n");
    printf("`get_books`\n");
    printf("`get_book`\n");
    printf("`add_book`\n");
    printf("`delete_book`\n");
    printf("`exit`\n");
}


/**
 * This function is used for registering a client to the server by
 * parsing the input `username` and `password` and computing a POST
 * request to the server. After that, it parses the repsonse and prints the result.
*/
void register_client()
{
    // Parse the input and compute the payload message which will be send to the server
    char *input = (char *) calloc(MAX_COMMAND_LEN, sizeof(char));
    DIE(input == NULL, "[ERROR]: Allocation error!\n");

    JSON_Value  *root_value  = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    printf("username=");
    scanf("%s", input);
    json_object_set_string(root_object, "username", input);

    memset(input, 0, strlen(input));
    printf("password=");
    scanf("%s", input);
    json_object_set_string(root_object, "password", input);

    char *serialized_string = json_serialize_to_string_pretty(root_value);

    // Compute the POST request and send the message to the server
    message     = compute_post_or_delete_request(true, HOST, "/api/v1/tema/auth/register", serialized_string, NULL, NULL);
    int sockfd  = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    response    = receive_from_server(sockfd);
    close_connection(sockfd);

    // Parse the response
    if (strstr(response, "Created"))
        printf("[SUCCESS]: Welcome, `%s`! You have successfully registered!\n", json_object_get_string(root_object, "username"));
    else
    {
        if (strstr(response, "many"))
            printf("[ERROR]: Too many requests, please try again later!\n");
        else
        {
            JSON_Value  *value  = json_parse_string(basic_extract_json_response(response));
            JSON_Object *object = json_value_get_object(value);
            printf("[ERROR]: %s\n",    json_object_get_string(object, "error"));
            json_value_free(value);
        }
    }

    // Free the memory
    json_value_free(root_value);
    free(serialized_string);
    free(input);
    free(message);
    free(response);
}


/**
 * This function is used for logging in a client to the server by
 * parsing the input `username` and `password` and computing a POST
 * request to the server. After that, it parses the repsonse,
 * saves a `cookie` which tells us if the user is logged in and then prints the result.
*/
void login_client()
{
    // Check if the user is already logged in
    if (strcmp(cookie, "NULL"))
    {
        printf("You are already logged in! If you want to switch the account, you must log out first!\n");
        return;
    }

    // Parse the input and compute the payload message which will be send to the server
    char *input = (char *) calloc(MAX_COMMAND_LEN, sizeof(char));
    DIE(input == NULL, "[ERROR]: Allocation error!\n");

    JSON_Value  *root_value  = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    printf("username=");
    scanf("%s", input);
    json_object_set_string(root_object, "username", input);

    memset(input, 0, strlen(input));
    printf("password=");
    scanf("%s", input);
    json_object_set_string(root_object, "password", input);

    char *serialized_string = json_serialize_to_string_pretty(root_value);

    // Compute the POST request and send the message to the server
    message     = compute_post_or_delete_request(true, HOST, "/api/v1/tema/auth/login", serialized_string, NULL, NULL);
    int sockfd  = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    response    = receive_from_server(sockfd);
    close_connection(sockfd);

    // Parse the response
    bool ok_status = true; 
    if (strstr(response, "OK"))
        printf("[SUCCESS]: Welcome, `%s`! You have logged in successfully!\n", json_object_get_string(root_object, "username"));
    else
    {
        ok_status = false;
       
        if (strstr(response, "many"))
            printf("[ERROR]: Too many requests, please try again later!\n");
        else
        {
            JSON_Value  *value  = json_parse_string(basic_extract_json_response(response));
            JSON_Object *object = json_value_get_object(value);
            printf("[ERROR]: %s\n", json_object_get_string(object, "error"));
            json_value_free(value);
        }
    }

    // Save cookie
    // Set-Cookie: connect.sid=s%3Abd_24iLJItPjLxsshgiNufamKzluGG70.%2F%2BGl1indwZ77luLAyd%2BcSeViEg5ipoi4wkq9tf0ktck;
    if (ok_status)
    {
        strcpy(cookie, basic_extract_cookie_response(response));
        cookie[strlen(cookie) - 1] = '\0';
    }

    // Free the memory
    json_value_free(root_value);
    free(serialized_string);
    free(input);
    free(message);
    free(response);
}


/**
 * This function is used for loggin out a client from the server by
 * computing a GET request to the server. After that, it parses the repsonse and prints the result.
*/
void logout_client()
{
    // Compute the get request and send the message to the server
    message     = compute_get_request(HOST, "/api/v1/tema/auth/logout", cookie, NULL);
    int sockfd  = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    response    = receive_from_server(sockfd);
    close_connection(sockfd);

    // Parse the response
    if (strstr(response, "OK"))
        printf("[SUCCESS]: You have logged out successfully!\n");
    else
    {
        if (strstr(response, "many"))
            printf("[ERROR]: Too many requests, please try again later!\n");
        else
        {
            JSON_Value  *value  = json_parse_string(basic_extract_json_response(response));
            JSON_Object *object = json_value_get_object(value);
            printf("[ERROR]: %s\n", json_object_get_string(object, "error"));
            json_value_free(value);
        }
    }

    // Remove cookie and token
    strcpy(cookie, "NULL");
    strcpy(token,  "NULL");

    // Free the memory
    free(message);
    free(response);
}


/**
 * This function is used for entering in the library by computing a GET request to the server.
 * It also saves the received `token` from the server (this will tell us access in the library).
*/
void enter_library()
{
    // Compute the GET request and send the message to the server
    message     = compute_get_request(HOST, "/api/v1/tema/library/access", cookie, NULL);
    int sockfd  = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    response    = receive_from_server(sockfd);
    close_connection(sockfd);

    // Parse the response
    bool ok_status = true; 
    if (strstr(response, "OK"))
        printf("[SUCCESS]: You are now in the library!\n");
    else
    {
        ok_status = false;
       
        if (strstr(response, "many"))
            printf("[ERROR]: Too many requests, please try again later!\n");
        else
        {
            JSON_Value  *value  = json_parse_string(basic_extract_json_response(response));
            JSON_Object *object = json_value_get_object(value);
            printf("[ERROR]: %s\n", json_object_get_string(object, "error"));
            json_value_free(value);
        }
    }

    // Save token
    // {"token":"..."}
    if (ok_status)
    {
        JSON_Value  *value  = json_parse_string(basic_extract_json_response(response));
        JSON_Object *object = json_value_get_object(value);
        strcpy(token, json_object_get_string(object, "token"));
        json_value_free(value);
    }

    // Free the memory
    free(message);
    free(response);
}


/**
 * This function is used for listing the user's books from the library by computing a GET request to the server.
 * After that, parse the repsonse and print the resulted list of books (`[]` if there are no books to print).
*/
void get_books()
{
    // Compute the GET request and send the message to the server
    message     = compute_get_request(HOST, "/api/v1/tema/library/books", cookie, token);
    int sockfd  = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    response    = receive_from_server(sockfd);
    close_connection(sockfd);

    // Parse the response
    if (strstr(response, "OK"))
        printf("[SUCCESS]: List of books: %s\n", basic_extract_json_books_list(response));
    else
    {
        if (strstr(response, "many"))
            printf("[ERROR]: Too many requests, please try again later!\n");
        else
        {
            JSON_Value  *value  = json_parse_string(basic_extract_json_response(response));
            JSON_Object *object = json_value_get_object(value);
            printf("[ERROR]: %s\n", json_object_get_string(object, "error"));
            json_value_free(value);
        }
    }

    // Free the memory
    free(message);
    free(response);
}


/**
 * This function is used for listing a book from the library (given an `id`) by computing a GET request to the server.
 * After that, parse the repsonse and print the book (or an error message if there is no book with the given `id`).
*/
void get_book()
{
    // Parse the input and compute the payload message which will be send to the server
    char *input = (char *) calloc(MAX_COMMAND_LEN, sizeof(char));
    DIE(input == NULL, "[ERROR]: Allocation error!\n");

    printf("id=");
    scanf("%s", input);
    if (atoi(input) == 0)
    {
        printf("[ERROR]: `id` should be a positive number!\n");
        return;
    }

    // Compute the URL
    char *url = (char *) calloc(MAX_COMMAND_LEN, sizeof(char));
    DIE(url == NULL, "[ERROR]: Allocation error!\n");
    sprintf(url, "/api/v1/tema/library/books/%s", input);

    // Compute the GET request and send the message to the server
    message     = compute_get_request(HOST, url, cookie, token);
    int sockfd  = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    response    = receive_from_server(sockfd);
    close_connection(sockfd);

    // Parse the response
    if (strstr(response, "OK"))
        printf("[SUCCESS]: Book: %s\n", basic_extract_json_books_list(response));
    else
    {
        if (strstr(response, "many"))
            printf("[ERROR]: Too many requests, please try again later!\n");
        else
        {
            JSON_Value  *value  = json_parse_string(basic_extract_json_response(response));
            JSON_Object *object = json_value_get_object(value);
            printf("[ERROR]: %s\n", json_object_get_string(object, "error"));
            json_value_free(value);
        }
    }

    // Free the memory
    free(url);
    free(message);
    free(response);
}


/**
 * This function is used for adding a book to the library, by computing a POST request to the server.
 * After that, parse the repsonse and print the result.
*/
void add_book()
{
    char *input = (char *) calloc(MAX_COMMAND_LEN, sizeof(char));
    DIE(input == NULL, "[ERROR]: Allocation error!\n");

    JSON_Value  *root_value  = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    // Read garbage
    size_t size = strlen(input);
    getline(&input, &size, stdin);
    memset(input, 0, strlen(input));

    // Parse the input and compute the payload message which will be send to the server
    printf("title=");
    getline(&input, &size, stdin);
    input[strlen(input) - 1] = '\0';
    json_object_set_string(root_object, "title", input);

    printf("author=");
    getline(&input, &size, stdin);
    input[strlen(input) - 1] = '\0';
    json_object_set_string(root_object, "author", input);

    printf("genre=");
    getline(&input, &size, stdin);
    input[strlen(input) - 1] = '\0';
    json_object_set_string(root_object, "genre", input);

    printf("publisher=");
    getline(&input, &size, stdin);
    input[strlen(input) - 1] = '\0';
    json_object_set_string(root_object, "publisher", input);

    printf("page_count=");
    getline(&input, &size, stdin);
    input[strlen(input) - 1] = '\0';
    json_object_set_string(root_object, "page_count", input);
    if (atoi(input) == 0)
    {
        printf("[ERROR]: `page_count` should be a positive number!\n");
        return;
    }

    char *serialized_string = json_serialize_to_string_pretty(root_value);

    // Compute the POST request and send the message to the server
    message     = compute_post_or_delete_request(true, HOST, "/api/v1/tema/library/books", serialized_string, cookie, token);
    int sockfd  = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    response    = receive_from_server(sockfd);
    close_connection(sockfd);

    // Parse the response
    if (strstr(response, "OK"))
        printf("[SUCCESS]: Added book: `%s`\n", json_object_get_string(root_object, "title"));
    else
    {
        if (strstr(response, "many"))
            printf("[ERROR]: Too many requests, please try again later!\n");
        else
        {
            JSON_Value  *value  = json_parse_string(basic_extract_json_response(response));
            JSON_Object *object = json_value_get_object(value);
            printf("[ERROR]: %s\n", json_object_get_string(object, "error"));
            json_value_free(value);
        }
    }

    // Free the memory
    free(message);
    free(response);
}


/**
 * This function is used for deleting a book from the library (given an `id`) by computing a DELETE request to the server.
 * After that, parse the repsonse and print the result.
*/
void delete_book()
{
    // Parse the input and compute the payload message which will be send to the server
    char *input = (char *) calloc(MAX_COMMAND_LEN, sizeof(char));
    DIE(input == NULL, "[ERROR]: Allocation error!\n");

    printf("id=");
    scanf("%s", input);
    if (atoi(input) == 0)
    {
        printf("[ERROR]: `id` should be a positive number!\n");
        return;
    }

    // Compute the URL
    char *url = (char *) calloc(MAX_COMMAND_LEN, sizeof(char));
    DIE(url == NULL, "[ERROR]: Allocation error!\n");
    sprintf(url, "/api/v1/tema/library/books/%s", input);

    // Compute the DELETE request and send the message to the server
    message     = compute_post_or_delete_request(false, HOST, url, NULL, cookie, token);
    int sockfd  = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    response    = receive_from_server(sockfd);
    close_connection(sockfd);

    // Parse the response
    if (strstr(response, "OK"))
        printf("[SUCCESS]: Deleted book with id `%d`\n", atoi(input));
    else
    {
        if (strstr(response, "many"))
            printf("[ERROR]: Too many requests, please try again later!\n");
        else
        {
            JSON_Value  *value  = json_parse_string(basic_extract_json_response(response));
            JSON_Object *object = json_value_get_object(value);
            printf("[ERROR]: %s\n", json_object_get_string(object, "error"));
            json_value_free(value);
        }
    }

    // Free the memory
    free(url);
    free(message);
    free(response);
}
