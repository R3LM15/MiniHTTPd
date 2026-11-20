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
        codigos_estado(client_sock,400);
        return -1;
    }else {
        printf("======================================\n");
        printf("Método: %s\n", method);   //"GET"
        printf("Path:   %s\n", path);     //"/index.html"
        printf("Versión: %s\n", version); //"HTTP/1.1">
        printf("======================================\n");
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

    if (strcmp(method, "GET") == 0 && strcmp(version, "HTTP/1.1") == 0) {
            // Procesar la solicitud GET
            int resultado = http_response(client_sock,path); // Llamar a la función http_response para enviar la respuesta al cliente
            if(resultado == -1){
            return -1;
            }

    } else {
            printf("Conexion Rechaza%s\n", method);
            codigos_estado(client_sock,405);
            return -1; // Método no soportado, cerrar la conexión
    }

    printf("\n");

    return 0;
}

int http_response(int client_sock, char *path) {
    size_t body_len = 0;
    int codigo_estado = 200;
    unsigned char *body = get_file(path, &body_len,&codigo_estado); // Obtener el contenido del archivo
    char headers[256];

    
    if (!body) {
        printf("Archivo no encontrado: %s\n", path);
        codigos_estado(client_sock,codigo_estado);
        return -1;
    } 


    int header_len = snprintf(headers, sizeof(headers),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: keep-alive\r\n"
        "\r\n",
        get_mime(path),
        body_len);

        if (send(client_sock, headers, header_len, 0) != header_len) // Enviar los encabezados de la respuesta al cliente 
        return -1;

        if (send(client_sock, body, body_len, 0) != (ssize_t)body_len) // Enviar el cuerpo de la respuesta al cliente
        return -1;
    return 0;
}

int codigos_estado(int client_sock, int codigo) {

    const char *respuesta = NULL;

    switch (codigo) {
        case 400:
            respuesta =
                "HTTP/1.1 400 Bad Request\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: 22\r\n"
                "Connection: close\r\n"
                "\r\n"
                "<h1>400 Bad Request</h1>\n";
            break;

        case 403:
            respuesta =
                "HTTP/1.1 403 Forbidden\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: 23\r\n"
                "Connection: close\r\n"
                "\r\n"
                "<h1>403 Forbidden</h1>\n";
            break;

        case 404:
            respuesta =
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: 23\r\n"
                "Connection: close\r\n"
                "\r\n"
                "<h1>404 Not Found</h1>\n";
            break;

         case 405:
            respuesta =
                "HTTP/1.1 405 Method Not Allowed\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: 23 \r\n"
                "Connection: close\r\n"
                "\r\n"
                "<h1>405 Method Not Allowed</h1>\n";
            break;

        case 500:
            respuesta =
                "HTTP/1.1 500 Internal Server Error\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: 35\r\n"
                "Connection: close\r\n"
                "\r\n"
                "<h1>500 Internal Server Error</h1>\n";
            break;

    }

    send(client_sock, respuesta, strlen(respuesta), 0);
    return -1;
}