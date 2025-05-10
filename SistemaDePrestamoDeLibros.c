/*
Membrete

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "SistemaDePrestamoDeLibros.h"


#define MAX_LIBROS 100
#define MAX_EJEMPLARES 10

typedef struct {
    int numero;
    char estado;  // 'P' o 'D'
    char fecha[11];
} Ejemplar;

typedef struct {
    char nombre[100];
    int isbn;
    int cantidad;
    Ejemplar ejemplares[MAX_EJEMPLARES];
} Libro;

Libro biblioteca[MAX_LIBROS];
int total_libros = 0;

char* fecha_actual() {
    static char fecha[11];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(fecha, sizeof(fecha), "%d-%d-%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    return fecha;
}

int cargar_base_datos(const char *archivo) {
    FILE *f = fopen(archivo, "r");
    if (!f) return -1;

    total_libros = 0;
    while (!feof(f)) {
        Libro *libro = &biblioteca[total_libros];
        fscanf(f, " %[^\n], %d, %d\n", libro->nombre, &libro->isbn, &libro->cantidad);

        for (int i = 0; i < libro->cantidad; i++) {
            fscanf(f, "%d, %c, %s\n", &libro->ejemplares[i].numero,
                                      &libro->ejemplares[i].estado,
                                      libro->ejemplares[i].fecha);
        }

        total_libros++;
    }
    fclose(f);
    return 0;
}

int guardar_base_datos(const char *archivo) {
    FILE *f = fopen(archivo, "w");
    if (!f) return -1;

    for (int i = 0; i < total_libros; i++) {
        Libro *libro = &biblioteca[i];
        fprintf(f, "%s, %d, %d\n", libro->nombre, libro->isbn, libro->cantidad);
        for (int j = 0; j < libro->cantidad; j++) {
            fprintf(f, "%d, %c, %s\n", libro->ejemplares[j].numero,
                                       libro->ejemplares[j].estado,
                                       libro->ejemplares[j].fecha);
        }
    }

    fclose(f);
    return 0;
}

Libro* buscar_libro(int isbn) {
    for (int i = 0; i < total_libros; i++) {
        if (biblioteca[i].isbn == isbn) return &biblioteca[i];
    }
    return NULL;
}

int prestar_libro(int isbn) {
    Libro *libro = buscar_libro(isbn);
    if (!libro) return -1;

    for (int i = 0; i < libro->cantidad; i++) {
        if (libro->ejemplares[i].estado == 'D') {
            libro->ejemplares[i].estado = 'P';
            strcpy(libro->ejemplares[i].fecha, fecha_actual());
            return 1;
        }
    }
    return 0;
}

int devolver_libro(int isbn) {
    Libro *libro = buscar_libro(isbn);
    if (!libro) return -1;

    for (int i = 0; i < libro->cantidad; i++) {
        if (libro->ejemplares[i].estado == 'P') {
            libro->ejemplares[i].estado = 'D';
            strcpy(libro->ejemplares[i].fecha, fecha_actual());
            return 1;
        }
    }
    return 0;
}

int renovar_libro(int isbn) {
    Libro *libro = buscar_libro(isbn);
    if (!libro) return -1;

    for (int i = 0; i < libro->cantidad; i++) {
        if (libro->ejemplares[i].estado == 'P') {
            strcpy(libro->ejemplares[i].fecha, fecha_actual());  // Se sobreescribe con fecha actual
            return 1;
        }
    }
    return 0;
}