#ifndef _COMMANDS_
#define _COMMANDS_

#define MAX_COMMAND_LEN 200

#define USAGE           "usage"
#define REGISTER        "register"
#define LOGIN           "login"
#define LOGOUT          "logout"
#define ENTER_LIBRARY   "enter_library"
#define GET_BOOKS       "get_books"
#define GET_BOOK        "get_book"
#define ADD_BOOK        "add_book"
#define DELETE_BOOK     "delete_book"
#define EXIT            "exit"


void usage();
void register_client();
void login_client();
void logout_client();
void enter_library();
void get_books();
void get_book();
void add_book();
void delete_book();

#endif
