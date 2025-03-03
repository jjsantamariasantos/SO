/*
 *Jesus Jose Santamaria Santos: j.j.ssantos
 */

#ifndef UNTITLED3_TIPOS_H
#define UNTITLED3_TIPOS_H

#include "macros.h"

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

typedef struct informacionProcesos
{
    pid_t pid;
    int prioridad;
    char *senal;
    char linea[20];
    char estado[20];
    time_t tiempo;
} informacionProcesos;

struct nodo
{
    informacionProcesos infoProcesos;
    struct nodo *siguiente;
};

typedef struct nodo *pProcesos;

typedef struct nodo *lProcesos;

int insertarEnElHistorial(char *orden, historial *historial1);
int insertarEnElFichero(int identificadorDelFichero, int modo, char *nombreDelFichero, informacionFichero *ficherosAbiertos);
int insertarEnMemoria(informacionMemoria **lista, void *direccion, size_t tamano, const char *tipoDeAsignacion, key_t clave, int descriptorFichero, const char *nombreFichero);

void borrarDelHistorial(historial *historial1);
void borrarDelFichero(int identificadorDelFichero, informacionFichero *ficherosAbiertos);
void borrarDeMemoria(informacionMemoria **lista, void *direccion);
void eliminarMemoria(informacionMemoria **lista);
void imprimirLista(informacionMemoria *lista, char *asignacion);

int senal(char *senal);
char *nombreSenal(int senal);
lProcesos createlProcesos();
int emptylist(lProcesos lista);
void insertProceso(informacionProcesos dp, pProcesos p);
pProcesos findProceso(lProcesos lista, pid_t pid);
void deleteProceso(lProcesos lista, pProcesos p);
pProcesos firstProceso(lProcesos lista);
pProcesos nextProceso(pProcesos p);
int endlProcesos(pProcesos p);
informacionProcesos elemento(pProcesos p);
void modElemento(pProcesos p);
void eliminarListaProcesos(lProcesos lista);

#endif // UNTITLED3_TIPOS_H