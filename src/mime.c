#include "mime.h"
#include <string.h>

typedef struct {
    const char *extension;
    const char *mime_type;
} mime_entry;

static const mime_entry mime_types[] = {
    {".html", "text/html"},
    {".css", "text/css"},
    {".js", "application/javascript"},
    {".png", "image/png"},
    {".jpg", "image/jpeg"},
    {NULL,    NULL}   // terminador
};

const char *get_mime(const char *path) {
    if (strcmp(path, "/") == 0) {
    return "text/html";
    }
    const char *ext = strrchr(path, '.');
    if (!ext) {
        return "application/octet-stream";
    }

    for (int i = 0; mime_types[i].extension != NULL; i++) {
        if (strcmp(ext, mime_types[i].extension) == 0) {
            return mime_types[i].mime_type;
        }
    }

    return "application/octet-stream";  // tipo por defecto;  // tipo por defecto
}