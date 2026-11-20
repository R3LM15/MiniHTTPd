#ifndef HTTP_H
#define HTTP_H

int http_request(int client_sock, char *buffer);
int http_response(int client_sock, char *path);
int codigos_estado(int client_sock, int codigo);

#endif