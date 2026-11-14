#include <stdio.h>

char method[16];
char path[256];
char version[16];


int https_request(int client_sock, char *buffer, char *ip) {
    

    int cc = sscanf(buffer, "%s %s %s", method, path, version);

    if (cc != 3) {
        printf("Error en pedito HTTP \n");
    }else {
        printf("===========================\n");
        printf("Método: %s\n", method);   //"GET"
        printf("Path:   %s\n", path);     //"/index.html"
        printf("Versión: %s\n", version); //"HTTP/1.1"
    }
    printf("Socket del cliente: %d\n", client_sock);
    printf("Dirección IP del cliente: %s\n", ip);


    return 0;
}