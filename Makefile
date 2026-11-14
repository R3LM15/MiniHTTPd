CC = gcc
CFLAGS = -Wall -Wextra -g

# Directorio de encabezados
INCLUDES = -Iinclude

# Nombre del ejecutable
TARGET = minihttpd

# Archivos fuente
SRC = src/main.c src/server.c src/http.c


# Regla principal
all:
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC) -o $(TARGET)

# Limpiar archivos generados
clean:
	rm -f $(TARGET)