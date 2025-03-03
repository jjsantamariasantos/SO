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
void cmdCommand(char *argumentos, historial *historial1, informacionFichero *ficherosAbiertos, informacionMemoria *listaMemoria);
void cmdOpen(char *tr[], informacionFichero *ficherosAbiertos);
void cmdClose(char *tr[], informacionFichero *ficherosAbiertos);
void cmdDup(char *tr[], informacionFichero *ficherosAbiertos);
void cmdListopen(informacionFichero *ficherosAbiertos);
void cmdInfosys();
void cmdHelp(char *argumentos);
void cmdSalida(informacionMemoria *lista);
void cmdCreate(int fichero, char *argumentos);
char letraTF(mode_t m);
char *convierteModo2(mode_t m);
void cmdStat(int caso, char *argumentos, bool transformarTemp);
void cmdList(int temp2, int temp3, int temp4, int temp5, int temp6, int temp7, char *argumentos);
void cmdDelete(char *argumentos);
void cmdDeltree(char *argumentos);
void llenarMemoria(void *p, size_t cont, unsigned char byte);
void *obtenerMemoriaShmget(key_t clave, size_t tam, informacionMemoria *lista);
void sharedCreate(char *tr[], informacionMemoria *lista);
void *mapearFichero(char *fichero, int protection, informacionMemoria *lista);
void sharedDelkey(char *args[]);
// ssize_t escribirFichero(char *f, void *p, size_t cont, int overwrite);
ssize_t leerFichero(char *f, void *p, size_t cont);
void do_MemPmap(void);
void cmdMalloc(char *trozos[], informacionMemoria *lista);
void cmdMmap(char *arg[], informacionMemoria *lista);
void cmdRead(char *ar[]);
void cmdWrite(char *trozos[]);
void cmdMemDump(char *trozos[]);
void cmdMemFill(char *trozos[]);
void cmdMem(char *trozos[], informacionMemoria *lista);
void cmdRecurse(int n);
void *cadtop(const char *cadena);
void imprimirMemDump(char *p, int n);
void imprimirChar(char c);
void imprimirHex(char c);
#endif // UNTITLED3_COMANDOS_H