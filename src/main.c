#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "server.h"
#include "http.h"
#include "files.h"
#include "mime.h"

#define MAX_EVENTS  128

int main(int argc, char *argv[]){ 

int port, sockTCP;

        if (argc < 2) {
            fprintf(stderr, "USO: %s <puerto>\n", argv[0]);
            exit(1);
        }

        port = atoi(argv[1]);
        sockTCP = create_server_TCP(port); //crear servidor TCP

        if (sockTCP < 0) {
            fprintf(stderr, "No se pudo crear el servidor\n");
            exit(1);
        }

        int epct = epoll_create1(0); //crear instancia de epoll para manejar múltiples conexiones de clientes

        if (epct < 0) {
            perror("epoll_create");
            exit(1);
        }

        struct epoll_event events[MAX_EVENTS];
        struct epoll_event ev = {0};                    
        ev.events = EPOLLIN;
        ev.data.fd = sockTCP;

       int ep = epoll_ctl(epct, EPOLL_CTL_ADD, sockTCP, &ev); //registrar el socket del servidor en la instancia de epoll para monitorear eventos de entrada

       if (ep < 0) {
            perror("epoll_ctl");
            exit(1);
        }

        printf("Servidor en http://localhost:%d\n", port);
        printf("Sirviendo archivos desde el directorio actual.\n");

    while (1) {
        
    int n = epoll_wait(epct, events, MAX_EVENTS, -1); //esperar eventos en los sockets registrados en la instancia de epoll

        if (n < 0) {
            perror("epoll_wait");
            exit(1);
        }

      for (int i = 0; i < n; i++) {
            if (events[i].data.fd == sockTCP) {
                int client_sock = accept_client(sockTCP);

                if (client_sock < 0) {
                    fprintf(stderr, "Error aceptando cliente\n");
                    continue;
                }
                    
                    struct epoll_event client_ev = {0};
                    client_ev.events = EPOLLIN;
                    client_ev.data.fd = client_sock;
                    
                    
                if (epoll_ctl(epct, EPOLL_CTL_ADD, client_sock, &client_ev) < 0) {
                perror("Error registrando cliente");
                close(client_sock);
                }

            } else {
                int client_sock = events[i].data.fd;
                char buffer[1024];
                ssize_t lectura = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
                

                if (lectura < 0) {
                    perror("Error leyendo del cliente");
                    epoll_ctl(epct, EPOLL_CTL_DEL, client_sock, NULL);
                    close(client_sock);
                    continue;
                }

                if (lectura == 0) {
                    // El cliente cerró la conexión
                    char ip[INET_ADDRSTRLEN];
                    Obtener_ip(client_sock, ip, sizeof(ip)); //obtiene la ip del socket del cliente antes de cerrarlo
                    printf("-->  Cliente desconectado: %s\n", ip);
                    epoll_ctl(epct, EPOLL_CTL_DEL, client_sock, NULL);
                    close(client_sock);
                    continue;
                }

                if (lectura > 0) {
                    buffer[lectura] = '\0'; // Null-terminate the buffer
                    // Procesar la solicitud HTTP y enviar la respuesta + identificador de ip 
                    https_request(client_sock, buffer);
                    continue;
                }
              

            }


        }
    

    }
    
}
