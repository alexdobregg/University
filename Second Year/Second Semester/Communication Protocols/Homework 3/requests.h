#ifndef _REQUESTS_
#define _REQUESTS_

// computes and returns a GET request string (query_params
// and cookies can be set to NULL if not needed)
char *compute_get_request(char *host, char *url, char *query_params,
							char *cookies, char *header);

// computes and returns a POST request string (cookies can be NULL if not needed)
char *compute_post_request(char *host, char *url, char  *content_type, char *body_data,
							char *cookie, char *header);

char *compute_delete_request(char *host, char *url, char *header);


#endif