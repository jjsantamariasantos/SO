/*
 *Jesus Jose Santamaria Santos: j.j.ssantos
 */

#include "macros.h"
#include "tipos.h"
#include "auxiliar.h"

#include <fcntl.h>

int main()
{
    historial historial1[HISTORIALMAX];
    char entrada[ENTRADAMAX];
    char *argumentos[ENTRADAMAX];
    informacionFichero ficherosAbiertos[HISTORIALMAX];
    int identificadorDelFichero1 = 0, identificadorDelFichero2 = 1,
        identificadorDelFichero3 = 2, modo1 = fcntl(identificadorDelFichero1, F_GETFL),
        modo2 = fcntl(identificadorDelFichero2, F_GETFL),
        modo3 = fcntl(identificadorDelFichero3, F_GETFL);
    char *nombreDelFichero1 = "Entrada estandar",
         *nombreDelFichero2 = "Salida estandar",
         *nombreDelFichero3 = "Error estandar";

    insertarEnElFichero(identificadorDelFichero1, modo1, nombreDelFichero1, ficherosAbiertos);
    insertarEnElFichero(identificadorDelFichero2, modo2, nombreDelFichero2, ficherosAbiertos);
    insertarEnElFichero(identificadorDelFichero3, modo3, nombreDelFichero3, ficherosAbiertos);
    while (1)
    {
        printf("%s%2s", PROMPT, "");
        escanearEntrada(entrada);
        ejecutarEntrada(entrada, argumentos, historial1, ficherosAbiertos);
    }
    return 0;
}