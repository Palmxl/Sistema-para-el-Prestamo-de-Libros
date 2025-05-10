CC = gcc
CFLAGS = -Wall -pthread
OBJS = bd_libros.o

all: solicitante receptor

solicitante: solicitante.c $(OBJS)
	$(CC) $(CFLAGS) -o solicitante solicitante.c $(OBJS)

receptor: receptor.c $(OBJS)
	$(CC) $(CFLAGS) -o receptor receptor.c $(OBJS)

bd_libros.o: bd_libros.c SistemaDePrestamoDeLibros.h
	$(CC) $(CFLAGS) -c bd_libros.c

clean:
	rm -f *.o solicitante receptor
