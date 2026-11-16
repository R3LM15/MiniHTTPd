#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "files.h"
#include "mime.h"



unsigned char *get_file(const char *path, size_t *file_size) {
    char filepath[512];

    // Si piden "/", servimos www/index.html
    if (strcmp(path, "/") == 0) {
        strcpy(filepath, "www/index.html");
    }
    // Si piden "/style.css", servimos www/style.css
    if (strncmp(path, "/style.css", 14) == 0) {
        strcpy(filepath, "www/style.css");
    }

    printf("Obteniendo archivo: %s\n", filepath);

    FILE *file = fopen(filepath, "rb");
    if (!file) {
        perror("Error al abrir el archivo");
        return NULL;
    }

    // Ir al final para saber el tamaño
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    if (size < 0) {
        perror("Error al obtener el tamaño del archivo");
        fclose(file);
        return NULL;
    }
    *file_size = (size_t)size;
    fseek(file, 0, SEEK_SET);

    // Reservar memoria
    unsigned char *buffer = malloc(*file_size + 1);
    if (!buffer) {
        perror("Error al asignar memoria");
        fclose(file);
        return NULL;
    }

    // Leer el archivo completo
    size_t read_bytes = fread(buffer, 1, *file_size, file);
    if (read_bytes != *file_size) {
        perror("Error al leer el archivo");
        free(buffer);
        fclose(file);
        return NULL;
    }

    buffer[*file_size] = '\0';  // por si acaso (útil si es texto)
    fclose(file);
    
    return buffer;
}