minihttpd/
├── Makefile
├── README.md
├── include/
│ ├── http.h
│ ├── server.h
│ ├── mime.h
│ └── files.h
├── src/
│ ├── main.c
│ ├── server.c
│ ├── http.c
│ ├── mime.c
│ └── files.c
├── www/
 ├── index.html
 ├── style.css
 └── image.png


                   CLIENTE (Navegador)
                          |
                          | HTTP Request
                          v
                  +---------------+
                  |   main.c      |
                  |  Event Loop   |
                  +---------------+
                          |
                          v
                  +---------------+
                  |   server.c    |
                  | Sockets/epoll |
                  +---------------+
                          |
                          | Datos recibidos
                          v
                  +---------------+
                  |    http.c     |
                  | HTTP Parsing  |
                  +---------------+
                     /         \
                    v           v
             +-----------+  +-----------+
             |  files.c  |  |  mime.c   |
             | Archivos  |  | Tipos MIME|
             +-----------+  +-----------+
                    |           |
                    +-----+-----+
                          |
                          v
                  HTTP Response
                          |
                          v
                  CLIENTE (Navegador)