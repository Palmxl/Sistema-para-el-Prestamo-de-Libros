/******************************************************
Integrantes: 
    - Juan Martín Sánchez
    - Juan Sebastián Tellez
    - Nathalia Ramírez
    - Juan Esteban Camargo
    - Santiago Martínez
Materia: Sistemas Operativos
Profesor: John Jairo Corredor
Proyecto: Sistema para el préstamo de libros
Descripción: 
    Contiene la implementación del proceso solicitante (PS), encargado de enviar solicitudes al proceso 
    receptor. Soporta dos modos de entrada: modo interactivo (menú) y desde un archivo de texto. 
    Cada solicitud (préstamo, renovación, devolución o salida) es enviada a través de un pipe nominal hacia el receptor.
******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_LINE 256

void enviar_peticion(int fd, char operacion, char *libro, int isbn) {
    char mensaje[MAX_LINE];
    snprintf(mensaje, MAX_LINE, "%c,%s,%d\n", operacion, libro, isbn);
    write(fd, mensaje, strlen(mensaje));
}

void menu_interactivo(int fd) {
    char op;
    char nombre[100];
    int isbn;

    while (1) {
        printf("Operación (P: Préstamo, R: Renovación, D: Devolución, Q: Salir): ");
        scanf(" %c", &op);

        if (op == 'Q') {
            enviar_peticion(fd, op, "Salir", 0);
            break;
        }

        memset(nombre, 0, sizeof(nombre)); // 🧼 Limpieza del buffer

        printf("Nombre del libro: ");
        scanf(" %[^\n]", nombre); // 📝 Acepta espacios

        printf("ISBN: ");
        scanf("%d", &isbn);

        enviar_peticion(fd, op, nombre, isbn);
    }
}

void leer_archivo_y_enviar(const char *archivo, int fd) {
    FILE *f = fopen(archivo, "r");
    if (!f) {
        perror("Archivo no encontrado");
        exit(1);
    }

    char linea[MAX_LINE];
    while (fgets(linea, sizeof(linea), f)) {
        write(fd, linea, strlen(linea));
    }

    fclose(f);
}

int main(int argc, char *argv[]) {
    char *archivo = NULL, *pipe_name = NULL;
    int opt;

    while ((opt = getopt(argc, argv, "i:p:")) != -1) {
        if (opt == 'i') archivo = optarg;
        else if (opt == 'p') pipe_name = optarg;
    }

    if (!pipe_name) {
        fprintf(stderr, "Uso: %s [-i archivo] -p pipe\n", argv[0]);
        exit(1);
    }

    int fd = open(pipe_name, O_WRONLY);
    if (fd == -1) {
        perror("No se puede abrir el pipe");
        exit(1);
    }

    if (archivo) leer_archivo_y_enviar(archivo, fd);
    else menu_interactivo(fd);

    close(fd);
    return 0;
}