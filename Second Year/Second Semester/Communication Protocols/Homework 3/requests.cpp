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

char *compute_get_request(char *host, char *url, char *query_params,
                            char *cookie, char *header)
{
    char *message = (char *) calloc(BUFLEN, sizeof(char));
    char *line = (char *) calloc(LINELEN, sizeof(char));

    if (query_params != NULL) {
        sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }
    compute_message(message, line);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    if (cookie != NULL) {
        sprintf(line, "Cookie: %s", cookie);
        compute_message(message, line);
    }
    if (header != NULL) {
        sprintf(line, "Authorization: Bearer %s", header);
        compute_message(message, line);
    }
    compute_message(message, "");
    free(line);
    return message;
}

char *compute_post_request(char *host, char *url, char* content_type, char *body_data,
                            char *cookie, char *header)
{
    char *message = (char *) calloc(BUFLEN, sizeof(char));
    char *line = (char *) calloc(LINELEN, sizeof(char));
    char *body_data_buffer = (char *) calloc(LINELEN, sizeof(char));

    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);
    int size = 0;
    if (body_data) {
        size = strlen(body_data);
        strcpy(body_data_buffer, body_data);
    }
    sprintf(line, "Content-Length: %d", size);
    compute_message(message, line);
    if (cookie != NULL) {
        sprintf(line, "Cookie: %s", cookie);
        compute_message(message, line);
    }
    if (header != NULL) {
        sprintf(line, "Authorization: Bearer %s", header);
        compute_message(message, line);
    }
    compute_message(message, "");
    memset(line, 0, LINELEN);
    compute_message(message, body_data_buffer);
    free(body_data_buffer);
    free(line);
    return message;
}

char *compute_delete_request(char *host, char *url, char *header) {
    char *message = (char *) calloc(BUFLEN, sizeof(char));
    char *line = (char *) calloc(LINELEN, sizeof(char));

    sprintf(line, "DELETE %s HTTP/1.1", url);
    compute_message(message, line);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    if (header != NULL) {
        sprintf(line, "Authorization: Bearer %s", header);
        compute_message(message, line);
    }
    compute_message(message, "");
    free(line);
    return message;
}
