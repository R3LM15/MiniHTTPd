

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