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
#include <stdbool.h>

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

void cmdCommand(char *argumentos, historial *historial1, informacionFichero *ficherosAbiertos)
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
    else
    {
        printf("Error: comando no reconocido\n");
    }
}

void cmdSalida()
{
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

char LetraTF(mode_t m)
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

char *ConvierteModo2(mode_t m)
{
    static char permisos[12];
    strcpy(permisos, "---------- ");

    permisos[0] = LetraTF(m);
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
        modo = ConvierteModo2(temp.st_mode);
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
