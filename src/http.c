#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "server.h"



int https_request(int client_sock, char *buffer) {
    
        char method[16];
        char path[256];
        char version[16];
        char host[32];
        char connection[32];
        char user_agent[256];


    int cc = sscanf(buffer, "%15s %255s %15s", method, path, version); // Analizar la solicitud HTTP y extraer el método, la ruta y la versión del protocolo
    //printf("Datos recibidos: %s\n", buffer);
    if (cc != 3) {
        printf("Error en pedito HTTP \n");
    }else {
        printf("===========================\n");
        printf("Método: %s\n", method);   //"GET"
        printf("Path:   %s\n", path);     //"/index.html"
        printf("Versión: %s\n", version); //"HTTP/1.1">
        printf("===========================\n");
    }

    while (1) {
        char *line_end = strchr(buffer, '\n');
        if (line_end == NULL || line_end == buffer) {
            break; // Salir del bucle si no hay más líneas o si es una línea vacía
        }

        *line_end = '\0'; // Reemplazar el salto de línea con un carácter nulo para terminar la cadena
        //Busca los diferentes encabezados HTTP y los imprime
        if (sscanf(buffer, "Host: %31s", host) == 1) {
            printf("Host: %s\n", host);
        }
        if (sscanf(buffer, "Connection: %31s", connection) == 1) {
            printf("Connection: %s\n", connection);
        }
        if (sscanf(buffer, "User-Agent: %255[^\n]", user_agent) == 1) {
            printf("User-Agent: %s\n", user_agent);
        }

        buffer = line_end + 1; // Mover el puntero al inicio de la siguiente línea
    } 
   
    printf("Socket del cliente: %d\n", client_sock);
    char ip[INET_ADDRSTRLEN];
    Obtener_ip(client_sock, ip, sizeof(ip)); // Obtener la dirección IP del cliente asociado al socket
    printf("Dirección IP del cliente: %s\n", ip);


     if (strcmp(method, "GET") == 0) {
        // Procesar la solicitud GET
        printf("Procesando solicitud GET para: %s\n", path);
    } else {
        printf("Método 405 no permitido: %s\n", method);
    }

    printf("\n");

    return 0;
}