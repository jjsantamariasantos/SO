/*
 *Jesus Jose Santamaria Santos: j.j.ssantos
 */

#include "tipos.h"
#include "macros.h"

#include <stdlib.h>
#include <string.h>

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

void borrarDelHistorial(historial *historial1)
{
    int temp;

    for (temp = 0; temp < HISTORIALMAX && historial1[temp].orden != NULL; temp++)
    {
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