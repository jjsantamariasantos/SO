/*
 *Jesus Jose Santamaria Santos: j.j.ssantos
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <fcntl.h>

#define AYUDA "authors pid chdir date time\n\
hist command open close dup\n\
listopen infosys help quit exit\n\
bye"
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

int trocearCadena(char *cadena, char *trozos[]);
void escanearEntrada(char *entrada);
void ejecutarEntrada(char *entrada, char *argumentos[], historial *historial1, informacionFichero *ficherosAbiertos);
void ejecutarOrden(char *entrada, char *argumentos[], historial *historial1, informacionFichero *ficherosAbiertos);
int insertarEnElHistorial(char *orden, historial *historial1);
int insertarEnElFichero(int identificadorDelFichero, int modo, char *nombreDelFichero, informacionFichero *ficherosAbiertos);
void borrarDelHistorial(historial *historial1);
void borrarDelFichero(int identificadorDelFichero, informacionFichero *ficherosAbiertos);
int listaOrdenes(char *argumentos[]);
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

int main()
{
    historial historial1[HISTORIALMAX];
    char entrada[ENTRADAMAX];
    char *argumentos[ENTRADAMAX];
    informacionFichero ficherosAbiertos[HISTORIALMAX];
    int identificadorDelFichero1 = 0, identificadorDelFichero2 = 1,
        identificadorDelFichero3 = 2, modo1 = fcntl(identificadorDelFichero1, F_GETFL), modo2 = fcntl(identificadorDelFichero2, F_GETFL), modo3 = fcntl(identificadorDelFichero3, F_GETFL);
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
int trocearCadena(char *cadena, char *trozos[])
{
    int i = 1;

    if ((trozos[0] = strtok(cadena, " \n\t")) == NULL)
        return 0;
    while ((trozos[i] = strtok(NULL, " \n\t")) != NULL)
        i++;
    return i;
}
void escanearEntrada(char *entrada)
{
    fgets(entrada, ENTRADAMAX, stdin);
}
void ejecutarEntrada(char *entrada, char *argumentos[], historial *historial1, informacionFichero *ficherosAbiertos)
{
    char *temp;
    int postemp;

    temp = malloc(sizeof(entrada));
    strcpy(temp, entrada);
    if (strstr(temp, "command") != NULL && strstr(temp, "help") == NULL)
    {
    }
    else
    {
        postemp = insertarEnElHistorial(temp, historial1);
        if (postemp == -1)
        {
            printf("Error: No se ha podido insertar en el historial");
            return;
        }
    }
    ejecutarOrden(entrada, argumentos, historial1, ficherosAbiertos);
    free(temp);
    temp = NULL;
}
void ejecutarOrden(char *entrada, char *argumentos[], historial *historial1, informacionFichero *ficherosAbiertos)
{
    int numeroArgumentos = trocearCadena(entrada, argumentos);

    switch (listaOrdenes(&argumentos[0]))
    {
    case 0:
        if (numeroArgumentos == 1)
        {
            cmdAuthors(0);
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
        else
        {
            cmdChdir(argumentos[1]);
        }
        break;
    case 3:
        cmdDate();
        break;
    case 4:
        cmdTime();
        break;
    case 5:
        if (numeroArgumentos == 1)
        {
            cmdHist(0, historial1);
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
        else
        {
            cmdCommand(argumentos[1], historial1, ficherosAbiertos);
        }
        break;
    case 7:
        cmdOpen(argumentos, ficherosAbiertos);
        break;
    case 8:
        cmdClose(argumentos, ficherosAbiertos);
        break;
    case 9:
        cmdDup(argumentos, ficherosAbiertos);
        break;
    case 10:
        cmdListopen(ficherosAbiertos);
        break;
    case 11:
        cmdInfosys();
        break;
    case 12:
        if (numeroArgumentos == 1)
        {
            cmdHelp(0);
        }
        else
        {
            cmdHelp(argumentos[1]);
        }
        break;
    case 13:
        cmdSalida();
        break;
    default:
        printf("Comando no reconocido\n");
    }
}
int insertarEnElHistorial(char *orden, historial *historial1)
{
    int temp;

    for (temp = 0; temp < HISTORIALMAX && historial1[temp].orden != NULL; temp++)
        ;
    if (temp < HISTORIALMAX && historial1[temp].orden == NULL)
    {
        historial1[temp].orden = malloc(strlen(orden) + 1);
        strcpy(historial1[temp].orden, orden);
        historial1[temp].siguiente = NULL;
        return 0;
    }
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
    return -1;
}
void cmdAuthors(char *argumentos)
{
    if (argumentos == 0)
    {
        printf("%s: %s\n", NOMBRE1, LOGIN1);
    }
    else if (!strcmp(argumentos, "-l"))
    {
        printf("%s\n", LOGIN1);
    }
    else if (!strcmp(argumentos, "-n"))
    {
        printf("%s\n", NOMBRE1);
    }
    else
    {
        printf("Error: argumento no reconocido\n");
    }
}
void cmdPid(char *argumento)
{
    if (argumento == 0)
    {
        printf("Pid del Shell: %d\n", getpid());
    }
    else if (strcmp(argumento, "-p") == 0)
    {
        printf("Pid del padre del Shell: %d\n", getppid());
    }
    else
    {
        printf("Error: argumento no reconocido\n");
    }
}
void cmdChdir(char *argumentos)
{
    char directorioact[ENTRADAMAX];

    if (argumentos == 0)
    {
        if (getcwd(directorioact, sizeof(directorioact)) == NULL)
        {
            perror("Imposible abrir fichero");
        }
        else
        {
            printf("%s\n", directorioact);
        }
    }
    else if (chdir(argumentos) != 0)
    {
        perror("Imposible cambiar de directorio");
    }
}
void cmdDate()
{
    time_t tiempo = time(NULL);
    struct tm tiempolocal = *localtime(&tiempo);
    char fecha[12];
    char *formato = "%d/%m/%Y\n";

    if (tiempo == -1)
    {
        perror("Error obteniendo el tiempo");
        return;
    }
    if (localtime_r(&tiempo, &tiempolocal) == NULL)
    {
        perror("Error conviertiendo el tiempo");
        return;
    }
    if (strftime(fecha, sizeof(fecha), formato, &tiempolocal) == 0)
    {
        perror("Error formateando el tiempo");
        return;
    }
    printf("%s", fecha);
}
void cmdTime()
{
    time_t tiempo = time(NULL);
    struct tm tiempolocal = *localtime(&tiempo);
    char hora[10];
    char *formato = "%H:%M:%S\n";

    if (tiempo == -1)
    {
        perror("Error obteniendo el tiempo");
        return;
    }
    if (localtime_r(&tiempo, &tiempolocal) == NULL)
    {
        perror("Error convirtiendo el tiempo");
        return;
    }
    if (strftime(hora, sizeof(hora), formato, &tiempolocal) == 0)
    {
        perror("Error formateando el tiempo");
        return;
    }
    printf("%s", hora);
}
void cmdHist(char *argumentos, historial *historial1)
{
    int temp, postemp;

    if (argumentos == 0)
    {
        for (temp = 0; temp < HISTORIALMAX && historial1[temp].orden != NULL; temp++)
        {
            printf("%d -> %s", temp, historial1[temp].orden);
        }
    }
    else if (strcmp(argumentos, "-c") == 0)
    {
        borrarDelHistorial(historial1);
    }
    else
    {
        postemp = atoi(argumentos);
        if (postemp > 0)
        {
            for (temp = 0; temp < postemp && historial1[temp].orden != NULL; temp++)
            {
                printf("%d -> %s", temp, historial1[temp].orden);
            }
        }
    }
}
void cmdCommand(char *argumentos, historial *historial1,
                informacionFichero *ficherosAbiertos)
{
    int temp;
    char *entradaTemp, *argumentosTemp[ENTRADAMAX];

    temp = atoi(argumentos);
    if (temp >= 0 && historial1[temp].orden != NULL)
    {
        printf("%s", historial1[temp].orden);
        entradaTemp = malloc(strlen(historial1[temp].orden) + 1);
        strcpy(entradaTemp, historial1[temp].orden);
        ejecutarOrden(entradaTemp, argumentosTemp, historial1, ficherosAbiertos);
        free(entradaTemp);
        entradaTemp = NULL;
    }
    else
    {
        printf("Error: comando no encontrado\n");
    }
}
void cmdOpen(char *tr[], informacionFichero *ficherosAbiertos)
{
    int i, df, mode = 0;

    if (tr[1] == NULL)
    { /*no hay parametro*/
        cmdListopen(ficherosAbiertos);
        return;
    }
    for (i = 2; tr[i] != NULL; i++)
    {
        if (!strcmp(tr[i], "cr"))
            mode |= O_CREAT;
        else if (!strcmp(tr[i], "ex"))
            mode |= O_EXCL;
        else if (!strcmp(tr[i], "ro"))
            mode |= O_RDONLY;
        else if (!strcmp(tr[i], "wo"))
            mode |= O_WRONLY;
        else if (!strcmp(tr[i], "rw"))
            mode |= O_RDWR;
        else if (!strcmp(tr[i], "ap"))
            mode |= O_APPEND;
        else if (!strcmp(tr[i], "tr"))
            mode |= O_TRUNC;
        else
            break;
    }
    if ((df = open(tr[1], mode, 0777)) == -1)
    {
        perror("Imposible abrir fichero");
    }
    else
    {
        insertarEnElFichero(df, mode, tr[1], ficherosAbiertos);
        printf("Anadida entrada a la tabla ficheros abiertos.\n");
    }
}
void cmdClose(char *tr[], informacionFichero *ficherosAbiertos)
{
    int df;

    if (tr[1] == NULL || (df = atoi(tr[1])) < 0)
    {
        cmdListopen(ficherosAbiertos);
        return;
    }
    if (close(df) == -1)
    {
        perror("Imposible cerrar descriptor");
    }
    else
    {
        borrarDelFichero(df, ficherosAbiertos);
    }
}
void cmdDup(char *tr[], informacionFichero *ficherosAbiertos)
{
    int df, duplicado, temp;
    char aux[ENTRADAMAX], *p;

    if (tr[1] == NULL || (df = atoi(tr[1])) < 0)
    {
        cmdListopen(ficherosAbiertos);
        return;
    }
    for (temp = 0; temp < HISTORIALMAX && ficherosAbiertos[temp].identificadorDelFichero != df; temp++)
        ;
    p = ficherosAbiertos[temp].nombreDelFichero;
    if (p == NULL)
    {
        printf("Error: No se ha encontrado el descriptor\n");
    }
    if ((duplicado = dup(df)) == -1)
    {
        perror("Error:");
    }
    else
    {
        sprintf(aux, "dup %d (%s)", df, p);
        insertarEnElFichero(duplicado, fcntl(duplicado, F_GETFL), aux, ficherosAbiertos);
    }
}
void cmdListopen(informacionFichero *ficherosAbiertos)
{
    int temp, flags;
    char modo[ENTRADAMAX];

    for (temp = 0; temp < HISTORIALMAX && ficherosAbiertos[temp].nombreDelFichero != NULL; temp++)
    {
        flags = fcntl(ficherosAbiertos[temp].identificadorDelFichero, F_GETFL);
        strcpy(modo, "");
        if (flags == -1)
        {
        }
        else if (flags & O_RDONLY)
        {
            strcat(modo, "O_RDONLY ");
        }
        else if (flags & O_CREAT)
        {
            strcat(modo, "O_CREAT ");
        }
        else if (flags & O_EXCL)
        {
            strcat(modo, "O_EXCL ");
        }
        else if (flags & O_WRONLY)
        {
            strcat(modo, "O_WRONLY ");
        }
        else if (flags & O_RDWR)
        {
            strcat(modo, "O_RDWR ");
        }
        else if (flags & O_APPEND)
        {
            strcat(modo, "O_APPEND ");
        }
        else if (flags & O_TRUNC)
        {
            strcat(modo, "O_TRUNC ");
        }
        printf("Descriptor: %d ->%s %s\n", ficherosAbiertos[temp].identificadorDelFichero, ficherosAbiertos[temp].nombreDelFichero, modo);
    }
}

