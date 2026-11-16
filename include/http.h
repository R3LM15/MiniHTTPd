#ifndef HTTP_H
#define HTTP_H

int http_request(int client_sock, char *buffer);
int http_response(int client_sock, char *path);

#endif