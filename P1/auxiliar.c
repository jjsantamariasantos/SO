/*
 *Jesus Jose Santamaria Santos: j.j.ssantos
 */

#include "auxiliar.h"
#include "comandos.h"
#include "macros.h"

#include <stdlib.h>
#include <string.h>

int trocearCadena(char *cadena, char *trozos[])
{
    int temp = 1;

    if ((trozos[0] = strtok(cadena, " \n\t")) == NULL)
    {
        return 0;
    }
    while ((trozos[temp] = strtok(NULL, " \n\t")) != NULL)
    {
        temp++;
    }

    return temp;
}

int listaOrdenes(char *argumentos[])
{
    if (strcmp(argumentos[0], "authors") == 0)
    {
        return 0;
    }
    else if (strcmp(argumentos[0], "pid") == 0)
    {
        return 1;
    }
    else if (strcmp(argumentos[0], "chdir") == 0)
    {
        return 2;
    }
    else if (strcmp(argumentos[0], "date") == 0)
    {
        return 3;
    }
    else if (strcmp(argumentos[0], "time") == 0)
    {
        return 4;
    }
    else if (strcmp(argumentos[0], "hist") == 0)
    {
        return 5;
    }
    else if (strcmp(argumentos[0], "command") == 0)
    {
        return 6;
    }
    else if (strcmp(argumentos[0], "open") == 0)
    {
        return 7;
    }
    else if (strcmp(argumentos[0], "close") == 0)
    {
        return 8;
    }
    else if (strcmp(argumentos[0], "dup") == 0)
    {
        return 9;
    }
    else if (strcmp(argumentos[0], "listopen") == 0)
    {
        return 10;
    }
    else if (strcmp(argumentos[0], "infosys") == 0)
    {
        return 11;
    }
    else if (strcmp(argumentos[0], "help") == 0)
    {
        return 12;
    }
    else if (strcmp(argumentos[0], "quit") == 0 || strcmp(argumentos[0], "exit") == 0 ||
             strcmp(argumentos[0], "bye") == 0)
    {
        return 13;
    }
    else if (strcmp(argumentos[0], "create") == 0)
    {
        return 14;
    }
    else if (strcmp(argumentos[0], "stat") == 0)
    {
        return 15;
    }
    else if (strcmp(argumentos[0], "list") == 0)
    {
        return 16;
    }
    else if (strcmp(argumentos[0], "delete") == 0)
    {
        return 17;
    }
    else if (strcmp(argumentos[0], "deltree") == 0)
    {
        return 18;
    }
    return -1;
}

void escanearEntrada(char *entrada)
{
    fgets(entrada, ENTRADAMAX, stdin);
}

void ejecutarEntrada(char *entrada, char *argumentos[], historial *historial1, informacionFichero *ficherosAbiertos)
{
    char *temp;
    int posTemp;

    temp = malloc(strlen(entrada) + 1);
    if (strlen(entrada) == 2)
    {
        free(temp);
        temp = NULL;
        return;
    }
    else
    {
        strcpy(temp, entrada);
        if (strstr(temp, "command") != NULL && strstr(temp, "help") == NULL && strstr(temp, "-?") == NULL)
        {
        }
        else
        {
            posTemp = insertarEnElHistorial(temp, historial1);
            if (posTemp == -1)
            {
                printf("Error: No se ha podido insertar en el historial");
                free(temp);
                temp = NULL;
                return;
            }
        }
        free(temp);
        temp = NULL;
        ejecutarOrden(entrada, argumentos, historial1, ficherosAbiertos);
    }
}

