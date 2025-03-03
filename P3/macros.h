/*
 *Jesus Jose Santamaria Santos: j.j.ssantos
 */

#ifndef UNTITLED3_MACROS_H
#define UNTITLED3_MACROS_H

#define AYUDA "authors pid chdir date time\n\
hist command open close dup\n\
listopen infosys help quit exit\n\
bye create stat list delete\n\
deltree"
#define AYUDA_AUTHORS "authors\n\
Muestra los nombres y logins de los autores.\n\
authors [-l]\n\
Muestra solo los logins.\n\
authors [-n]\n\
Muestra solo los nombres."
#define AYUDA_PID "pid\n\
Muestra el pid del shell.\n\
pid [-p]\n\
Muestra el pid del proceso padre."
#define AYUDA_CHDIR "chdir\n\
Muestra el directorio de trabajo actual.\n\
chdir [dir]\n\
Cambia el directorio actual a dir."
#define AYUDA_DATE "date\n\
Muestra la fecha actual."
#define AYUDA_TIME "time\n\
Muestra la hora actual."
#define AYUDA_HIST "hist\n\
Muestra el historial de comandos.\n\
hist [-c]\n\
Borra el historial de comandos.\n\
hist [-N]\n\
Muestra los N primeros comandos."
#define AYUDA_COMMAND "command N\n\
Repite el comando q ocupa el puesto N en el historial:"
#define AYUDA_OPEN "open [file] mode\n\
Abre el fichero file y lo anade a la lista de ficheros abiertos.\n\
Los modos disponibles son:\n\
cr: O_CREAT\n\
ap: O_APPEND\n\
ex: O_EXCL\n\
rw: O_RDWR\n\
wo: O_WRONLY\n\
tr: O_TRUNC\n\
ro: O_RDONLY"
#define AYUDA_CLOSE "close [df]\n\
Cierra el descriptor df y elimina el fichero correspondiente de la lista de ficheros abiertos."
#define AYUDA_DUP "dup [df]\n\
Duplica el descriptor df y anade una nueva entrada a la lista de ficheros abiertos."
#define AYUDA_LISTOPEN "listopen\n\
Lista los ficheros abiertos del shell.\n\
listopen [n]\n\
Lista los ficheros abiertos (al menos n) del shell."
#define AYUDA_INFOSYS "infosys\n\
Muestra información de la máquina donde se está ejecutando el shell."
#define AYUDA_HELP "help\n\
Muestra una lista con los comandos disponibles.\n\
help [cmd]\n\
Muestra informacion del comando cmd."
#define AYUDA_CREATE "create [f][name]\n\
Crea un directorio o un fichero [f] con nombre [name]."
#define AYUDA_STAT "stat [-long][-link][-acc] [name1] [name2] [...] \n\
lista ficheros\n\
-long: listado largo.\n\
-acc: acesstime.\n\
-link: si es enlace simbolico, el path contenido."
#define AYUDA_LIST "list [-reca] [-recb] [-hid][-long][-link][-acc] [n1] [n2] [...]\n\
Lista de contenidos de directorios\n\
-hid: incluye los ficheros ocultos\n\
-recb: recursivo (antes).\n\
-reca: recursivo (despues).\n\
-long: listado largo.\n\
-acc: acesstime.\n\
-link: si es enlace simbolico, el path contenido."
#define AYUDA_DELETE "delete [name1] [name2] [...]\n\
Borra ficheros o directorios vacios."
#define AYUDA_DELTREE "delete [name1] [name2] [...]\n\
Borra ficheros o directorios no vacios recursivamente."
#define AYUDA_MALLOC "malloc [-free] [tam]\n\
Asigna un bloque memoria de tamano tam con malloc.\n\
-free: desasigna un bloque de memoria de tamano tam asignado con malloc."
#define AYUDA_SHARED "shared [-free|-create|-delkey] cl [tam]\n\
Asigna memoria compartida con clave cl en el programa.\n\
-create cl tam: asigna (creando) el bloque de memoria compartida de clave cl y tamano tam.\n\
-free cl: desmapea el bloque de memoria compartida de clave cl.\n\
-delkey cl: elimina del sistema (sin desmapear) la clave de memoria c."
#define AYUDA_MMAP "mmap [-free] fich prm\n\
Mapea el fichero fich con permisos prm.\n\
-free fich: desmapea el fichero fich."
#define AYUDA_READ "read fiche addr cont\n\
Lee cont bytes desde fich a la direccion addr."
#define AYUDA_WRITE "write [-o] fiche addr cont\n\
Escribe cont bytes desde la direccion addr a fich.\n\
-o: sobreescribe."
#define AYUDA_MEMDUMP "memdump addr cont\n\
Vuelca en pantallas los contenidos (cont bytes) de la posicion de memoria addr."
#define AYUDA_MEMFILL "memfill addr cont byte\n\
Llena la memoria a partir de addr cont byte."
#define AYUDA_MEM "mem [-blocks|-funcs|-vars|-all|-pmap] [...]\n\
Muestra muestra detalles de la memoria del proceso.\n\
-blocks: los bloques de memoria asignados.\n\
-funcs: las direcciones de las funciones.\n\
-vars: las direcciones de las variables.\n\
:-all: todo.\n\
-pmap: muestra la salida del comando pmap(o similar)."
#define AYUDA_RECURSE "recurse [n]\n\
Invoca a la funcion cmdRecurse n veces."
#define AYUDA_UID "uid [-get|-set] [-l] [id] Accede a las credenciales del proceso que ejecuta el shell.\n\
-get: muestra las credenciales.\n\
-set id: establece la credencial al valor numerico id.\n\
-set -l id: establece la credencial a login id."
#define AYUDA_SHOWVAR "showvar var	Muestra el valor y las direcciones de la variable de entorno var."
#define AYUDA_CHANGEVAR "changevar [-a|-e|-p] var valor	Cambia el valor de una variable de entorno.\n\
-a: accede por el tercer arg de main.\n\
-e: accede mediante environ.\n\
-p: accede mediante putenv."
#define AYUDA_SUBSVAR "subsvar [-a|-e] var1 var2 valor Sustituye la variable de entorno var1 con var2=valor\n\
-a: accede por el tercer arg de main.\n\
-e: accede mediante environ."
#define AYUDA_SHOWENV "showenv [-environ|-addr] Muestra el entorno del proceso.\n\
-environ: accede usando environ (en lugar del tercer arg de main).\n\
-addr: muestra el valor y donde se almacenan environ y el 3er arg main."
#define AYUDA_FORK "fork El shell hace fork y queda en espera a que su hijo termine."
#define AYUDA_EXEC "exec VAR1 VAR2 ..prog args....[@pri] Ejecuta, sin crear proceso,prog con argumentos en un entorno que contiene solo las variables VAR1, VAR2..."
#define AYUDA_JOBS "jobs Lista los procesos en segundo plano."
#define AYUDA_DELJOBS "deljobs [-term][-sig]	Elimina los procesos de la lista procesos en sp.\n\
-term: los terminados.\n\
-sig: los terminados por senal."
#define AYUDA_JOB "job [-fg] pid Muestra informacion del proceso pid.\n\
-fg: lo pasa a primer plano."
#define AYUDA_EXIT "exit/quit/bye\n\
Termina la ejecucion del shell."
#define ENTRADAMAX 1024
#define HISTORIALMAX 2048
#define TAMANO 2048
#define MAXVAR 4096
#define PROMPT "(っ▀¯▀)つ"
#define NOMBRE1 "Jesus Jose Santamaria Santos"
#define LOGIN1 "j.j.ssantos"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/utsname.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <grp.h>
#include <pwd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <sys/resource.h>
#include <time.h>
#include <sys/types.h>
#include <malloc.h>
#include <stdio.h>
#include <signal.h>

#endif // UNTITLED3_MACROS_H