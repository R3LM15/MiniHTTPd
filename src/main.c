#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
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

        struct epoll_event ev = {0};
        struct epoll_event events[MAX_EVENTS];
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
                if (client_sock >= 0) {
                    //Funcion para soliticar y enviar archivos al cliente
                    close(client_sock);
                }
            }
        }
    




    }
    


    close(sockTCP);






}
