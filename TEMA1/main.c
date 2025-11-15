// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "usuarios.h"
#include "productos.h"

#define RUTA_PRODUCTOS "producto.txt"
#define RUTA_VENTAS "ventas.txt"
#define RUTA_VENDEDORES "vendedores.txt"

#define MAX_ITEMS 128

typedef struct {
    char codigo[64];
    char nombre[128];
    int cantidad;
    double costo;
    double precio;
} DetalleVenta;

int main(void) {
    Usuario usuario;
    printf("=== Sistema de Inventario y Ventas ===\n");
    if (!autenticar(RUTA_VENDEDORES, &usuario)) {
        printf("Autenticación fallida. Cerrando sistema.\n");
        return 1;
    }
    printf("Bienvenido: %s\n", usuario.nombre);

    Producto *productos = NULL;
    int nprod = 0;
    if (!cargar_productos(RUTA_PRODUCTOS, &productos, &nprod)) {
        printf("No se pudo cargar %s\n", RUTA_PRODUCTOS);
        return 1;
    }

    // Listar productos
    mostrar_productos(productos, nprod);

    // Preparar venta
    DetalleVenta carrito[MAX_ITEMS];
    int carrito_n = 0;
    char opcion[8];

    while (1) {
        printf("\n¿Desea registrar un producto para la venta? (si/no): ");
        if (scanf("%7s", opcion) != 1) break;
        if (strcmp(opcion, "no") == 0) break;
        if (strcmp(opcion, "si") != 0) continue;

        char codigo[64];
        int cantidad;
        printf("Ingrese código del producto: ");
        if (scanf("%63s", codigo) != 1) continue;
        int idx = buscar_producto(productos, nprod, codigo);
        if (idx == -1) {
            printf("Producto no encontrado.\n");
            continue;
        }
        printf("Ingrese cantidad a vender (disponible %d): ", productos[idx].cantidad);
        if (scanf("%d", &cantidad) != 1) continue;
        if (cantidad <= 0) {
            printf("Cantidad inválida.\n");
            continue;
        }
        if (cantidad > productos[idx].cantidad) {
            printf("No hay suficiente inventario. Disponible: %d\n", productos[idx].cantidad);
            continue;
        }

        // Si ya existe en carrito, sumar cantidades
        int encontrado = -1;
        for (int i = 0; i < carrito_n; ++i) {
            if (strcmp(carrito[i].codigo, codigo) == 0) { encontrado = i; break; }
        }
        if (encontrado != -1) {
            if (carrito[encontrado].cantidad + cantidad > productos[idx].cantidad) {
                printf("Supera inventario al sumar al carrito.\n");
                continue;
            }
            carrito[encontrado].cantidad += cantidad;
        } else {
            if (carrito_n >= MAX_ITEMS) {
                printf("Carrito lleno.\n");
                continue;
            }
            strncpy(carrito[carrito_n].codigo, productos[idx].codigo, sizeof(carrito[carrito_n].codigo)-1);
            strncpy(carrito[carrito_n].nombre, productos[idx].nombre, sizeof(carrito[carrito_n].nombre)-1);
            carrito[carrito_n].cantidad = cantidad;
            carrito[carrito_n].costo = productos[idx].costo;
            carrito[carrito_n].precio = productos[idx].precio;
            carrito_n++;
        }
        printf("Producto agregado al carrito.\n");
    }

    if (carrito_n == 0) {
        printf("No se registraron productos. Saliendo.\n");
        free(productos);
        return 0;
    }

    // Obtener fecha actual
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char fecha[16];
    snprintf(fecha, sizeof(fecha), "%04d-%02d-%02d", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday);

    // Obtener ultimo numero de factura
    int ultimo = obtener_ultimo_num_factura(RUTA_VENTAS);
    int num_factura = ultimo + 1;

    // Mostrar encabezado y detalle
    printf("\n=====================================\n");
    printf("Factura Nro: %d\n", num_factura);
    printf("Fecha: %s\n", fecha);
    printf("-------------------------------------\n");
    printf("%-10s %-20s %-8s %-10s %-10s\n", "Codigo", "Nombre", "Cant", "PrecioU", "Total");
    double total_factura = 0.0;
    for (int i = 0; i < carrito_n; ++i) {
        double tot = carrito[i].cantidad * carrito[i].precio;
        total_factura += tot;
        printf("%-10s %-20s %-8d %-10.2f %-10.2f\n",
               carrito[i].codigo, carrito[i].nombre, carrito[i].cantidad,
               carrito[i].precio, tot);
    }
    printf("-------------------------------------\n");
    printf("Total factura: %.2f\n", total_factura);

    printf("\nConfirma la venta? (si/no): ");
    if (scanf("%7s", opcion) != 1) { free(productos); return 1; }
    if (strcmp(opcion, "si") != 0) {
        printf("Venta cancelada.\n");
        free(productos);
        return 0;
    }

    // Actualizar inventario
    for (int i = 0; i < carrito_n; ++i) {
        int idx = buscar_producto(productos, nprod, carrito[i].codigo);
        if (idx >= 0) {
            productos[idx].cantidad -= carrito[i].cantidad;
        }
    }
    // Guardar producto.txt actualizado
    if (!guardar_productos(RUTA_PRODUCTOS, productos, nprod)) {
        printf("Error guardando inventario.\n");
        free(productos);
        return 1;
    }

    // Registrar ventas (append) — una línea por producto
    FILE *fv = fopen(RUTA_VENTAS, "a");
    if (!fv) {
        printf("No se pudo abrir %s para registro de ventas.\n", RUTA_VENTAS);
        free(productos);
        return 1;
    }
    for (int i = 0; i < carrito_n; ++i) {
        fprintf(fv, "%d,%s,%s,%d,%.2f,%.2f,%s\n",
                num_factura,
                carrito[i].codigo,
                carrito[i].nombre,
                carrito[i].cantidad,
                carrito[i].costo,
                carrito[i].precio,
                fecha);
    }
    fclose(fv);

    printf("Venta registrada correctamente con factura Nro %d\n", num_factura);
    free(productos);
    return 0;
}