void cmdInfosys()
{
    struct utsname unameInfo;

    if (uname(&unameInfo) == -1)
    {
        perror("Error");
    }
    else
    {
        printf("%s (%s), OS: %s-%s-%s\n", unameInfo.nodename, unameInfo.machine, unameInfo.sysname, unameInfo.release, unameInfo.version);
    }
}
void cmdHelp(char *argumentos)
{
    if (argumentos == 0)
    {
        printf("%s\n", AYUDA);
    }
    else if (strcmp(argumentos, "authors") == 0)
    {
        printf("%s\n", AYUDA_AUTHORS);
    }
    else if (strcmp(argumentos, "pid") == 0)
    {
        printf("%s\n", AYUDA_PID);
    }
    else if (strcmp(argumentos, "chdir") == 0)
    {
        printf("%s\n", AYUDA_CHDIR);
    }
    else if (strcmp(argumentos, "date") == 0)
    {
        printf("%s\n", AYUDA_DATE);
    }
    else if (strcmp(argumentos, "time") == 0)
    {
        printf("%s\n", AYUDA_TIME);
    }
    else if (strcmp(argumentos, "hist") == 0)
    {
        printf("%s\n", AYUDA_HIST);
    }
    else if (strcmp(argumentos, "command") == 0)
    {
        printf("%s\n", AYUDA_COMMAND);
    }
    else if (strcmp(argumentos, "open") == 0)
    {
        printf("%s\n", AYUDA_OPEN);
    }
    else if (strcmp(argumentos, "close") == 0)
    {
        printf("%s\n", AYUDA_CLOSE);
    }
    else if (strcmp(argumentos, "dup") == 0)
    {
        printf("%s\n", AYUDA_DUP);
    }
    else if (strcmp(argumentos, "listopen") == 0)
    {
        printf("%s\n", AYUDA_LISTOPEN);
    }
    else if (strcmp(argumentos, "infosys") == 0)
    {
        printf("%s\n", AYUDA_INFOSYS);
    }
    else if (strcmp(argumentos, "help") == 0)
    {
        printf("%s\n", AYUDA_HELP);
    }
    else if (strcmp(argumentos, "exit") == 0)
    {
        printf("%s\n", AYUDA_EXIT);
    }
    else if (strcmp(argumentos, "quit") == 0)
    {
        printf("%s\n", AYUDA_QUIT);
    }
    else if (strcmp(argumentos, "bye") == 0)
    {
        printf("%s\n", AYUDA_BYE);
    }
    else
    {
        printf("Error: comando no reconocido\n");
    }
}
void cmdSalida()
{
    exit(0);
}
