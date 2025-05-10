CC = gcc
CFLAGS = -Wall -pthread
OBJS = bd_libros.o

all: solicitante receptor

solicitante: Solicitante.c $(OBJS)
	$(CC) $(CFLAGS) -o solicitante Solicitante.c $(OBJS)

receptor: Receptor.c $(OBJS)
	$(CC) $(CFLAGS) -o receptor Receptor.c $(OBJS)

bd_libros.o: SistemaDePrestamoDeLibros.c SistemaDePrestamoDeLibros.h
	$(CC) $(CFLAGS) -c SistemaDePrestamoDeLibros.c

clean:
	rm -f *.o solicitante receptor
