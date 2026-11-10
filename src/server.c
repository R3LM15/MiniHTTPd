#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

int create_server_TCP(int port){ 


    int sockTCP = socket(AF_INET, SOCK_STREAM, 0); //crea socket TCP
    struct sockaddr_in server_addr;
    int opt = 1;

    if(sockTCP < 0){
            perror("socket");
            close(sockTCP);
            return -1;
        }


    int sd = setsockopt(sockTCP, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); //Configurar opciones del socket para permitir reutilizar la dirección/puerto

    if(sd < 0){
        perror("ERROR en setsockopt");
        close(sockTCP);
        return -1;
    }

    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);


    int bd = bind(sockTCP, (struct sockaddr *)&server_addr, sizeof(server_addr)); //Asociar el socket a una dirección IP y puerto específicos

    if(bd < 0){
        perror("ERROR en bind");
        close(sockTCP);
        return -1;
    }

    int ls = listen(sockTCP, 5); //Poner el socket en modo de escucha para aceptar conexiones entrantes}

    if(ls < 0){
        perror("ERROR en listen");
        close(sockTCP);
        return -1;
    }

    return sockTCP;
}

