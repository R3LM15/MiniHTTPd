#include <stdio.h>
#include <arpa/inet.h>
#include "server.h"

char method[16];
char path[256];
char version[16];


int https_request(int client_sock, char *buffer) {
    

    int cc = sscanf(buffer, "%s %s %s", method, path, version); // Analizar la solicitud HTTP y extraer el método, la ruta y la versión del protocolo

    if (cc != 3) {
        printf("Error en pedito HTTP \n");
    }else {
        printf("===========================\n");
        printf("Método: %s\n", method);   //"GET"
        printf("Path:   %s\n", path);     //"/index.html"
        printf("Versión: %s\n", version); //"HTTP/1.1"
    }
    printf("Socket del cliente: %d\n", client_sock);
    char ip[INET_ADDRSTRLEN];
    Obtener_ip(client_sock, ip, sizeof(ip)); // Obtener la dirección IP del cliente asociado al socket
    printf("Dirección IP del cliente: %s\n", ip);


    return 0;
}