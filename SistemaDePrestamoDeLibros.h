#ifndef SistemaDePrestamoDeLibros_H
#define SistemaDePrestamoDeLibros_H

int cargar_base_datos(const char *archivo);
int guardar_base_datos(const char *archivo);
int prestar_libro(int isbn);
int devolver_libro(int isbn);
int renovar_libro(int isbn);

#endif