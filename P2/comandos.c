/*
 *Jesus Jose Santamaria Santos: j.j.ssantos
 */

#include "comandos.h"
#include "macros.h"
#include "auxiliar.h"

#include <time.h>
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

int intGlobal = 0;
char charGlobal = 'r';
double floatGlobal = 1.2;

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
    char directorioAct[ENTRADAMAX];

    if (argumentos == 0)
    {
        if (getcwd(directorioAct, sizeof(directorioAct)) == NULL)
        {
            perror("Imposible abrir fichero");
        }
        else
        {
            printf("%s\n", directorioAct);
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
    struct tm tiempoLocal = *localtime(&tiempo);
    char fecha[12];
    char *formato = "%d/%m/%Y\n";

    if (tiempo == -1)
    {
        perror("Error obteniendo el tiempo");
        return;
    }
    if (localtime_r(&tiempo, &tiempoLocal) == NULL)
    {
        perror("Error conviertiendo el tiempo");
        return;
    }
    if (strftime(fecha, sizeof(fecha), formato, &tiempoLocal) == 0)
    {
        perror("Error formateando el tiempo");
        return;
    }
    printf("%s", fecha);
}

void cmdTime()
{
    time_t tiempo = time(NULL);
    struct tm tiempoLocal = *localtime(&tiempo);
    char hora[10];
    char *formato = "%H:%M:%S\n";

    if (tiempo == -1)
    {
        perror("Error obteniendo el tiempo");
        return;
    }
    if (localtime_r(&tiempo, &tiempoLocal) == NULL)
    {
        perror("Error convirtiendo el tiempo");
        return;
    }
    if (strftime(hora, sizeof(hora), formato, &tiempoLocal) == 0)
    {
        perror("Error formateando el tiempo");
        return;
    }
    printf("%s", hora);
}

void cmdHist(char *argumentos, historial *historial1)
{
    int temp, posTemp;

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
        posTemp = atoi(argumentos);
        if (posTemp > 0)
        {
            for (temp = 0; temp < posTemp && historial1[temp].orden != NULL; temp++)
            {
                printf("%d -> %s", temp, historial1[temp].orden);
            }
        }
    }
}

