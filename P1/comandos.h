/*
 *Jesus Jose Santamaria Santos: j.j.ssantos
 */

#ifndef UNTITLED3_COMANDOS_H
#define UNTITLED3_COMANDOS_H

#include "tipos.h"

#include <sys/types.h>
#include <stdbool.h>

void cmdAuthors(char *argumentos);
void cmdPid(char *argumento);
void cmdChdir(char *argumentos);
void cmdDate();
void cmdTime();
void cmdHist(char *argumentos, historial *historial1);
void cmdCommand(char *argumentos, historial *historial1, informacionFichero *ficherosAbiertos);
void cmdOpen(char *tr[], informacionFichero *ficherosAbiertos);
void cmdClose(char *tr[], informacionFichero *ficherosAbiertos);
void cmdDup(char *tr[], informacionFichero *ficherosAbiertos);
void cmdListopen(informacionFichero *ficherosAbiertos);
void cmdInfosys();
void cmdHelp(char *argumentos);
void cmdSalida();
void cmdCreate(int fichero, char *argumentos);
char LetraTF(mode_t m);
char *ConvierteModo2(mode_t m);
void cmdStat(int caso, char *argumentos, bool transformarTemp);
void cmdList(int temp2, int temp3, int temp4, int temp5, int temp6, int temp7, char *argumentos);
void cmdDelete(char *argumentos);
void cmdDeltree(char *argumentos);

#endif // UNTITLED3_COMANDOS_H