void ejecutarOrden(char *entrada, char *argumentos[], historial *historial1,
                   informacionFichero *ficherosAbiertos)
{
    int temp1 = 0, temp2 = 0, temp3 = 0, temp4 = 0, temp5 = 0, temp6 = 0, temp7 = 0,
        caso = 0, suma = 0, bucleTemp, numeroArgumentos = trocearCadena(entrada, argumentos);
    bool transformarTemp = false;

    if (argumentos[0] == NULL)
    {
        return;
    }
    switch (listaOrdenes(&argumentos[0]))
    {
    case 0:
        if (numeroArgumentos == 1)
        {
            cmdAuthors(0);
        }
        else if (strcmp(argumentos[1], "-?") == 0)
        {
            cmdHelp(argumentos[0]);
        }
        else
        {
            cmdAuthors(argumentos[1]);
        }
        break;
    case 1:
        if (numeroArgumentos == 1)
        {
            cmdPid(0);
        }
        else if (strcmp(argumentos[1], "-?") == 0)
        {
            cmdHelp(argumentos[0]);
        }
        else
        {
            cmdPid(argumentos[1]);
        }
        break;
    case 2:
        if (numeroArgumentos == 1)
        {
            cmdChdir(0);
        }
        else if (strcmp(argumentos[1], "-?") == 0)
        {
            cmdHelp(argumentos[0]);
        }
        else
        {
            cmdChdir(argumentos[1]);
        }
        break;
    case 3:
        if (numeroArgumentos > 1 && strcmp(argumentos[1], "-?") == 0)
        {
            cmdHelp(argumentos[0]);
        }
        else
        {
            cmdDate();
        }
        break;
    case 4:
        if (numeroArgumentos > 1 && strcmp(argumentos[1], "-?") == 0)
        {
            cmdHelp(argumentos[0]);
        }
        else
        {
            cmdTime();
        }
        break;
    case 5:
        if (numeroArgumentos == 1)
        {
            cmdHist(0, historial1);
        }
        else if (strcmp(argumentos[1], "-?") == 0)
        {
            cmdHelp(argumentos[0]);
        }
        else
        {
            cmdHist(argumentos[1], historial1);
        }
        break;
    case 6:
        if (numeroArgumentos == 1)
        {
            cmdHist(0, historial1);
        }
        else if (strcmp(argumentos[1], "-?") == 0)
        {
            cmdHelp(argumentos[0]);
        }
        else
        {
            cmdCommand(argumentos[1], historial1, ficherosAbiertos);
        }
        break;
    case 7:
        if (numeroArgumentos > 1 && strcmp(argumentos[1], "-?") == 0)
        {
            cmdHelp(argumentos[0]);
        }
        else
        {
            cmdOpen(argumentos, ficherosAbiertos);
        }
        break;
    case 8:
        if (numeroArgumentos > 1 && strcmp(argumentos[1], "-?") == 0)
        {
            cmdHelp(argumentos[0]);
        }
        else
        {
            cmdClose(argumentos, ficherosAbiertos);
        }
        break;
    case 9:
        if (numeroArgumentos > 1 && strcmp(argumentos[1], "-?") == 0)
        {
            cmdHelp(argumentos[0]);
        }
        else
        {
            cmdDup(argumentos, ficherosAbiertos);
        }
        break;
    case 10:
        if (numeroArgumentos > 1 && strcmp(argumentos[1], "-?") == 0)
        {
            cmdHelp(argumentos[0]);
        }
        else
        {
            cmdListopen(ficherosAbiertos);
        }
        break;
    case 11:
        if (numeroArgumentos > 1 && strcmp(argumentos[1], "-?") == 0)
        {
            cmdHelp(argumentos[0]);
        }
        else
        {
            cmdInfosys();
        }
        break;
    case 12:
        if (numeroArgumentos == 1 || (strcmp(argumentos[1], "-?") == 0))
        {
            cmdHelp(0);
        }
        else
        {
            cmdHelp(argumentos[1]);
        }
        break;
    case 13:
        if (numeroArgumentos > 1 && strcmp(argumentos[1], "-?") == 0)
        {
            cmdHelp(argumentos[0]);
        }
        else
        {
            cmdSalida();
        }
        break;
    case 14:
        if (numeroArgumentos == 1)
        {
            cmdChdir(0);
        }
        else if (strcmp(argumentos[1], "-?") == 0)
        {
            cmdHelp(argumentos[0]);
        }
        else if (strcmp(argumentos[1], "-f") == 0)
        {
            cmdCreate(1, argumentos[2]);
        }
        else
        {
            cmdCreate(0, argumentos[1]);
        }
        break;
    case 15:
        if (numeroArgumentos == 1)
        {
            cmdChdir(0);
        }
        else if (numeroArgumentos > 1)
        {
            if (strcmp(argumentos[1], "-?") == 0)
            {
                cmdHelp(argumentos[0]);
            }
            else
            {
                for (temp1 = 1; temp1 < numeroArgumentos; temp1++)
                {
                    if (strcmp(argumentos[temp1], "-long") == 0)
                    {
                        temp2 = 1;
                    }
                    else if (strcmp(argumentos[temp1], "-link") == 0)
                    {
                        temp3 = 1;
                    }
                    else if (strcmp(argumentos[temp1], "-acc") == 0)
                    {
                        temp4 = 1;
                    }
                    else
                    {
                        break;
                    }
                }
                suma = temp2 + temp3 + temp4;
                if (temp1 < numeroArgumentos)
                {
                    if (suma == 0)
                    {
                        caso = 0;
                    }
                    else if (suma == 1)
                    {
                        if (temp2 == 1)
                        {
                            caso = 1;
                        }
                        else
                        {
                            caso = 0;
                        }
                    }
                    else if (suma == 2)
                    {
                        if (temp2 == 1)
                        {
                            if (temp3 == 1)
                            {
                                caso = 2;
                            }
                            else
                            {
                                caso = 3;
                            }
                        }
                        else
                        {
                            caso = 0;
                        }
                    }
                    else if (suma == 3)
                    {
                        caso = 4;
                    }
                    for (bucleTemp = temp1; bucleTemp < numeroArgumentos; bucleTemp++)
                    {
                        cmdStat(caso, argumentos[bucleTemp], transformarTemp);
                    }
                }
                else
                {
                    cmdChdir(0);
                }
            }
        }
        break;
    case 16:
        if (numeroArgumentos == 1)
        {
            cmdChdir(0);
        }
        else if (numeroArgumentos > 1)
        {
            if (strcmp(argumentos[1], "-?") == 0)
            {
                cmdHelp(argumentos[0]);
            }
            else
            {
                for (temp1 = 1; temp1 < numeroArgumentos; temp1++)
                {
                    if (strcmp(argumentos[temp1], "-recb") == 0)
                    {
                        temp2 = 1;
                    }
                    else if (strcmp(argumentos[temp1], "-reca") == 0)
                    {
                        temp3 = 1;
                    }
                    else if (strcmp(argumentos[temp1], "-hid") == 0)
                    {
                        temp4 = 1;
                    }
                    else if (strcmp(argumentos[temp1], "-long") == 0)
                    {
                        temp5 = 1;
                    }
                    else if (strcmp(argumentos[temp1], "-link") == 0)
                    {
                        temp6 = 1;
                    }
                    else if (strcmp(argumentos[temp1], "-acc") == 0)
                    {
                        temp7 = 1;
                    }
                    else
                    {
                        break;
                    }
                }
                if (temp1 < numeroArgumentos)
                {
                    for (bucleTemp = temp1; bucleTemp < numeroArgumentos; bucleTemp++)
                    {
                        cmdList(temp2, temp3, temp4, temp5, temp6, temp7, argumentos[bucleTemp]);
                    }
                }
                else
                {
                    cmdChdir(0);
                }
            }
        }
        break;
    case 17:
        if (numeroArgumentos == 1)
        {
            cmdChdir(0);
        }
        else if (numeroArgumentos > 1 && strcmp(argumentos[1], "-?") == 0)
        {
            cmdHelp(argumentos[0]);
        }
        else
        {
            for (temp1 = 1; temp1 < numeroArgumentos; temp1++)
            {
                cmdDelete(argumentos[temp1]);
            }
        }
        break;
    case 18:
        if (numeroArgumentos == 1)
        {
            cmdChdir(0);
        }
        else if (numeroArgumentos > 1 && strcmp(argumentos[1], "-?") == 0)
        {
            cmdHelp(argumentos[0]);
        }
        else
        {
            for (temp1 = 1; temp1 < numeroArgumentos; temp1++)
            {
                cmdDeltree(argumentos[temp1]);
            }
        }
        break;
    default:
        printf("Comando no reconocido\n");
    }
}