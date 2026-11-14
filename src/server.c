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

int accept_client(int sockTCP){
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int ca = accept(sockTCP, (struct sockaddr *)&client_addr, &client_len); //Aceptar una conexión entrante de un cliente

    if(ca < 0){
        perror("ERROR en accept");
        return -1;
    }

    return ca;
}

int Obtener_ip(int sockTCP, char *ip, size_t ip_size) {
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    if (getpeername(sockTCP, (struct sockaddr *)&addr, &addr_len) < 0) { //Obtener la dirección IP del socket del servidor
        perror("ERROR en getsockname");
        return -1;
    }

    if (inet_ntop(AF_INET, &addr.sin_addr, ip, ip_size) == NULL) { //Convertir la dirección IP a una cadena legible
        perror("ERROR en inet_ntop");
        return -1;
    }


    return 0;
}

