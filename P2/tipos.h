/*
 *Jesus Jose Santamaria Santos: j.j.ssantos
 */

#ifndef UNTITLED3_TIPOS_H
#define UNTITLED3_TIPOS_H

#include "macros.h"

#include <stdio.h>
#include <time.h>
#include <sys/types.h>

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

typedef struct informacionMemoria
{
    void *direccion;
    size_t tamano;
    time_t tiempo;
    char tipoDeAsignacion[20];
    union
    {
        key_t clave;
        struct
        {
            char *nombreFichero;
            int descriptorFichero;
        } mmapInfo;
    } info;
    struct informacionMemoria *siguiente;
} informacionMemoria;

int insertarEnElHistorial(char *orden, historial *historial1);
int insertarEnElFichero(int identificadorDelFichero, int modo, char *nombreDelFichero, informacionFichero *ficherosAbiertos);
int insertarEnMemoria(informacionMemoria **lista, void *direccion, size_t tamano, const char *tipoDeAsignacion, key_t clave, int descriptorFichero, const char *nombreFichero);

void borrarDelHistorial(historial *historial1);
void borrarDelFichero(int identificadorDelFichero, informacionFichero *ficherosAbiertos);
void borrarDeMemoria(informacionMemoria **lista, void *direccion);
void eliminarMemoria(informacionMemoria **lista);
void imprimirLista(informacionMemoria *lista, char *asignacion);

#endif // UNTITLED3_TIPOS_H