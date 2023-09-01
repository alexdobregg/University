#include "requests.h"
#include "buffer.h"
#include "helpers.h"
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;


void logout_cmd(string host, int sockfd, int port, bool *logged_in,
                        bool *in_library, string *cookie, string *tokenJWT) {
    // If the client is already logged out, he can't log out again.
    if((*logged_in) == true) {
        sockfd = open_connection((char *) host.c_str(), port, AF_INET, SOCK_STREAM, 0);
        string url = "/api/v1/tema/auth/logout";
        // Computing the message.
        char *message = compute_get_request((char *) host.c_str(),
                                            (char *) url.c_str(), NULL,
                                            (char *) (*cookie).c_str(), NULL);
        send_to_server(sockfd, message);
        // Receiving the response from the server.
        char *response = receive_from_server(sockfd);
        // Seeing what the response is.
        if (strstr(response, "200 OK") != NULL) {
            // The log out was a success.
            cout << "\n[SUCCESS] You logged out successfully!\n\n";
            (*logged_in) = false;
            (*in_library) = false;
            (*tokenJWT).clear();
            (*cookie).clear();
        } else if (strstr(response, "429 Too Many Requests") != NULL) {
            // Too many requests to the server.
            char *pch = strstr(response, "Too many");
            cout << "\n[ERROR] " << pch << "\n\n";
        } else {
            // Just in case, a problem loggin out other than too many requests.
            cout << "\n[ERROR] You can't log out! Try again!\n\n";
        }
        free(message);
        free(response);
        close(sockfd);
    } else {
        cout << "\n[ERROR] You must be logged in first!\n\n";
    }
}

void register_cmd(string host, string contentType, int sockfd, int port) {
    // Reading the username and the password and creating a json object
    // in order to obtain the string needed to be sent to the server.
    string username, password;
    cout << "username=";
    cin >> username;
    cout << "password=";
    cin >> password;
    json j;
    j["username"] = username;
    j["password"] = password;
    string data = j.dump(4);
    sockfd = open_connection((char *) host.c_str(), port, AF_INET, SOCK_STREAM, 0);
    string url = "/api/v1/tema/auth/register";
    // Computing the message.
    char *message = compute_post_request((char *) host.c_str(), (char *) url.c_str(),
                                         (char *) contentType.c_str(),
                                         (char *) data.c_str(), NULL, NULL);
    send_to_server(sockfd, message);
    // Receiving the response from the server.
    char *response = receive_from_server(sockfd);
    // Seeing what the response is.
    if (strstr(response, "201 Created") != NULL) {
        // The creation was a success.
        cout << "\n[SUCCESS] The account was created successfully!\n\n";
    } else if (strstr(response, "429 Too Many Requests") != NULL) {
        // Too many requests error.
        char *pch = strstr(response, "Too many");
        cout << "\n[ERROR] " << pch << "\n\n";
    } else {
        // An error for when the username that was typed is already existing.
        char *pch = strstr(response, "{");
        string toky1, toky2;
        toky1.append(pch);
        json j = json::parse(toky1);
        toky2 = j.at("error").dump();
        toky2.erase(toky2.begin() + 0);
        toky2.erase(toky2.begin() + toky2.length() - 1);
        cout << "\n[ERROR] " << toky2 << " Try again using a different username!\n\n";
    }
    free(message);
    free(response);
    close(sockfd);
}

