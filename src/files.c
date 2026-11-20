#define _GNU_SOURCE
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h> 
#include "files.h"


unsigned char *get_file(const char *path, size_t *file_size) {
    char filepath[PATH_MAX];
    char ruta_base[PATH_MAX];
    char ruta_archivo[PATH_MAX]; 
    
    
    if (strcmp(path, "/") == 0) {
        strcpy(filepath, "www/index.html"); // Si piden "/", servimos www/index.html
    }else{
        strcpy(filepath, "www");            
        strcat(filepath, path);            // www + path 
    }
    printf("Obteniendo archivo: %s\n", filepath);
    
    //obtener la raiz absoluta directorio 
    if(realpath("www", ruta_base) == NULL){
        printf("Error en directorio");
        return NULL;
    }  

    //obtener la raiz absoluta del archivo
    if (realpath(filepath, ruta_archivo) == NULL) {
        printf("Error al resolver archivo");
        return NULL;
    }

    // Comprobar que el archivo pertenezca al directorio www
    size_t longitud = strlen(ruta_base);

    if(strncmp(ruta_base, ruta_archivo, longitud) != 0|| (ruta_archivo[longitud] != '/' && ruta_archivo[longitud] != '\0')){
        printf("No Permitido");
        return NULL;
    }
    // abrir el archivo validado
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