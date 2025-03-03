
CC = gcc
CFLAGS = -Wall -g

# Objetivos
all: p3

# Reglas de compilación
p3: p3.o tipos.o
	$(CC) $(CFLAGS) -o p3 p3.o tipos.o

p3.o: p3.c tipos.h
	$(CC) $(CFLAGS) -c p3.c

tipos.o: tipos.c tipos.h macros.h
	$(CC) $(CFLAGS) -c tipos.c

clean:
	rm -f *.o p3