void login_cmd(string host, string contentType, int sockfd, int port,
                                     bool *logged_in, string *cookie) {
    // If the client is already logged in, he can't log in again.
    if((*logged_in) == false) {
        // Reading the username and the password and creating a json object
        // in order to obtain the string needed to be sent to the server.
        string username, password;
        cout << "username=";
        cin >> username;
        cout << "password=";
        cin >> password;
        json j;
        j["username"] = username;
        j["password"] = password;
        string data = j.dump(4);
        sockfd = open_connection((char *) host.c_str(), port, AF_INET, SOCK_STREAM, 0);
        string url = "/api/v1/tema/auth/login";
        // Computing the message.
        char *message = compute_post_request((char *) host.c_str(), (char *) url.c_str(),
                                             (char *) contentType.c_str(), 
                                             (char *) data.c_str(), NULL, NULL);
        send_to_server(sockfd, message);
        // Receiving the response from the server.
        char *response = receive_from_server(sockfd);
        if (strstr(response, "200 OK") != NULL) {
            // The login was a success.
            cout << "\n[SUCCESS] You logged in successfully!\n\n";
            // Getting the cookie that is necessary for entering the library.
            char *pch = strstr(response, "Set-Cookie:");
            char *stk = strtok(pch, " ");
            stk = strtok(NULL, ";");
            (*cookie).append(stk);
            (*logged_in) = true;
        } else if (strstr(response, "429 Too Many Requests") != NULL) {
            // Too many requests error.
            char *pch = strstr(response, "Too many");
            cout << "\n[ERROR] " << pch << "\n\n";
        } else {
            // The credentials are not good or the username does not exist errors.
            char *pch = strstr(response, "{");
            string toky1, toky2;
            toky1.append(pch);
            json j = json::parse(toky1);
            toky2 = j.at("error").dump();
            toky2.erase(toky2.begin() + 0);
            toky2.erase(toky2.begin() + toky2.length() - 1);
            cout << "\n[ERROR] " << toky2 << "\n\n";
        }
        free(message);
        free(response);
        close(sockfd);
    } else {
        cout << "\n[ERROR] You are already logged in!\n\n";
    }
}

void enter_library_cmd(string host, string contentType, string cookie,
                         string *tokenJWT, bool *in_library, int sockfd, int port) {
    // If the client is already in the library, he can't enter again.
    if((*in_library) == true) {
        cout << "\n[ERROR] You are already in the library!\n\n";
    } else {
        sockfd = open_connection((char *) host.c_str(), port, AF_INET, SOCK_STREAM, 0);
        string url = "/api/v1/tema/library/access";
        // Computing the message.
        char *message = compute_get_request((char *) host.c_str(), (char *) url.c_str(),
                                             NULL, (char *) cookie.c_str(), NULL);
        send_to_server(sockfd, message);
        // Receiving the response from server.
        char *response = receive_from_server(sockfd);
        if (strstr(response, "401 Unauthorized") != NULL) {
            // If the client is not logged in, and error will pop up.
            char *pch = strstr(response, "{");
            string toky1, toky2;
            toky1.append(pch);
            json j = json::parse(toky1);
            toky2 = j.at("error").dump();
            toky2.erase(toky2.begin() + 0);
            toky2.erase(toky2.begin() + toky2.length() - 1);
            cout << "\n[ERROR] " << toky2 << "\n\n";
        } else if (strstr(response, "429 Too Many Requests") != NULL) {
            // Too many requests error.
            char *pch = strstr(response, "Too many");
            cout << "\n[ERROR] " << pch << "\n\n";
        } else {
            // Entering the library was a success.
            cout << "\n[SUCCESS] You entered the library!\n\n";
            // Getting the JWT token to prove that the client is in the library.
            char *pch = strstr(response, "{");
            string toky;
            toky.append(pch);
            json j = json::parse(toky);
            (*tokenJWT) = j.at("token").dump();
            (*tokenJWT).erase((*tokenJWT).begin() + 0);
            (*tokenJWT).erase((*tokenJWT).begin() + (*tokenJWT).length() - 1);
            (*in_library) = true;
        }
        free(message);
        free(response);
        close(sockfd);
    }
}

void get_books_cmd(string host, string tokenJWT, bool logged_in,
                                 bool in_library, int sockfd, int port) {
    // The client must be logged in to obtain the books.
    if (logged_in == false) {
        cout << "\n[ERROR] You are not logged in!\n\n";
    } else {
        // The client must be in the libraray to obtain the books.
        if (in_library == false) {
            cout << "\n[ERROR] You must enter the library first!\n\n";
        } else {
            sockfd = open_connection((char *) host.c_str(), port, AF_INET, SOCK_STREAM, 0);
            string url = "/api/v1/tema/library/books";
            // Computin the message.
            char *message = compute_get_request((char *) host.c_str(), (char *) url.c_str(),
                                                 NULL, NULL, (char *) tokenJWT.c_str());
            send_to_server(sockfd, message);
            // Receiving the response from the server.
            char *response = receive_from_server(sockfd);
            // Obtaining the books from the server.
            char *pch = strstr(response, "[");
            string toky1, toky2;
            toky1.append(pch);
            json j = json::parse(toky1);
            toky2 = j.dump(4);
            cout << "\n[SUCCESS] The books from the library are:\n\n" << toky2 << "\n\n";
            free(message);
            free(response);
            close(sockfd);
        }
    }
}

