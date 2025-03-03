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
#define AYUDA_QUIT "quit\n\
Termina la ejecucion del shell."
#define AYUDA_EXIT "exit\n\
Termina la ejecucion del shell."
#define AYUDA_BYE "bye\n\
Termina la ejecucion del shell."
#define ENTRADAMAX 1024
#define HISTORIALMAX 2048
#define PROMPT "(っ▀¯▀)つ"
#define NOMBRE1 "Jesus Jose Santamaria Santos"
#define LOGIN1 "j.j.ssantos"

#endif // UNTITLED3_MACROS_H