void cmdCommand(char *argumentos, historial *historial1, informacionFichero *ficherosAbiertos, informacionMemoria *listaMemoria)
{
    int temp;
    char *entradaTemp, *argumentosTemp[ENTRADAMAX];

    temp = atoi(argumentos);
    if (temp >= 0 && historial1[temp].orden != NULL)
    {
        printf("%s", historial1[temp].orden);
        entradaTemp = malloc(strlen(historial1[temp].orden) + 1);
        strcpy(entradaTemp, historial1[temp].orden);
        ejecutarOrden(entradaTemp, argumentosTemp, historial1, ficherosAbiertos, listaMemoria);
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
    {
        cmdListopen(ficherosAbiertos);
        return;
    }
    if (tr[2] == NULL)
    {
        mode |= O_CREAT;
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

    for (temp = 0; temp < HISTORIALMAX && ficherosAbiertos[temp].nombreDelFichero != NULL; temp++)
    {
        flags = fcntl(ficherosAbiertos[temp].identificadorDelFichero, F_GETFL);
        char modo[ENTRADAMAX];

        strcpy(modo, "");
        if (flags == -1)
        {
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
        else
        {
            strcat(modo, "O_RDONLY ");
        }
        printf("Descriptor: %d ->%s %s\n", ficherosAbiertos[temp].identificadorDelFichero,
            ficherosAbiertos[temp].nombreDelFichero, modo);
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
        printf("%s (%s), OS: %s-%s-%s\n", unameInfo.nodename, unameInfo.machine, unameInfo.sysname,
            unameInfo.release, unameInfo.version);
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
    else if (strcmp(argumentos, "exit") == 0 || strcmp(argumentos, "quit") == 0 || strcmp(argumentos, "bye") == 0)
    {
        printf("%s\n", AYUDA_EXIT);
    }
    else if (strcmp(argumentos, "create") == 0)
    {
        printf("%s\n", AYUDA_CREATE);
    }
    else if (strcmp(argumentos, "stat") == 0)
    {
        printf("%s\n", AYUDA_STAT);
    }
    else if (strcmp(argumentos, "list") == 0)
    {
        printf("%s\n", AYUDA_LIST);
    }
    else if (strcmp(argumentos, "delete") == 0)
    {
        printf("%s\n", AYUDA_DELETE);
    }
    else if (strcmp(argumentos, "deltree") == 0)
    {
        printf("%s\n", AYUDA_DELTREE);
    }
    else if (strcmp(argumentos, "malloc") == 0)
    {
        printf("%s\n", AYUDA_MALLOC);
    }
    else if (strcmp(argumentos, "shared") == 0)
    {
        printf("%s\n", AYUDA_SHARED);
    }
    else if (strcmp(argumentos, "mmap") == 0)
    {
        printf("%s\n", AYUDA_MMAP);
    }
    else if (strcmp(argumentos, "read") == 0)
    {
        printf("%s\n", AYUDA_READ);
    }
    else if (strcmp(argumentos, "write") == 0)
    {
        printf("%s\n", AYUDA_WRITE);
    }
    else if (strcmp(argumentos, "memdump") == 0)
    {
        printf("%s\n", AYUDA_MEMDUMP);
    }
    else if (strcmp(argumentos, "memfill") == 0)
    {
        printf("%s\n", AYUDA_MEMFILL);
    }
    else if (strcmp(argumentos, "mem") == 0)
    {
        printf("%s\n", AYUDA_MEM);
    }
    else if (strcmp(argumentos, "recurse") == 0)
    {
        printf("%s\n", AYUDA_RECURSE);
    }
    else
    {
        printf("Error: comando no reconocido\n");
    }
}

void cmdSalida(informacionMemoria *lista)
{
    eliminarMemoria(&lista);
    exit(0);
}

void cmdCreate(int fichero, char *argumentos)
{
    FILE *ficheroTemp;

    if (fichero == 0)
    {
        if (access(argumentos, F_OK) != -1)
        {
            printf("Ya existe un directorio con ese nombre.\n");
        }
        else
        {
            if (mkdir(argumentos, S_IRWXU) && errno != EEXIST)
            {
                perror("No se ha podido crear el directorio:");
            }
            else
            {
                printf("Directorio creado\n");
            }
        }
    }
    else
    {
        if (access(argumentos, F_OK) != -1)
        {
            printf("Ya existe un fichero con ese nombre.\n");
        }
        else
        {
            ficheroTemp = fopen(argumentos, "w+");
            if (ficheroTemp && errno != ENOENT)
            {
                perror("No se ha podido crear el fichero\n");
            }
            else
            {
                printf("Se ha creado el fichero\n");
            }
        }
    }
}

char letraTF(mode_t m)
{
    switch (m & S_IFMT)
    { /*and bit a bit con los bits de formato,0170000 */
    case S_IFSOCK:
        return 's'; /*socket */
    case S_IFLNK:
        return 'l'; /*symbolic link*/
    case S_IFREG:
        return '-'; /* fichero normal*/
    case S_IFBLK:
        return 'b'; /*block device*/
    case S_IFDIR:
        return 'd'; /*directorio */
    case S_IFCHR:
        return 'c'; /*char device*/
    case S_IFIFO:
        return 'p'; /*pipe*/
    default:
        return '?'; /*desconocido, no deberia aparecer*/
    }
}

char *convierteModo2(mode_t m)
{
    static char permisos[12];
    strcpy(permisos, "---------- ");

    permisos[0] = letraTF(m);
    if (m & S_IRUSR)
        permisos[1] = 'r'; /*propietario*/
    if (m & S_IWUSR)
        permisos[2] = 'w';
    if (m & S_IXUSR)
        permisos[3] = 'x';
    if (m & S_IRGRP)
        permisos[4] = 'r'; /*grupo*/
    if (m & S_IWGRP)
        permisos[5] = 'w';
    if (m & S_IXGRP)
        permisos[6] = 'x';
    if (m & S_IROTH)
        permisos[7] = 'r'; /*resto*/
    if (m & S_IWOTH)
        permisos[8] = 'w';
    if (m & S_IXOTH)
        permisos[9] = 'x';
    if (m & S_ISUID)
        permisos[3] = 's'; /*setuid, setgid y stickybit*/
    if (m & S_ISGID)
        permisos[6] = 's';
    if (m & S_ISVTX)
        permisos[9] = 't';

    return permisos;
}

void cmdStat(int caso, char *argumentos, bool transformarTemp)
{
    DIR *dirTemp;
    struct stat temp;
    struct passwd *propietario;
    struct group *grupo;
    int tamano = 4096, link, inodo, tempTrozos = 0;
    time_t t;
    char *modo, fecha[50], *argumentosTemp[ENTRADAMAX],
        imprimirLink1[PATH_MAX + 1] = "", imprimirLink2[PATH_MAX + 1] = "";

    if (lstat(argumentos, &temp) != 0)
    {
        perror("No se pudo acceder al directorio/fichero en cuestion");
    }
    else
    {
        if (caso == 1 || caso == 2)
        {
            t = temp.st_mtime;
        }
        else
        {
            t = temp.st_atime;
        }
        if (caso == 2 || caso == 4)
        {
            if (readlink(argumentos, imprimirLink2, PATH_MAX) == 0)
            {
                perror("Se ha producido un error al procesar el link");
                return;
            }
            else
            {
                strcpy(imprimirLink1, "");
                strcat(imprimirLink1, " -> ");
                if (strcmp(imprimirLink2, "") == 0)
                {
                    strcpy(imprimirLink1, "");
                }
                strcat(imprimirLink1, imprimirLink2);
            }
        }
        if ((dirTemp = opendir(argumentos)) == NULL)
        {
            tamano = (int)temp.st_size;
        }
        else
        {
            closedir(dirTemp);
        }
        propietario = getpwuid(temp.st_uid);
        grupo = getgrgid(temp.st_gid);
        link = (int)temp.st_nlink;
        inodo = (int)temp.st_ino;
        modo = convierteModo2(temp.st_mode);
        strftime(fecha, 50, "%Y/%m/%d-%H:%M", localtime(&t));
        if (transformarTemp)
        {
            strcpy((char *)argumentosTemp, argumentos);
            if ((argumentosTemp[0] = strtok(argumentos, "/")) != NULL)
            {

                while ((argumentosTemp[tempTrozos] = strtok(NULL, "/")) != NULL)
                {
                    tempTrozos++;
                }
                if (caso == 0)
                {
                    printf("%10d%5s%s\n", tamano, " ", argumentosTemp[tempTrozos - 1]);
                }
                else
                {
                    printf("%10s%4d(%d)%10s%10s%12s%10d%5s%s%5s%s\n", fecha, link, inodo,
                           propietario->pw_name, grupo->gr_name, modo, tamano, " ",
                           argumentosTemp[tempTrozos - 1], " ", imprimirLink1);
                }
            }
        }
        else
        {
            if (caso == 0)
            {
                printf("%10d%5s%s\n", tamano, " ", argumentos);
            }
            else
            {
                printf("%10s%4d(%d)%10s%10s%12s%10d%5s%s%5s%s\n", fecha, link, inodo,
                       propietario->pw_name, grupo->gr_name, modo, tamano, " ",
                       argumentos, " ", imprimirLink1);
            }
        }
    }
}

void cmdList(int temp2, int temp3, int temp4, int temp5, int temp6, int temp7, char *argumentos)
{
    DIR *dirTemp;
    struct stat temp;
    struct dirent *infoTemp;
    int suma = 0, caso = 0;
    bool transformarTemp = true;

    if (lstat(argumentos, &temp) != 0)
    {
        perror("No se pudo acceder");
    }
    else
    {
        suma = temp5 + temp6 + temp7;
        if (suma == 0)
        {
            caso = 0;
        }
        else if (suma == 1)
        {
            if (temp5 == 1)
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
            if (temp5 == 1)
            {
                if (temp6 == 1)
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
        if (S_ISDIR(temp.st_mode))
        {
            if ((dirTemp = opendir(argumentos)) == NULL)
            {
                perror("No se pudo abrir");
            }
            else
            {
                if (temp2 == 0)
                {
                    printf("***********%s\n", argumentos);
                    while ((infoTemp = readdir(dirTemp)) != NULL)
                    {
                        char pathTemp[PATH_MAX];

                        if (infoTemp->d_name[0] == '.' && temp4 == 0)
                            continue;
                        sprintf(pathTemp, "%s/%s", argumentos, infoTemp->d_name);
                        if (lstat(pathTemp, &temp) != 0)
                        {
                            perror("No se pudo acceder");
                        }
                        else
                        {
                            cmdStat(caso, pathTemp, transformarTemp);
                        }
                    }
                    closedir(dirTemp);
                    dirTemp = opendir(argumentos);
                }
                while ((infoTemp = readdir(dirTemp)) != NULL)
                {
                    char pathTemp[PATH_MAX];
                    if (strcmp(infoTemp->d_name, ".") != 0 && strcmp(infoTemp->d_name, "..") != 0)
                    {
                        if (infoTemp->d_name[0] == '.' && temp4 == 0)
                            continue;
                        sprintf(pathTemp, "%s/%s", argumentos, infoTemp->d_name);
                        if (lstat(pathTemp, &temp) != 0)
                        {
                            perror("No se pudo acceder");
                        }
                        else
                        {
                            if (temp2 || temp3)
                            {
                                cmdList(temp2, temp3, temp4, temp5, temp6, temp7, pathTemp);
                            }
                        }
                    }
                }
                if (temp2 == 1)
                {
                    printf("***********%s\n", argumentos);
                    closedir(dirTemp);
                    dirTemp = opendir(argumentos);
                    while ((infoTemp = readdir(dirTemp)) != NULL)
                    {
                        char pathTemp[PATH_MAX];

                        if (infoTemp->d_name[0] == '.' && temp4 == 0)
                            continue;
                        sprintf(pathTemp, "%s/%s", argumentos, infoTemp->d_name);
                        if (lstat(pathTemp, &temp) != 0)
                        {
                            perror("No se pudo acceder");
                        }
                        else
                        {
                            cmdStat(caso, pathTemp, transformarTemp);
                        }
                    }
                }
                closedir(dirTemp);
            }
        }
        else if (S_ISREG(temp.st_mode) && !(temp2 || temp3))
        {
            cmdStat(caso, argumentos, 0);
        }
    }
}

void cmdDelete(char *argumentos)
{
    struct stat temp;

    if (lstat(argumentos, &temp) != 0)
    {
        perror("No se pudo acceder al directorio/fichero en cuestion");
    }
    else if (S_ISDIR(temp.st_mode))
    {
        if (rmdir(argumentos) != 0)
        {
            perror("No se pudo borrar el directorio");
        }
    }
    else if (S_ISREG(temp.st_mode))
    {
        if (unlink(argumentos) != 0)
        {
            perror("No se pudo borrar el fichero");
        }
    }
    else
    {
        printf("El nombre %s no ha sido reconocido ni como fichero ni como directorio", argumentos);
    }
}

void cmdDeltree(char *argumentos)
{
    struct stat temp;
    struct dirent *infoTemp;
    DIR *dirTemp;
    char argumentoTemp[PATH_MAX];

    if (lstat(argumentos, &temp) != 0)
    {
        perror("No se pudo acceder");
    }
    else if (S_ISDIR(temp.st_mode))
    {
        dirTemp = opendir(argumentos);
        if (rmdir(argumentos) != 0)
        {
            if (dirTemp == NULL)
            {
                perror("No se pudo abrir el directorio");
            }
            else
            {
                while ((infoTemp = readdir(dirTemp)) != NULL)
                {
                    if (strcmp(infoTemp->d_name, ".") != 0 && strcmp(infoTemp->d_name, "..") != 0)
                    {
                        sprintf(argumentoTemp, "%s/%s", argumentos, infoTemp->d_name);
                        cmdDeltree(argumentoTemp);
                    }
                }
            }
            closedir(dirTemp);
            if (rmdir(argumentos) != 0)
            {
                perror("No se pudo borrar el directorio");
            }
        }
    }
    else if (S_ISREG(temp.st_mode))
    {
        if (unlink(argumentos) != 0)
        {
            perror("No se pudo borrar el fichero");
        }
    }
    else
    {
        printf("El nombre %s no ha sido reconocido ni como fichero ni como directorio", argumentos);
    }
}

void llenarMemoria(void *p, size_t cont, unsigned char byte)
{
    unsigned char *arr = (unsigned char *)p;
    size_t i;

    for (i = 0; i < cont; i++)
        arr[i] = byte;
    printf("Memoria llenada desde %p con %zu bytes usando el caracter %c.\n", p, cont, byte);
}

void *obtenerMemoriaShmget(key_t clave, size_t tam, informacionMemoria *lista)
{
    void *p;
    int aux, id, flags = 0777;
    struct shmid_ds s;

    if (tam) /*tam distito de 0 indica crear */
        flags = flags | IPC_CREAT | IPC_EXCL;
    if (clave == IPC_PRIVATE) /*no nos vale*/
    {
        errno = EINVAL;
        return NULL;
    }
    if ((id = shmget(clave, tam, flags)) == -1)
        return (NULL);
    if ((p = shmat(id, NULL, 0)) == (void *)-1)
    {
        aux = errno;
        if (tam)
            shmctl(id, IPC_RMID, NULL);
        errno = aux;
        return (NULL);
    }
    shmctl(id, IPC_STAT, &s);
    insertarEnMemoria(&lista, p, s.shm_segsz, "shared", clave, -1, "");
    return (p);
}

void sharedCreate(char *tr[], informacionMemoria *lista)
{
    key_t cl;
    size_t tam;
    void *p;

    if (tr[2] == NULL || tr[3] == NULL)
    {
        imprimirLista(lista, "shared");
        return;
    }

    cl = (key_t)strtoul(tr[2], NULL, 10);
    tam = (size_t)strtoul(tr[3], NULL, 10);
    if (tam == 0)
    {
        printf("No se asignan bloques de 0 bytes\n");
        return;
    }
    if ((p = obtenerMemoriaShmget(cl, tam, lista)) != NULL)
        printf("Asignados %lu bytes en %p\n", (unsigned long)tam, p);
    else
        printf("Imposible asignar memoria compartida clave %lu:%s\n", (unsigned long)cl, strerror(errno));
}

void *mapearFichero(char *fichero, int protection, informacionMemoria *lista)
{
    int df, map = MAP_PRIVATE, modo = O_RDONLY;
    struct stat s;
    void *p;

    if (protection & PROT_WRITE)
        modo = O_RDWR;
    if (stat(fichero, &s) == -1 || (df = open(fichero, modo)) == -1)
        return NULL;
    if ((p = mmap(NULL, s.st_size, protection, map, df, 0)) == MAP_FAILED)
        return NULL;
    insertarEnMemoria(&lista, p, s.st_size, "mmap", 0, df, fichero);
    return p;
}

void sharedDelkey(char *args[])
{
    key_t clave;
    int id;
    char *key = args[2];

    if (key == NULL || (clave = (key_t)strtoul(key, NULL, 10)) == IPC_PRIVATE)
    {
        printf("      delkey necesita clave_valida\n");
        return;
    }
    if ((id = shmget(clave, 0, 0666)) == -1)
    {
        perror("shmget: imposible obtener memoria compartida");
        return;
    }
    if (shmctl(id, IPC_RMID, NULL) == -1)
        perror("shmctl: imposible eliminar id de memoria compartida\n");
}

/*ssize_t escribirFichero(char *f, void *p, size_t cont, int overwrite){
    ssize_t  n;
    int df,aux, flags=O_CREAT | O_EXCL | O_WRONLY;


    if (overwrite)
        flags=O_CREAT | O_WRONLY | O_TRUNC;

    if ((df=open(f,flags,0777))==-1)
        return -1;

    if ((n=write(df,p,cont))==-1){
        aux=errno;
        close(df);
        errno=aux;
        return -1;
    }
    close (df);
    return n;
}*/

ssize_t leerFichero(char *f, void *p, size_t cont)
{
    struct stat s;
    ssize_t n;
    int df, aux;

    if (stat(f, &s) == -1 || (df = open(f, O_RDONLY)) == -1)
        return -1;
    if (cont == -1) /* si pasamos -1 como bytes a leer lo leemos entero*/
        cont = s.st_size;
    if ((n = read(df, p, cont)) == -1)
    {
        aux = errno;
        close(df);
        errno = aux;
        return -1;
    }
    close(df);
    return n;
}

void do_MemPmap(void)
{
    pid_t pid;
    char elpid[32];
    char *argv[4] = {"pmap", elpid, NULL};

    sprintf(elpid, "%d", (int)getpid());
    if ((pid = fork()) == -1)
    {
        perror("Imposible crear proceso");
        return;
    }
    if (pid == 0)
    { /*proceso hijo*/
        if (execvp(argv[0], argv) == -1)
            perror("cannot execute pmap (linux, solaris)");

        argv[0] = "vmmap";
        argv[1] = "-interleave";
        argv[2] = elpid;
        argv[3] = NULL;
        if (execvp(argv[0], argv) == -1) /*probamos vmmap Mac-OS*/
            perror("cannot execute vmmap (Mac-OS)");

        argv[0] = "procstat";
        argv[1] = "vm";
        argv[2] = elpid;
        argv[3] = NULL;
        if (execvp(argv[0], argv) == -1) /*No hay pmap, probamos procstat FreeBSD */
            perror("cannot execute procstat (FreeBSD)");

        argv[0] = "procmap", argv[1] = elpid;
        argv[2] = NULL;
        if (execvp(argv[0], argv) == -1) /*probamos procmap OpenBSD*/
            perror("cannot execute procmap (OpenBSD)");

        exit(1);
    }
    waitpid(pid, NULL, 0);
}

void cmdMalloc(char *trozos[], informacionMemoria *lista)
{
    size_t tamano;
    key_t clave;
    informacionMemoria *temp = NULL;
    bool comprobar = false;

    temp = lista;
    if (trozos[1] == NULL)
    {
        imprimirLista(lista, trozos[0]);
    }
    else if (strcmp(trozos[0], "malloc") == 0)
    {
        if (strcmp(trozos[1], "-free") == 0)
        {
            if (trozos[2] != NULL)
            {
                tamano = strtol(trozos[2], NULL, 10);
                if (tamano == 0)
                {
                    printf("No se pueden liberar bloques de 0 bytes\n");
                }
                else
                {
                    while (temp != NULL)
                    {
                        if (temp->tamano == tamano)
                        {
                            if (temp->direccion == NULL)
                            {
                                printf("Error: no se pudo liberar el bloque\n");
                                return;
                            }
                            else
                            {
                                comprobar = true;
                                borrarDeMemoria(&lista, temp->direccion);
                                free(temp->direccion);
                                break;
                            }
                        }
                        temp = temp->siguiente;
                    }
                    if (!comprobar)
                    {
                        printf("No se ha podido encontrar un bloque con esas caracteristicas\n");
                    }
                }
            }
            else
            {
                printf("No se han proporcionado los argumentos necesarios\n");
            }
        }
        else
        {
            tamano = strtol(trozos[1], NULL, 10);
            temp->tamano = tamano;
            if (tamano <= 0)
            {
                return;
            }
            temp->direccion = malloc(tamano);
            strcpy(temp->tipoDeAsignacion, "malloc");
            if (temp->direccion == NULL)
            {
                printf("No se ha podido crear la direccion\n");
            }
            else
            {
                insertarEnMemoria(&lista, temp->direccion, temp->tamano, temp->tipoDeAsignacion, temp->info.clave, temp->info.mmapInfo.descriptorFichero, temp->info.mmapInfo.nombreFichero);
                printf("Se han insertado %zu bytes en %p\n", temp->tamano, temp->direccion);
            }
        }
    }
    else if (strcmp(trozos[0], "shared") == 0)
    {
        if (strcmp(trozos[1], "-free") == 0)
        {
            if (trozos[2] != NULL)
            {
                clave = (key_t)atoi(trozos[2]);
                while (temp != NULL)
                {
                    if (strcmp(temp->tipoDeAsignacion, "shared") == 0)
                    {
                        if (temp->info.clave == clave)
                        {
                            if ((shmget(temp->info.clave, 0, 0)) != -1)
                            {
                                if (shmdt(temp->direccion) == -1)
                                {
                                    perror("Error al intentar liberar el bloque");
                                    return;
                                }
                            }
                            borrarDeMemoria(&lista, temp->direccion);
                            break;
                        }
                    }
                    temp = temp->siguiente;
                }
            }
            else
            {
                printf("No se han proporcionado los argumentos necesarios\n");
            }
        }
        else if (strcmp(trozos[1], "-create") == 0)
        {
            sharedCreate(trozos, lista);
        }
        else if (strcmp(trozos[1], "-delkey") == 0)
        {
            sharedDelkey(trozos);
        }
        else if (trozos[2] != NULL)
        {
            obtenerMemoriaShmget((key_t)strtoul(trozos[1], NULL, 10), 0, lista);
        }
    }
    else if (strcmp(trozos[0], "mmap") == 0)
    {
        if (strcmp(trozos[1], "-free") == 0)
        {
            while (temp != NULL)
            {
                if (temp->info.mmapInfo.nombreFichero)
                {
                    comprobar = true;
                    borrarDeMemoria(&lista, temp->direccion);
                    break;
                }
                temp = temp->siguiente;
            }
            if (!comprobar)
            {
                printf("No se ha podido encontrar el fichero a mapear\n");
            }
        }
        else
        {
            cmdMmap(trozos, lista);
        }
    }
}

void cmdMmap(char *arg[], informacionMemoria *lista)
{
    char *perm;
    void *p;
    int protection = 0;

    if (arg[1] == NULL)
    {
        imprimirLista(lista, "mmap");
        return;
    }
    if ((perm = arg[2]) != NULL && strlen(perm) < 4)
    {
        if (strchr(perm, 'r') != NULL)
            protection |= PROT_READ;
        if (strchr(perm, 'w') != NULL)
            protection |= PROT_WRITE;
        if (strchr(perm, 'x') != NULL)
            protection |= PROT_EXEC;
    }
    if ((p = mapearFichero(arg[1], protection, lista)) == NULL)
        perror("Imposible mapear fichero");
    else
        printf("fichero %s mapeado en %p\n", arg[0], p);
}
void cmdRead(char *ar[])
{
    void *p;
    size_t cont = -1; /* -1 indica leer todo el fichero*/
    ssize_t n;

    if (ar[1] == NULL || ar[2] == NULL)
    {
        printf("faltan parametros\n");
        return;
    }
    p = cadtop(ar[2]); /*convertimos de cadena a puntero*/
    if (ar[3] != NULL)
        cont = (size_t)atoll(ar[3]);

    if ((n = leerFichero(ar[1], p, cont)) == -1)
        perror("Imposible leer fichero");
    else
        printf("leidos %lld bytes de %s en %p\n", (long long)n, ar[1], p);
}
void cmdWrite(char *trozos[])
{
    int bytes, flags = 0, df;
    char *dir, *nombre;
    if (trozos[1] != NULL)
    {
        if (strcmp(trozos[1], "-o") == 0)
        {
            if (trozos[2] != NULL && trozos[3] != NULL && trozos[4] != NULL)
            {
                nombre = trozos[2];
                bytes = atoi(trozos[4]);
                dir = (char *)strtoul(trozos[3], NULL, 16);
                flags = O_WRONLY | O_CREAT | O_TRUNC;
            }
            else
            {
                printf("Faltan argumentos\n");
            }
        }
        else if (trozos[1] != NULL && trozos[2] != NULL && trozos[3] != NULL)
        {
            nombre = trozos[1];
            bytes = atoi(trozos[3]);
            dir = (char *)strtoul(trozos[2], NULL, 16);
            flags = O_WRONLY | O_CREAT | O_EXCL;
        }
        else
        {
            printf("Faltan argumentos\n");
        }
        if ((df = open(nombre, flags, S_IRWXU | S_IRWXG | S_IRWXO)) == -1)
        {
            perror("Error al abrir el archivo");
            return;
        }
        if (write(df, dir, bytes) != -1)
        {
            printf("Escritos %d bytes en %s desde %p\n", bytes, nombre, dir);
        }
        close(df);
    }
}
void cmdMemDump(char *trozos[])
{
    char *p;
    int n = 25;

    if (trozos[1] == NULL)
    {
        return;
    }
    p = (char *)strtol(trozos[1], NULL, 16);
    if (trozos[2] != NULL)
    {
        n = (int)strtol(trozos[2], NULL, 10);
    }
    imprimirMemDump(p, n);
}
void cmdMemFill(char *trozos[])
{
    void *p;
    int caracter;
    unsigned char defecto = 'A';
    size_t n = 128;
    if (trozos[1] == NULL)
    {
        return;
    }
    p = (void *)strtol(trozos[1], NULL, 16);
    if (trozos[2] != NULL)
    {
        if ((n = atoi(trozos[2])) != 0 && trozos[3] == NULL)
        {
            llenarMemoria(p, n, defecto);
            return;
        }
        caracter = 2;
        n = 128;
    }
    if (trozos[2] != NULL && trozos[3] != NULL)
    {
        n = atoi(trozos[2]);
        defecto = atoi(trozos[3]);
        printf("%c", defecto);
        caracter = 3;
    }
    if (trozos[caracter][1] == 'x')
    {
        defecto = (unsigned char)strtol(trozos[caracter], NULL, 16);
    }
    llenarMemoria(p, n, defecto);
}
void cmdMem(char *trozos[], informacionMemoria *lista)
{

    bool blocks = false, funcs = false, vars = false, pmap = false;
    int intLocal = 0;
    char charLocal = 'r';
    double floatLocal = 1.2;
    static int intSlocal = 0;
    static char charSlocal = 'r';
    static double floatSlocal = 1.2;
    int intLocalNI;
    char charLocalNI;
    double floatLocalNI;
    static int intSlocalNI;
    static char charSlocalNI;
    static double floatSlocalNI;

    if (trozos[1] == NULL || strcmp(trozos[1], "-all") == 0)
    {
        blocks = true;
        funcs = true;
        vars = true;
    }
    else if (strcmp(trozos[1], "-blocks") == 0)
    {

        blocks = true;
    }
    else if (strcmp(trozos[1], "-funcs") == 0)
    {
        funcs = true;
    }
    else if (strcmp(trozos[1], "-vars") == 0)
    {
        vars = true;
    }
    else if (strcmp(trozos[1], "-pmap") == 0)
    {
        pmap = true;
    }
    if (vars)
    {
        printf("Variables globales\t%p,\t%p,\t%p\n", &intGlobal, &charGlobal, &floatGlobal);
        printf("Variables locales\t%p,\t%p,\t%p\n", &intLocal, &charLocal, &floatLocal);
        printf("Variables locales(NI)\t%p,\t%p,\t%p\n", &intLocalNI, &charLocalNI, &floatLocalNI);
        printf("Variables estaticas\t%p,\t%p,\t%p\n", &intSlocal, &charSlocal, &floatSlocal);
        printf("Variables estaticas(NI)\t%p,\t%p,\t%p\n", &intSlocalNI, &charSlocalNI, &floatSlocalNI);
    }
    if (funcs)
    {
        printf("Funciones programa\t%p,\t%p,\t%p\n", &cmdChdir, &cmdDate, &cmdAuthors);
        printf("Funciones libreria\t%p,\t%p,\t%p\n", &printf, &malloc, &perror);
    }
    if (blocks)
    {
        imprimirLista(lista, "todos");
    }
    if (pmap)
    {
        do_MemPmap();
    }
    vars = false;
    funcs = false;
    blocks = false;
    pmap = false;
}
void cmdRecurse(int n)
{
    char automatico[TAMANO];
    static char estatico[TAMANO];

    printf("parametro:%3d(%p) array %p, arr estatico %p\n", n, &n, automatico, estatico);
    if (n > 0)
        cmdRecurse(n - 1);
}
void *cadtop(const char *cadena)
{
    void *puntero;
    sscanf(cadena, "%p", &puntero);
    return puntero;
}
void imprimirMemDump(char *p, int n)
{
    int i, j;
    int pos_char;
    int veces = 0, lineas = 0;

    for (i = 0; i < n; i++)
    {
        imprimirChar(p[i]);
        veces++;

        if ((veces == n - 25 * lineas) || (veces == 25))
        {
            printf("\n");
            pos_char = i + 1 - veces;

            for (j = pos_char; j < pos_char + veces; ++j)
            {
                imprimirHex(p[j]);
            }
            printf("\n");
            lineas++;
            veces = 0;
        }
    }
}

void imprimirChar(char c)
{
    bool imprimir = false;
    if (c > 32 && c < 126)
        imprimir = true;
    else
        imprimir = false;

    if (imprimir)
        printf("%4c", c);
    else
        printf("%4c", ' ');

    imprimir = false;
}
void imprimirHex(char c)
{
    bool imprimir = false;
    if (c >= 32 && c <= 126)
        imprimir = true;
    else
        imprimir = false;

    if (imprimir)
        printf("%4x", c);
    else
        printf("%3c%c", '0', '0');
    imprimir = false;
}