void get_book_cmd(string host, string tokenJWT, bool logged_in,
                                 bool in_library, int sockfd, int port) {
    // The client must be logged in to obtain the book.
    if (logged_in == false) {
        cout << "\n[ERROR] You are not logged in!\n\n";
    } else {
        // The client must be in the library to obtain the book.
        if (in_library == false) {
            cout << "\n[ERROR] You must enter the library first!\n\n";
        } else {
            string id;
            cout << "id=";
            cin >> id;
            sockfd = open_connection((char *) host.c_str(), port, AF_INET, SOCK_STREAM, 0);
            string url = "/api/v1/tema/library/books/";
            url.append(id);
            // Computing the message.
            char *message = compute_get_request((char *) host.c_str(), (char *) url.c_str(),
                                                 NULL, NULL, (char *) tokenJWT.c_str());
            send_to_server(sockfd, message);
            // Receiving the response from the server.
            char *response = receive_from_server(sockfd);
            if (strstr(response, "200 OK") != NULL) {
                // Getting a book by its id was a success.
                char *pch = strstr(response, "[{");
                string toky1, toky2;
                toky1.append(pch);
                json j = json::parse(toky1);
                toky2 = j.dump(4);
                cout << "\n[SUCCESS] The book with id " << id << " is :\n\n" << toky2 << "\n\n";
            } else if (strstr(response, "429 Too Many Requests") != NULL) {
                // Too many requests error.
                char *pch = strstr(response, "Too many");
                cout << "\n[ERROR] " << pch << "\n\n";
            } else {
                // There is no book with the provided id in the library.
                char *pch = strstr(response, "{");
                string toky1, toky2;
                toky1.append(pch);
                json j = json::parse(toky1);
                toky2 = j.at("error").dump();
                toky2.erase(toky2.begin() + 0);
                toky2.erase(toky2.begin() + toky2.length() - 1);
                cout << "\n[ERROR] " << toky2 << " Try using get_books to see the exact id of a book!\n\n";
            }
            free(message);
            free(response);
            close(sockfd);
        }
    }
}


void add_book_cmd(string host, string contentType, string tokenJWT, bool logged_in,
                                             bool in_library, int sockfd, int port) {
    // The client must be logged in to add a book.
    if (logged_in == false) {
        cout << "\n[ERROR] You are not logged in!\n\n";
    } else {
        // The client must be in the library to add a book.
        if (in_library == false) {
            cout << "\n[ERROR] You must enter the library first!\n\n";
        } else {
            // Reading the information about a book in order to add it to the library.
            string title, author, genre, publisher;
            int page_count;
            string to_throw;
            getline(cin, to_throw);
            cout << "title=";
            getline(cin, title);
            cout << "author=";
            getline(cin, author);
            cout << "genre=";
            getline(cin, genre);
            cout << "publisher=";
            getline(cin, publisher);
            cout << "page_count=";
            cin >> page_count;
            json j;
            j["title"] = title;
            j["author"] = author;
            j["genre"] = genre;
            j["publisher"] = publisher;
            j["page_count"] = page_count;
            string data = j.dump(4);
            sockfd = open_connection((char *) host.c_str(), port, AF_INET, SOCK_STREAM, 0);
            string url = "/api/v1/tema/library/books";
            // Computing the message.
            char *message = compute_post_request((char *) host.c_str(), (char *) url.c_str(),
                                                 (char *) contentType.c_str(), (char *) data.c_str(), NULL,
                                                 (char *) tokenJWT.c_str());
            send_to_server(sockfd, message);
            // Receiving the resposonse from the server.
            char *response = receive_from_server(sockfd);
            if (strstr(response, "200 OK") != NULL) {
                cout << "\n[SUCCESS] Book added successfully!\n\n";
            } else if (strstr(response, "429 Too Many Requests") != NULL) {
                // Too many requests error.
                char *pch = strstr(response, "Too many");
                cout << "\n[ERROR] " << pch << "\n\n";
            } else {
                // Just in case the book is not added.
                cout << "\n[ERROR] The book was not added! Try again!\n\n";
            }
            free(message);
            free(response);
            close(sockfd);
        }
    }
}

