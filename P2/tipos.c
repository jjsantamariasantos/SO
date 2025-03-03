/*
 *Jesus Jose Santamaria Santos: j.j.ssantos
 */

#include "tipos.h"

#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

int insertarEnElHistorial(char *orden, historial *historial1)
{
    int temp;

    for (temp = 0; temp < HISTORIALMAX && historial1[temp].orden != NULL; temp++)
        ;
    if (temp < HISTORIALMAX && historial1[temp].orden == NULL)
    {
        if (temp > 1)
        {
        }
        historial1[temp].orden = malloc(strlen(orden) + 1);
        strcpy(historial1[temp].orden, orden);
        historial1[temp].siguiente = NULL;
        if (temp > 1)
        {
            historial1[temp - 1].siguiente = &historial1[temp];
        }
        return 0;
    }
    free(historial1[temp].orden);
    historial1[temp].orden = NULL;
    return -1;
}

int insertarEnElFichero(int identificadorDelFichero, int modo, char *nombreDelFichero, informacionFichero *ficherosAbiertos)
{
    int temp;

    for (temp = 0; temp < HISTORIALMAX && ficherosAbiertos[temp].nombreDelFichero != NULL; temp++)
        ;
    if (temp < HISTORIALMAX && ficherosAbiertos[temp].nombreDelFichero == NULL)
    {
        ficherosAbiertos[temp].identificadorDelFichero = identificadorDelFichero;
        ficherosAbiertos[temp].modo = modo;
        ficherosAbiertos[temp].nombreDelFichero = malloc(strlen(nombreDelFichero) + 1);
        if (ficherosAbiertos[temp].nombreDelFichero == NULL)
        {
            printf("Error: No se pudo asignar memoria para el nombre del fichero\n");
            free(ficherosAbiertos[temp].nombreDelFichero);
            ficherosAbiertos[temp].nombreDelFichero = NULL;
            return -1;
        }
        strcpy(ficherosAbiertos[temp].nombreDelFichero, nombreDelFichero);
        return 0;
    }
    printf("Error: No se puede insertar en la lista\n");
    return -1;
}
int insertarEnMemoria(informacionMemoria **lista, void *direccion, size_t tamano, const char *tipoDeAsignacion, key_t clave, int descriptorFichero, const char *nombreFichero)
{
    informacionMemoria *nuevoNodo = malloc(sizeof(informacionMemoria));
    if (nuevoNodo == NULL)
    {
        perror("Error al asignar memoria para el nuevo nodo");
        return -1;
    }
    nuevoNodo->direccion = direccion;
    nuevoNodo->tamano = tamano;
    nuevoNodo->tiempo = time(NULL);
    strcpy(nuevoNodo->tipoDeAsignacion, tipoDeAsignacion);
    if (strcmp(tipoDeAsignacion, "shared") == 0)
    {
        nuevoNodo->info.clave = clave;
    }
    else if (strcmp(tipoDeAsignacion, "mmap") == 0)
    {
        nuevoNodo->info.mmapInfo.nombreFichero = strdup(nombreFichero);
        if (nuevoNodo->info.mmapInfo.nombreFichero == NULL)
        {
            perror("Error al asignar memoria para el nombre del archivo");
            free(nuevoNodo->info.mmapInfo.nombreFichero);
            return -1;
        }
        nuevoNodo->info.mmapInfo.descriptorFichero = descriptorFichero;
    }
    nuevoNodo->siguiente = NULL;
    if (*lista == NULL)
    {
        *lista = nuevoNodo;
    }
    else
    {
        informacionMemoria *temp = *lista;
        while (temp->siguiente != NULL)
        {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevoNodo;
    }

    return 0;
}

void borrarDelHistorial(historial *historial1)
{
    int temp;

    for (temp = 0; temp < HISTORIALMAX && historial1[temp].orden != NULL; temp++)
    {
        free(historial1[temp].orden);
        historial1[temp].orden = NULL;
    }
}

void borrarDelFichero(int identificadorDelFichero, informacionFichero *ficherosAbiertos)
{
    int temp1, temp2;

    for (temp1 = 0; temp1 < HISTORIALMAX && ficherosAbiertos[temp1].identificadorDelFichero != identificadorDelFichero; temp1++)
        ;
    if (temp1 < HISTORIALMAX)
    {
        for (temp2 = temp1; temp2 < HISTORIALMAX; temp2++)
        {
            ficherosAbiertos[temp2] = ficherosAbiertos[temp2 + 1];
        }
    }
}
void borrarDeMemoria(informacionMemoria **lista, void *direccion)
{
    if (*lista == NULL)
    {
        return;
    }

    informacionMemoria *temp = *lista;
    informacionMemoria *anterior = NULL;

    while (temp != NULL && temp->direccion != direccion)
    {
        anterior = temp;
        temp = temp->siguiente;
    }

    if (temp == NULL)
    {
        return;
    }
    else if (anterior == NULL)
    {
        *lista = temp->siguiente;
    }
    else
    {
        anterior->siguiente = temp->siguiente;
    }
    if (strcmp(temp->tipoDeAsignacion, "mmap") == 0)
    {
        ;
        if (munmap(temp->direccion, temp->tamano) == -1)
        {
            perror("Error al desmapear la memoria mapeada");
        }
    }
}
void eliminarMemoria(informacionMemoria **lista)
{
    informacionMemoria *actual = *lista;
    informacionMemoria *siguiente = NULL;

    while (actual != NULL)
    {
        siguiente = actual->siguiente;
        if (strcmp(actual->tipoDeAsignacion, "mmap") == 0)
        {
            free(actual->info.mmapInfo.nombreFichero);
        }
        actual = siguiente;
    }
    *lista = NULL;
}
void imprimirLista(informacionMemoria *lista, char *asignacion)
{
    informacionMemoria *actual = lista;

    printf("Lista de Información de Memoria:\n");
    printf("******* Lista de bloques asignados %s para el proceso %d\n", asignacion, getpid());
    while (actual != NULL && actual->direccion != NULL)
    {
        struct tm *tiempoInfo = localtime(&(actual->tiempo));
        if (strcmp(actual->tipoDeAsignacion, asignacion) == 0 || strcmp(asignacion, "todos") == 0)
        {
            printf("Dirección: %p, Tamaño: %zu, Tipo: %s, Tiempo: %02d:%02d\n", actual->direccion, actual->tamano, actual->tipoDeAsignacion, tiempoInfo->tm_hour, tiempoInfo->tm_min);
            if (strcmp(actual->tipoDeAsignacion, "shared") == 0)
            {
                printf("Clave: %d\n", actual->info.clave);
            }
            else if (strcmp(actual->tipoDeAsignacion, "mmap") == 0)
            {
                printf("Nombre del Fichero: %s, Descriptor de Archivo: %d\n", actual->info.mmapInfo.nombreFichero, actual->info.mmapInfo.descriptorFichero);
            }
        }
        actual = actual->siguiente;
    }
}