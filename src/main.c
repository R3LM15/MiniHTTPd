#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "server.h"
#include "http.h"
#include "files.h"
#include "mime.h"

int main(int argc, char *argv[]){ 

int port, sockTCP;

    if (argc < 2) {
        fprintf(stderr, "USO: %s <puerto>\n", argv[0]);
        exit(1);
    }

    port = atoi(argv[1]);
    sockTCP = create_server_TCP(port);

    if (sockTCP < 0) {
        fprintf(stderr, "No se pudo crear el servidor\n");
        exit(1);
    }

    printf("Servidor TCP escuchando en el puerto %d\n", port);

    // Aquí iría el bucle de accept + fork...




    close(sockTCP);






}
