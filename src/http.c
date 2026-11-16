#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include "server.h"
#include "http.h"
#include "files.h"
#include "mime.h"


int http_request(int client_sock, char *buffer) {
    
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
        
        http_response(client_sock,path); // Llamar a la función http_response para enviar la respuesta al cliente
    } else {
        printf("Conexion Rechaza%s\n", method);
        close(client_sock); // Cerrar el socket del cliente si el método no es GET
    }

    printf("\n");

    return 0;
}

int http_response(int client_sock, char *path) {
    size_t body_len = 0;
    unsigned char *body = get_file(path, &body_len); // Obtener el contenido del archivo
    char headers[256];

    if (!body) {
        // Archivo no encontrado
        const char *not_found =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 22\r\n"
            "Connection: close\r\n"
            "\r\n"
            "<h1>404 Not Found</h1>";
        send(client_sock, not_found, strlen(not_found), 0);
        close(client_sock);
        return -1;
    }


    printf("path: %s\n", path);
    
    int header_len = snprintf(headers, sizeof(headers),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: keep-alive\r\n"
        "\r\n",
        get_mime(path),
        body_len);

        printf("PATH: %s\n", path);
        printf("MIME: %s\n", get_mime(path));
        printf("TAMAÑO: %zu\n", body_len);

    if (send(client_sock, headers, header_len, 0) != header_len) 
        return -1;

    if (send(client_sock, body, body_len, 0) != (ssize_t)body_len) // Enviar el cuerpo de la respuesta al cliente
    return -1;
    

    return 0;
}
