// productos.h
#ifndef PRODUCTOS_H
#define PRODUCTOS_H

typedef struct {
    char codigo[64];
    char nombre[128];
    int cantidad;
    double costo;
    double precio;
} Producto;

typedef struct {
    char codigo[64];
    int cantidad_vendida;
} ItemVenta;

int cargar_productos(const char *ruta, Producto **lista, int *n);
int guardar_productos(const char *ruta, Producto *lista, int n);
int buscar_producto(Producto *lista, int n, const char *codigo);
void mostrar_productos(Producto *lista, int n);
int obtener_ultimo_num_factura(const char *ruta);

#endif
