// usuarios.h
#ifndef USUARIOS_H
#define USUARIOS_H

#define MAX_LINEA 256

typedef struct {
    char usuario[64];
    char clave[64];
    char nombre[128];
} Usuario;

int cargar_usuarios(const char *ruta, Usuario **lista, int *n);
int autenticar(const char *ruta, Usuario *usuario_logueado);

#endif