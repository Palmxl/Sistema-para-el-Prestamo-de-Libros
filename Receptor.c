/*
Membrete

*/

#include "SistemaDePrestamoDeLibros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>

#define MAX_LINE 256
#define N 10 // Tamaño del buffer

typedef struct {
    char tipo;  // 'D' o 'R'
    char libro[100];
    int isbn;
} Peticion;

Peticion buffer[N];
int in = 0, out = 0;

sem_t empty, full;
pthread_mutex_t mutex;

int verbose = 0;
int terminar = 0;

// Simula la escritura en la base de datos
void procesar_peticion(Peticion p) {
    printf("[DB] Procesando %c - %s (%d)\n", p.tipo, p.libro, p.isbn);
    // Aquí va la lógica para modificar el archivo de base de datos
    sleep(1);
}

void* hilo_auxiliar(void* arg) {
    while (!terminar) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        Peticion p = buffer[out];
        out = (out + 1) % N;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

        procesar_peticion(p);
    }
    return NULL;
}

void* hilo_consola(void* arg) {
    char comando;
    while (!terminar) {
        comando = getchar();
        if (comando == 's') {
            terminar = 1;
        } else if (comando == 'r') {
            printf("[REPORTE] (Simulado)\n");
            // Aquí iría la lógica para imprimir el estado de los libros
        }
    }
    return NULL;
}

void publicar_en_buffer(Peticion p) {
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    buffer[in] = p;
    in = (in + 1) % N;

    pthread_mutex_unlock(&mutex);
    sem_post(&full);
}

void procesar_linea(char* linea) {
    Peticion p;
    sscanf(linea, " %c, %[^,], %d", &p.tipo, p.libro, &p.isbn);

    if (verbose) printf("[RP] Recibido: %s", linea);

    if (p.tipo == 'D' || p.tipo == 'R') {
        publicar_en_buffer(p);
        printf("[RP] Aceptado %c para %s\n", p.tipo, p.libro);
    } else if (p.tipo == 'P') {
        // Préstamo: acceso directo a BD
        printf("[RP] Procesando préstamo de %s\n", p.libro);
        // Lógica de disponibilidad en base de datos...
    } else if (p.tipo == 'Q') {
        printf("[RP] PS indica salida\n");
    }
}

int main(int argc, char *argv[]) {
    char *pipe_name = NULL;
    int opt;
    int fd;
    char linea[MAX_LINE]
    char *archivoBD = NULL;
    char *archivoSalida = NULL;

    while ((opt = getopt(argc, argv, "p:f:s:v")) != -1) {
        switch (opt) {
            case 'p': pipe_name = optarg; break;
            case 'f': archivoBD = optarg; break;
            case 's': archivoSalida = optarg; break;
            case 'v': verbose = 1; break;
        }
    }

    if (!pipe_name || !archivoBD) {
        fprintf(stderr, "Uso: %s -p pipe -f archivoBD [-v] [-s archivoSalida]\n", argv[0]);
        exit(1);
    }

    // Cargar la base de datos desde archivo
    if (cargar_base_datos(archivoBD) == -1) {
        perror("No se pudo cargar la base de datos");
        exit(1);
    }

    // Abrir pipe para lectura
    fd = open(pipe_name, O_RDONLY);
    if (fd == -1) {
        perror("Error abriendo pipe");
        exit(1);
    }

    // Inicializar sincronización
    sem_init(&empty, 0, N);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    // Crear hilos
    pthread_t aux_thread, consola_thread;
    pthread_create(&aux_thread, NULL, hilo_auxiliar, NULL);
    pthread_create(&consola_thread, NULL, hilo_consola, NULL);

    // Leer del pipe y procesar línea por línea
    while (!terminar && read(fd, linea, sizeof(linea)) > 0) {
        char *line = strtok(linea, "\n");
        while (line != NULL) {
            procesar_linea(line);
            line = strtok(NULL, "\n");
        }
        memset(linea, 0, sizeof(linea));
    }

    // Esperar fin de hilos
    pthread_join(aux_thread, NULL);
    pthread_join(consola_thread, NULL);

    // Guardar base de datos si se especificó
    if (archivoSalida) {
        guardar_base_datos(archivoSalida);
        printf("[RP] Base de datos guardada en %s\n", archivoSalida);
    }

    // Limpiar
    close(fd);
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    printf("[RP] Finalizado.\n");
    return 0;
}
