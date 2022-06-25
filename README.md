## Name: *Iulian-Marius Tăiatu (322CB)*
---

# General presentation

**In this documentation, I will talk about some general ideas and the workflow of the program. I implemented a HTTP client application which interacts with a REST API.**

**The purpose of the project is to learn the basic ideas about HTTP, such as sending GET/POST requests to the server, receiving and parsing the responses, working with cookies and tokens, and so on.**

# JSON Parser

For parsing the input from `stdin` and the received repsonse from the server, I've used a JSON library called [Parson](https://github.com/kgabis/parson).

The main advantage is the fact that it is a lightweight library written in C (with only 2 files), very portable and it's also compatible with the C89 standard. For bigger projects there are other libraries, but for this small project is enough.

# Workflow

Before talking about each `command`, I will explain the general `workflow` of the program:
- Parse the user input (if needed)
- Compute a GET / POST / DELETE request
- Open the connection with the server
- Send the request
- Receive the response
- Close the connection with the server
- Parse the response
- Free the allocated memory

The purpose of `opening` and `closing` the connection for every command is because the server has something similar to a `TTL` (time to live).
If there are no requests in a given interval of time, then the server will automatically close the connection.


# Requests

## GET
For computing a `GET` request, I had to write the method name, URL and protocol type.

After that, I completed the Host, then the cookie (if exists) and the token (if exists).

## POST / DELETE
For computing a `POST` or `DELETE` request, I had to write the method name, URL and protocol type.

After that, I completed the Host, then I've added the Content-Type and Content-Length (if json payload exists),
then the cookie (if exists) and the token (if exists). At the end, I wrote the actual payload (json_data).


# Commands

## Register client
This function is used for `registering` a client to the server by parsing the input `username` and `password` and computing a `POST` request to the server. After that, it parses the repsonse and prints the result.

## Login client
This function is used for `logging in` a client to the server by parsing the input `username` and `password` and computing a `POST` request to the server. After that, it parses the repsonse, saves a `cookie` which tells us if the user is logged in and then prints the result.

## Logout client
This function is used for `logging out` a client from the server by computing a `GET` request to the server. After that, it parses the repsonse and prints the result.

## Enter library
This function is used for `entering in the library` by computing a `GET` request to the server. It also saves the received `token` from the server (this will tell us if the user has access in the library).

## Get books
This function is used for `listing the user's books` from the library by computing a `GET` request to the server. After that, it parses the repsonse and prints the resulted list of books ([ ] if there are no books to print).

## Get book
This function is used for `listing a book` from the library (given an `id`) by computing a `GET` request to the server. After that, it parses the repsonse and prints the book (or an error message if the user has no book saved with the given id).

## Add book
This function is used for `adding a book` to the library, by computing a `POST` request to the server. After that, it parses the repsonse and prints the result.

## Delete book
This function is used for `deleting a book` from the library (given an `id`) by computing a `DELETE` request to the server. After that, it parses the repsonse and prints the result.
