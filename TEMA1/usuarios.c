// usuarios.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usuarios.h"

int cargar_usuarios(const char *ruta, Usuario **lista, int *n) {
    FILE *f = fopen(ruta, "r");
    if (!f) return 0;
    char linea[MAX_LINEA];
    int capacidad = 8;
    *lista = malloc(sizeof(Usuario) * capacidad);
    *n = 0;
    while (fgets(linea, sizeof(linea), f)) {
        // quitar newline
        linea[strcspn(linea, "\r\n")] = 0;
        if (strlen(linea) == 0) continue;
        char *u = strtok(linea, ",");
        char *k = strtok(NULL, ",");
        char *nom = strtok(NULL, ",");
        if (!u || !k || !nom) continue;
        if (*n >= capacidad) {
            capacidad *= 2;
            *lista = realloc(*lista, sizeof(Usuario) * capacidad);
        }
        strncpy((*lista)[*n].usuario, u, sizeof((*lista)[*n].usuario)-1);
        strncpy((*lista)[*n].clave, k, sizeof((*lista)[*n].clave)-1);
        strncpy((*lista)[*n].nombre, nom, sizeof((*lista)[*n].nombre)-1);
        (*n)++;
    }
    fclose(f);
    return 1;
}

int autenticar(const char *ruta, Usuario *usuario_logueado) {
    Usuario *lista = NULL;
    int n = 0;
    if (!cargar_usuarios(ruta, &lista, &n)) {
        return 0;
    }
    char user_in[64], pass_in[64];
    int intentos = 0;
    while (intentos < 3) {
        printf("Usuario: ");
        if (scanf("%63s", user_in) != 1) { intentos++; continue; }
        printf("Clave: ");
        if (scanf("%63s", pass_in) != 1) { intentos++; continue; }

        for (int i = 0; i < n; ++i) {
            if (strcmp(user_in, lista[i].usuario) == 0 &&
                strcmp(pass_in, lista[i].clave) == 0) {
                // autenticado
                if (usuario_logueado) *usuario_logueado = lista[i];
                free(lista);
                return 1;
            }
        }
        intentos++;
        printf("Credenciales incorrectas. Intentos restantes: %d\n", 3 - intentos);
    }
    free(lista);
    return 0;
}
