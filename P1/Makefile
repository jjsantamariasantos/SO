CCC=-std=gnu99 -Wall -g -O0
P1=p1

all: comandos.o p1.c
	gcc $(CCC) -o $(P1) p1.c *.o

clean:
	-rm *.o $(P1)

comandos.o: auxiliar.o comandos.c comandos.h
	gcc $(CCC) -c comandos.c

auxiliar.o: tipos.o comandos.c comandos.h auxiliar.c auxiliar.h
	gcc $(CCC) -c auxiliar.c

tipos.o: macros.h tipos.c tipos.h
	gcc $(CCC) -c tipos.c
valgrind: all P1
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose./$(P1)