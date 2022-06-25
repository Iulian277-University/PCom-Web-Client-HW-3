#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "commands.h"

/* General usage buffers */
char *command;

char *message;
char *response;

char *cookie;
char *token;


int main(int argc, char *argv[])
{
    // Input user `command`
    command = (char *) calloc(MAX_COMMAND_LEN, sizeof(char));
    DIE(command == NULL, "[ERROR]: Allocation error!\n");

    // `message` which will be `send to the server`
    message = (char *) calloc(BUFLEN, sizeof(char));
    DIE(message == NULL, "[ERROR]: Allocation error!\n");

    // Response `message` which will be `received from the server`
    response = (char *) calloc(BUFLEN, sizeof(char));
    DIE(response == NULL, "[ERROR]: Allocation error!\n");

    // Cookie which tells us if the user is `logged in`
    cookie = (char *) calloc(BUFLEN, sizeof(char));
    DIE(cookie == NULL, "[ERROR]: Allocation error!\n");
    strcpy(cookie, "NULL");

    // Token which tells us if the user is `in the library`
    token = (char *) calloc(BUFLEN, sizeof(char));
    DIE(token == NULL, "[ERROR]: Allocation error!\n");
    strcpy(token, "NULL");

    // Sequentially, read commands from stdin
    while (1)
    {
        // Read the command
        memset(command, 0, strlen(command));
        scanf("%s", command);

        // Sanity check
        if (strcmp(command, EXIT) == 0)
            break;

        if      (!strcmp(command, USAGE))
            usage();
        else if (!strcmp(command, REGISTER))
            register_client();
        else if (!strcmp(command, LOGIN))
            login_client();
        else if (!strcmp(command, LOGOUT))
            logout_client();
        else if (!strcmp(command, ENTER_LIBRARY))
            enter_library();
        else if (!strcmp(command, GET_BOOKS))
            get_books();
        else if (!strcmp(command, GET_BOOK))
            get_book();
        else if (!strcmp(command, ADD_BOOK))
            add_book();
        else if (!strcmp(command, DELETE_BOOK))
            delete_book();
        else
            printf("[UNKNOWN COMMAND]: Type `usage` to list all valid commands!\n");
    }

    // Free the memory
    free(command);
    free(cookie);
    free(token);
    return 0;
}
