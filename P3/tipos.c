/*
 *Jesus Jose Santamaria Santos: j.j.ssantos
 */

#include "tipos.h"

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
int insertarEnMemoria(informacionMemoria **lista, void *direccion, size_t tamano, const char *tipoDeAsignacion, key_t clave, int descriptorFichero, const char *nombreFichero)
{
    informacionMemoria *nuevoNodo = malloc(sizeof(informacionMemoria));
    if (nuevoNodo == NULL)
    {
        perror("Error al asignar memoria para el nuevo nodo");
        return -1;
    }
    nuevoNodo->direccion = direccion;
    nuevoNodo->tamano = tamano;
    nuevoNodo->tiempo = time(NULL);
    strcpy(nuevoNodo->tipoDeAsignacion, tipoDeAsignacion);
    if (strcmp(tipoDeAsignacion, "shared") == 0)
    {
        nuevoNodo->info.clave = clave;
    }
    else if (strcmp(tipoDeAsignacion, "mmap") == 0)
    {
        nuevoNodo->info.mmapInfo.nombreFichero = strdup(nombreFichero);
        if (nuevoNodo->info.mmapInfo.nombreFichero == NULL)
        {
            perror("Error al asignar memoria para el nombre del archivo");
            free(nuevoNodo->info.mmapInfo.nombreFichero);
            return -1;
        }
        nuevoNodo->info.mmapInfo.descriptorFichero = descriptorFichero;
    }
    nuevoNodo->siguiente = NULL;
    if (*lista == NULL)
    {
        *lista = nuevoNodo;
    }
    else
    {
        informacionMemoria *temp = *lista;
        while (temp->siguiente != NULL)
        {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevoNodo;
    }

    return 0;
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
void borrarDeMemoria(informacionMemoria **lista, void *direccion)
{
    if (*lista == NULL)
    {
        return;
    }

    informacionMemoria *temp = *lista;
    informacionMemoria *anterior = NULL;

    while (temp != NULL && temp->direccion != direccion)
    {
        anterior = temp;
        temp = temp->siguiente;
    }

    if (temp == NULL)
    {
        return;
    }
    else if (anterior == NULL)
    {
        *lista = temp->siguiente;
    }
    else
    {
        anterior->siguiente = temp->siguiente;
    }
    if (strcmp(temp->tipoDeAsignacion, "mmap") == 0)
    {
        if (munmap(temp->direccion, temp->tamano) == -1)
        {
            perror("Error al desmapear la memoria mapeada");
        }
    }
}
void eliminarMemoria(informacionMemoria **lista)
{
    informacionMemoria *actual = *lista;
    informacionMemoria *siguiente = NULL;

    while (actual != NULL)
    {
        siguiente = actual->siguiente;
        if (strcmp(actual->tipoDeAsignacion, "mmap") == 0)
        {
            free(actual->info.mmapInfo.nombreFichero);
        }
        actual = siguiente;
    }
    *lista = NULL;
}
void imprimirLista(informacionMemoria *lista, char *asignacion)
{
    informacionMemoria *actual = lista;

    printf("Lista de Información de Memoria:\n");
    printf("******* Lista de bloques asignados %s para el proceso %d\n", asignacion, getpid());
    while (actual != NULL && actual->direccion != NULL)
    {
        struct tm *tiempoInfo = localtime(&(actual->tiempo));
        if (strcmp(actual->tipoDeAsignacion, asignacion) == 0 || strcmp(asignacion, "todos") == 0)
        {
            printf("Dirección: %p, Tamaño: %zu, Tipo: %s, Tiempo: %02d:%02d\n", actual->direccion, actual->tamano, actual->tipoDeAsignacion, tiempoInfo->tm_hour, tiempoInfo->tm_min);
            if (strcmp(actual->tipoDeAsignacion, "shared") == 0)
            {
                printf("Clave: %d\n", actual->info.clave);
            }
            else if (strcmp(actual->tipoDeAsignacion, "mmap") == 0)
            {
                printf("Nombre del Fichero: %s, Descriptor de Archivo: %d\n", actual->info.mmapInfo.nombreFichero, actual->info.mmapInfo.descriptorFichero);
            }
        }
        actual = actual->siguiente;
    }
}

struct SEN
{
    char *nombre;
    int senal;
};

static struct SEN sigstrnum[] = {
    {"HUP", SIGHUP},
    {"INT", SIGINT},
    {"QUIT", SIGQUIT},
    {"ILL", SIGILL},
    {"TRAP", SIGTRAP},
    {"ABRT", SIGABRT},
    {"IOT", SIGIOT},
    {"BUS", SIGBUS},
    {"FPE", SIGFPE},
    {"KILL", SIGKILL},
    {"USR1", SIGUSR1},
    {"SEGV", SIGSEGV},
    {"USR2", SIGUSR2},
    {"PIPE", SIGPIPE},
    {"ALRM", SIGALRM},
    {"TERM", SIGTERM},
    {"CHLD", SIGCHLD},
    {"CONT", SIGCONT},
    {"STOP", SIGSTOP},
    {"TSTP", SIGTSTP},
    {"TTIN", SIGTTIN},
    {"TTOU", SIGTTOU},
    {"URG", SIGURG},
    {"XCPU", SIGXCPU},
    {"XFSZ", SIGXFSZ},
    {"VTALRM", SIGVTALRM},
    {"PROF", SIGPROF},
    {"WINCH", SIGWINCH},
    {"IO", SIGIO},
    {"SYS", SIGSYS},
/*senales que no hay en todas partes*/
#ifdef SIGPOLL
    {"POLL", SIGPOLL},
#endif
#ifdef SIGPWR
    {"PWR", SIGPWR},
#endif
#ifdef SIGEMT
    {"EMT", SIGEMT},
#endif
#ifdef SIGINFO
    {"INFO", SIGINFO},
#endif
#ifdef SIGSTKFLT
    {"STKFLT", SIGSTKFLT},
#endif
#ifdef SIGCLD
    {"CLD", SIGCLD},
#endif
#ifdef SIGLOST
    {"LOST", SIGLOST},
#endif
#ifdef SIGCANCEL
    {"CANCEL", SIGCANCEL},
#endif
#ifdef SIGTHAW
    {"THAW", SIGTHAW},
#endif
#ifdef SIGFREEZE
    {"FREEZE", SIGFREEZE},
#endif
#ifdef SIGLWP
    {"LWP", SIGLWP},
#endif
#ifdef SIGWAITING
    {"WAITING", SIGWAITING},
#endif
    {NULL, -1},
}; /*fin array sigstrnum */

int senal(char *senal)
{
    int temp;

    for (temp = 0; sigstrnum[temp].nombre != NULL; temp++)
    {
        if (!strcmp(senal, sigstrnum[temp].nombre))
        {
            return sigstrnum[temp].senal;
        }
    }
    return -1;
}

char *nombreSenal(int senal)
{
    int temp;

    for (temp = 0; sigstrnum[temp].nombre != NULL; temp++)
    {
        if (senal == sigstrnum[temp].senal)
        {
            return sigstrnum[temp].nombre;
        }
    }
    return ("SIGUNKNOWN");
}

static struct nodo *createNodo()
{
    struct nodo *temp = malloc(sizeof(struct nodo));

    if (temp == NULL)
    {
        printf("memoria agotada\n");
        exit(EXIT_FAILURE);
    }
    return temp;
}

lProcesos createlProcesos()
{
    struct nodo *lista = createNodo();

    lista->siguiente = NULL;
    return lista;
}

int emptylist(lProcesos lista)
{
    return (lista->siguiente == NULL);
}

void insertProceso(informacionProcesos dp, pProcesos p)
{
    struct nodo *temp = createNodo();

    temp->infoProcesos = dp;
    temp->siguiente = p->siguiente;
    p->siguiente = temp;
}

pProcesos findProceso(lProcesos lista, pid_t pid)
{
    struct nodo *temp = lista->siguiente;

    while (temp != NULL && temp->infoProcesos.pid != pid)
    {
        temp = temp->siguiente;
    }
    return temp;
}

void deleteProceso(lProcesos lista, pProcesos p)
{
    struct nodo *temp;

    temp = lista;
    while (temp->siguiente != p && temp->siguiente != NULL)
    {
        temp = temp->siguiente;
    }
    temp->siguiente = p->siguiente;
    free(p);
}

pProcesos firstProceso(lProcesos lista)
{
    return lista->siguiente;
}

pProcesos nextProceso(pProcesos p)
{
    return p->siguiente;
}

int endlProcesos(pProcesos p)
{
    return (p == NULL);
}

informacionProcesos elemento(pProcesos p)
{
    return p->infoProcesos;
}

void modElemento(pProcesos p)
{
    int estado;

    if (getpriority(PRIO_PROCESS, p->infoProcesos.pid) != 1)
    {
        p->infoProcesos.prioridad = getpriority(PRIO_PROCESS, p->infoProcesos.pid);
    }
    if (waitpid(p->infoProcesos.pid, &estado, WNOHANG | WUNTRACED | WCONTINUED) == p->infoProcesos.pid)
    {

        if (WIFEXITED(estado))
        {
            strcpy(p->infoProcesos.estado, "Terminated Normally");
            return;
        }
        if (WIFCONTINUED(estado))
        {
            strcpy(p->infoProcesos.estado, "Running");
            return;
        }
        if (WIFSTOPPED(estado))
        {
            strcpy(p->infoProcesos.estado, "Stopped");
            p->infoProcesos.senal = nombreSenal(WIFSTOPPED(estado));
            return;
        }
        if (WTERMSIG(estado))
        {
            strcpy(p->infoProcesos.estado, "Terminated by signal");
            p->infoProcesos.senal = nombreSenal(WTERMSIG(estado));
            return;
        }
    }
}
void eliminarListaProcesos(lProcesos lista)
{
    pProcesos actual = lista;
    pProcesos siguiente;

    while (actual != NULL)
    {
        siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
}
