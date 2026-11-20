# MiniHTTPd

## 1. Descripcion del proyecto

MiniHTTPd es un servidor HTTP/1.1 basico desarrollado en lenguaje C para sistemas Linux. 

**Objetivo:** 

[Implementar un Servidor concurrente con un hilo que atienda a varios clientes con la implementacion de epoll]

---

## 2. Caracteristicas principales

El servidor implementa las siguientes funcionalidades:

- [x] Servidor basado en sockets TCP.
- [x] Procesamiento de solicitudes HTTP mediante el metodo GET.
- [x] Analisis de encabezados HTTP/1.1.
- [x] Gestion de multiples clientes mediante epoll.
- [x] Servir archivos estaticos.
- [x] Identificacion de tipos MIME.
- [x] Manejo de codigos de estado HTTP.
- [x] Proteccion contra Directory Traversal.

---

## 3. Arquitectura del proyecto


### Descripcion de los modulos

| Archivo | Responsabilidad |
|---|---|
| `main.c` | [Event Loop - Control de Concurrencia con epoll] |
| `server.c` | [Creacion de Sockets Servidor y Cliente] |
| `http.c`   | [Manejo de Pedido - Respuesta HTTP codigos de estado] |
| `files.c` | [Get_file - Logica y seguridad del manejo de los archivos estaticos  ] |
| `mime.c` | [Identificar el tipo MIME de los archivos solicitados por el cliente HTTP mediante su extensión]
---

## 4. Estructura de directorios

```
├── Makefile
├── README.md
├── include
│   ├── files.h
│   ├── http.h
│   ├── mime.h
│   └── server.h
├── minihttpd
├── src
│   ├── files.c
│   ├── http.c
│   ├── main.c
│   ├── mime.c
│   └── server.c
└── www
    ├── imagen.png
    ├── index.html
    └── style.css
```
## 6. Compilacion

Desde el directorio principal del proyecto, ejecutar:

```bash
make
```

Para limpiar los archivos generados durante la compilacion:

```bash
make clean
```

---

## 7. Ejecucion

Para iniciar el servidor:

```bash
./minihttpd 8080
```

Una vez iniciado, acceder desde un navegador mediante:

```text
http://localhost:8080
```

## 8. Pruebas

Se realizaron pruebas utilizando [navegador / curl / otras herramientas].

### Ejemplos de solicitudes

Consultar la pagina principal:

```bash
curl -i http://localhost:8080/
```

Consultar un archivo CSS:

```bash
curl -i http://localhost:8080/style.css
```

Consultar una ruta inexistente:

```bash
curl -i http://localhost:8080/noexiste.html
```

## 9. Codigos de estado HTTP

| Codigo | Descripcion |
|---|---|
| 200 | OK |
| 400 | Bad Request |
| 403 | Forbidden |
| 404 | Not Found |
| 405 | Method Not Allowed |
| 500 | Internal Server Error |

---

## 10. Tipos MIME

| Extension | Tipo MIME |
|---|---|
| `.html` | `text/html` |
| `.css` | `text/css` |
| `.js` | `application/javascript` |
| `.png` | `image/png` |
| `.jpg` | `image/jpeg` |

---

## 11. Autor

**Nombre:** [Mateo Molina Moises Estrella]

**Institucion:** Escuela Politecnica Nacional (EPN)

**Asignatura:** [Computación Distribuida]

**Proyecto:** MiniHTTPd
