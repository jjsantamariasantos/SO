/*
 *Jesus Jose Santamaria Santos: j.j.ssantos
 */

#ifndef UNTITLED3_AUXILIAR_H
#define UNTITLED3_AUXILIAR_H

#include "tipos.h"

int trocearCadena(char *cadena, char *trozos[]);
int listaOrdenes(char *argumentos[]);

void escanearEntrada(char *entrada);
void ejecutarEntrada(char *entrada, char *argumentos[], historial *historial1, informacionFichero *ficherosAbiertos);
void ejecutarOrden(char *entrada, char *argumentos[], historial *historial1, informacionFichero *ficherosAbiertos);
#endif // UNTITLED3_AUXILIAR_H