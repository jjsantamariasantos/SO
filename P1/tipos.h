/*
 *Jesus Jose Santamaria Santos: j.j.ssantos
 */

#ifndef UNTITLED3_TIPOS_H
#define UNTITLED3_TIPOS_H

#include <stdio.h>

typedef struct historial
{
    char *orden;
    struct historial *siguiente;
} historial;

typedef struct informacionFichero
{
    int identificadorDelFichero;
    int modo;
    char *nombreDelFichero;
} informacionFichero;

int insertarEnElHistorial(char *orden, historial *historial1);
int insertarEnElFichero(int identificadorDelFichero, int modo, char *nombreDelFichero, informacionFichero *ficherosAbiertos);

void borrarDelHistorial(historial *historial1);
void borrarDelFichero(int identificadorDelFichero, informacionFichero *ficherosAbiertos);

#endif // UNTITLED3_TIPOS_H