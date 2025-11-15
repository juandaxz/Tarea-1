// productos.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "productos.h"

#define MAX_LINEA 256

int cargar_productos(const char *ruta, Producto **lista, int *n) {
    FILE *f = fopen(ruta, "r");
    if (!f) return 0;
    char linea[MAX_LINEA];
    int capacidad = 16;
    *lista = malloc(sizeof(Producto) * capacidad);
    *n = 0;
    while (fgets(linea, sizeof(linea), f)) {
        linea[strcspn(linea, "\r\n")] = 0;
        if (strlen(linea) == 0) continue;
        char *cod = strtok(linea, ",");
        char *nom = strtok(NULL, ",");
        char *cant = strtok(NULL, ",");
        char *costo = strtok(NULL, ",");
        char *precio = strtok(NULL, ",");
        if (!cod || !nom || !cant || !costo || !precio) continue;
        if (*n >= capacidad) {
            capacidad *= 2;
            *lista = realloc(*lista, sizeof(Producto) * capacidad);
        }
        strncpy((*lista)[*n].codigo, cod, sizeof((*lista)[*n].codigo)-1);
        strncpy((*lista)[*n].nombre, nom, sizeof((*lista)[*n].nombre)-1);
        (*lista)[*n].cantidad = atoi(cant);
        (*lista)[*n].costo = atof(costo);
        (*lista)[*n].precio = atof(precio);
        (*n)++;
    }
    fclose(f);
    return 1;
}

int guardar_productos(const char *ruta, Producto *lista, int n) {
    FILE *f = fopen(ruta, "w");
    if (!f) return 0;
    for (int i = 0; i < n; ++i) {
        fprintf(f, "%s,%s,%d,%.2f,%.2f\n",
                lista[i].codigo,
                lista[i].nombre,
                lista[i].cantidad,
                lista[i].costo,
                lista[i].precio);
    }
    fclose(f);
    return 1;
}

int buscar_producto(Producto *lista, int n, const char *codigo) {
    for (int i = 0; i < n; ++i) {
        if (strcmp(lista[i].codigo, codigo) == 0) return i;
    }
    return -1;
}

void mostrar_productos(Producto *lista, int n) {
    printf("=== Productos disponibles ===\n");
    printf("%-10s %-25s %-8s %-8s %-8s\n", "Código", "Nombre", "Cant", "Costo", "Precio");
    for (int i = 0; i < n; ++i) {
        printf("%-10s %-25s %-8d %-8.2f %-8.2f\n",
               lista[i].codigo, lista[i].nombre, lista[i].cantidad,
               lista[i].costo, lista[i].precio);
    }
}

int obtener_ultimo_num_factura(const char *ruta) {
    FILE *f = fopen(ruta, "r");
    if (!f) return 0; // si no existe, empezamos en 0
    char linea[MAX_LINEA];
    int num = 0;
    while (fgets(linea, sizeof(linea), f)) {
        // parsear el primer campo
        char *c = strtok(linea, ",");
        if (c) {
            int val = atoi(c);
            if (val > num) num = val;
        }
    }
    fclose(f);
    return num;
}