void delete_book_cmd(string host, string tokenJWT, bool logged_in,
                                 bool in_library, int sockfd, int port) {
    // The client must be logged in to delete a book.
    if (logged_in == false) {
        cout << "\n[ERROR] You are not logged in!\n\n";
    } else {
        // The client must be in the library to delete a book.
        if (in_library == false) {
            cout << "\n[ERROR] You must enter the library first!\n\n";
        } else {
            string id;
            cout << "id=";
            cin >> id;
            sockfd = open_connection((char *) host.c_str(), port, AF_INET, SOCK_STREAM, 0);
            string url = "/api/v1/tema/library/books/";
            url.append(id);
            // Computing the message.
            char *message = compute_delete_request((char *) host.c_str(), (char *) url.c_str(),
                                                    (char *) tokenJWT.c_str());
            send_to_server(sockfd, message);
            // Receiving the response from the server.
            char *response = receive_from_server(sockfd);
            if (strstr(response, "404 Not Found") != NULL) {
                // The book was not found in the library.
                char *pch = strstr(response, "{");
                string toky1, toky2;
                toky1.append(pch);
                json j = json::parse(toky1);
                toky2 = j.at("error").dump();
                toky2.erase(toky2.begin() + 0);
                toky2.erase(toky2.begin() + toky2.length() - 1);
                cout << "\n[ERROR] " << toky2 << " No book has id " << id << "!\n\n";
            } else if (strstr(response, "429 Too Many Requests") != NULL) {
                // Too many requests error.
                char *pch = strstr(response, "Too many");
                cout << "\n[ERROR] " << pch << "\n\n";
            } else {
                cout << "\n[SUCCESS] The book was deleted successfully!\n\n";
            }
            free(message);
            free(response);
            close(sockfd);
        }
    }
    
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    int port = 8080;
    int sockfd;
    bool logged_in = false;
    bool in_library = false;
    string host = "34.118.48.238";
    string contentType = "application/json";
    string buffer;
    string cookie;
    string tokenJWT;

    // It shows the commands when the client is started.
    cout << "The only accepted commands are the following ones:\n";
    cout << "register - creates a new account\n";
    cout << "login - logs into an existing account\n";
    cout << "enter_library - enters the library\n";
    cout << "get_books - summary information about the books from the library\n";
    cout << "get_book - information about a book from the library\n";
    cout << "add_book - adds a new book to the library\n";
    cout << "delete_book - deletes a book from the library\n";
    cout << "logout - logs you out of your account\n";
    cout << "exit - logs you out if you are logged in and closes the program\n\n";

    while(true) {
        cout << "Enter a comand:\n";
        cin >> buffer;
        if (buffer.compare("exit") == 0) {
            if (logged_in) {
                logout_cmd(host, sockfd, port, &logged_in, &in_library, &cookie, &tokenJWT);
            }
            break;
        } else if (buffer.compare("register") == 0) {
            register_cmd(host, contentType, sockfd, port);
        } else if (buffer.compare("login") == 0) {
            login_cmd(host, contentType, sockfd, port, &logged_in, &cookie);
        } else if (buffer.compare("enter_library") == 0) {
            enter_library_cmd(host, contentType, cookie, &tokenJWT, &in_library ,sockfd, port);
        } else if (buffer.compare("get_books") == 0) {
            get_books_cmd(host, tokenJWT, logged_in, in_library, sockfd, port);
        } else if (buffer.compare("get_book") == 0) {
            get_book_cmd(host, tokenJWT, logged_in, in_library, sockfd, port);
        } else if (buffer.compare("add_book") == 0) {
            add_book_cmd(host, contentType, tokenJWT, logged_in, in_library, sockfd, port);
        } else if (buffer.compare("delete_book") == 0) {
            delete_book_cmd(host, tokenJWT, logged_in, in_library, sockfd, port);
        } else if (buffer.compare("logout") == 0) {
            logout_cmd(host, sockfd, port, &logged_in, &in_library, &cookie, &tokenJWT);
        } else {
            cout << "\n[ERROR] The only accepted commands are the following ones:\n";
            cout << "register - creates a new account\n";
            cout << "login - logs into an existing account\n";
            cout << "enter_library - enters the library\n";
            cout << "get_books - summary information about the books from the library\n";
            cout << "get_book - information about a book from the library\n";
            cout << "add_book - adds a new book to the library\n";
            cout << "delete_book - deletes a book from the library\n";
            cout << "logout - logs you out of your account\n";
            cout << "exit - logs you out if you are logged in and closes the program\n\n";
        }
    }
    return 0;
}