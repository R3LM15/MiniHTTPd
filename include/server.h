#ifndef SERVER_H
#define SERVER_H

int create_server_TCP(int port);
int accept_client(int serverTCP);
int Obtener_ip(int sockTCP, char *ip, size_t ip_size);


#endif
