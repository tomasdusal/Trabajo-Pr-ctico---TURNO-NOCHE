#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ARCHIVO "torneos.txt"
#define ARCHIVO_FINALIZADOS "torneos_finalizados.txt"
#define ARCHIVO_EN_CURSO "torneos_en_curso.txt"
#define MAX_NOMBRE 55
#define MAX_DEPORTE 55
#define MIN_EQUIPOS 2
#define MAX_EQUIPOS 999
#define PREMIO_MAX 9999999999.99
#define TAM_LINEA 128

typedef struct {
    char nombre[MAX_NOMBRE + 1];
    char deporte[MAX_DEPORTE + 1];
    int cantidadEquipos;
    double premio;
    int terminado; // 1 si finalizo, 0 si esta en curso
} torneo;

int menu(void);
void leerLinea(char *destino, int tamanio);
void leerCadena(char mensaje[], char destino[], int maximo);
int leerNumero(char mensaje[], int minimo, int maximo);
double leerNumeroReal(char mensaje[], double minimo, double maximo);
int leerSiNo(char mensaje[]);
void pedirTorneo(torneo *t);
void mostrarTorneo(torneo t);
int leerTorneo(FILE *archivo, torneo *t);
void escribirTorneo(FILE *archivo, torneo t);
void agregarTorneo(void);
void listarTorneos(int filtrar, double premioMinimo);
void dividirArchivos(void);

int main(void)
{
    int opcion;
    double premioMinimo;

    do {
        opcion = menu();

        switch (opcion) {
            case 1:
                agregarTorneo();
                break;
            case 2:
                listarTorneos(0, 0);
                break;
            case 3:
                premioMinimo = leerNumeroReal("Premio minimo a listar: ", 0, PREMIO_MAX);
                listarTorneos(1, premioMinimo);
                break;
            case 4:
                dividirArchivos();
                break;
            case 5:
                printf("Fin del programa.\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
    } while (opcion != 5);

    return 0;
}

int menu(void)
{
    printf("\n===== TORNEOS DEPORTIVOS =====\n");
    printf("1. Agregar un torneo\n");
    printf("2. Listar todos los torneos\n");
    printf("3. Listar torneos con premio mayor o igual a un monto\n");
    printf("4. Dividir el archivo en finalizados y en curso\n");
    printf("5. Salir\n");
    return leerNumero("Opcion: ", 1, 5);
}

// Lee una linea completa consumiendo el buffer correctamente
void leerLinea(char *destino, int tamanio)
{
    int largo;
    int caracter;

    if (fgets(destino, tamanio, stdin) == NULL) {
        printf("\nSe cerro la entrada de datos. Fin del programa.\n");
        exit(1);
    }

    largo = (int) strlen(destino);
    if (largo > 0 && destino[largo - 1] == '\n') {
        destino[largo - 1] = '\0';
    } else {
        do {
            caracter = getchar();
        } while (caracter != '\n' && caracter != EOF);
    }
}

// Lee una cadena sin sobrepasar el tamano maximo permitido
void leerCadena(char mensaje[], char destino[], int maximo)
{
    char linea[TAM_LINEA];
    int ok;

    do {
        printf("%s", mensaje);
        leerLinea(linea, TAM_LINEA);
        
        if ((int)strlen(linea) > maximo || strlen(linea) == 0) {
            printf("Error: Ingrese un texto valido (maximo %d caracteres).\n", maximo);
            ok = 0;
        } else {
            ok = 1;
        }
    } while (!ok);

    strcpy(destino, linea);
}

// Lee un entero validando rango e ingresando mediante buffer
int leerNumero(char mensaje[], int minimo, int maximo)
{
    char linea[TAM_LINEA];
    char sobrante;
    int valor = 0;
    int ok;

    do {
        printf("%s", mensaje);
        leerLinea(linea, TAM_LINEA);
        
        ok = sscanf(linea, "%d %c", &valor, &sobrante) == 1 
             && valor >= minimo && valor <= maximo;

        if (!ok) {
            printf("Valor invalido. Intente de nuevo.\n");
        }
    } while (!ok);

    return valor;
}

// Lee un double validando rango e ingresando mediante buffer
double leerNumeroReal(char mensaje[], double minimo, double maximo)
{
    char linea[TAM_LINEA];
    char sobrante;
    double valor = 0;
    int ok;

    do {
        printf("%s", mensaje);
        leerLinea(linea, TAM_LINEA);

        ok = sscanf(linea, "%lf %c", &valor, &sobrante) == 1 
             && valor >= minimo && valor <= maximo;

        if (!ok) {
            printf("Valor invalido. Intente de nuevo.\n");
        }
    } while (!ok);

    return valor;
}

// Lee la respuesta S/N procesando caracteres
int leerSiNo(char mensaje[])
{
    char linea[TAM_LINEA];
    char respuesta = ' ';
    char sobrante;
    int ok;

    do {
        printf("%s", mensaje);
        leerLinea(linea, TAM_LINEA);

        ok = sscanf(linea, " %c %c", &respuesta, &sobrante) == 1;
        respuesta = (char) toupper((unsigned char) respuesta);

        if (!ok || (respuesta != 'S' && respuesta != 'N')) {
            printf("Respuesta invalida. Ingrese S o N.\n");
            ok = 0;
        }
    } while (!ok);

    return respuesta == 'S';
}

void pedirTorneo(torneo *t)
{
    printf("\n--- Nuevo torneo ---\n");
    leerCadena("Nombre: ", t->nombre, MAX_NOMBRE);
    leerCadena("Deporte: ", t->deporte, MAX_DEPORTE);
    t->cantidadEquipos = leerNumero("Cantidad de equipos: ", MIN_EQUIPOS, MAX_EQUIPOS);
    t->premio = leerNumeroReal("Premio: ", 0, PREMIO_MAX);
    t->terminado = leerSiNo("Ya finalizo? (S/N): ");
}

void mostrarTorneo(torneo t)
{
    printf("%-20s %-15s %5d %12.2f  %s\n", t.nombre, t.deporte,
           t.cantidadEquipos, t.premio,
           t.terminado == 1 ? "Finalizado" : "En curso");
}

int leerTorneo(FILE *archivo, torneo *t)
{
    return fscanf(archivo, "%s %s %d %lf %d", t->nombre, t->deporte,
                  &t->cantidadEquipos, &t->premio, &t->terminado) == 5;
}

void escribirTorneo(FILE *archivo, torneo t)
{
    fprintf(archivo, "%s %s %d %.2f %d\n", t.nombre, t.deporte,
            t.cantidadEquipos, t.premio, t.terminado);
}

void agregarTorneo(void)
{
    torneo t;
    FILE *archivo;

    pedirTorneo(&t);

    archivo = fopen(ARCHIVO, "a");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    escribirTorneo(archivo, t);
    fclose(archivo);
    printf("Torneo agregado con exito.\n");
}

void listarTorneos(int filtrar, double premioMinimo)
{
    torneo t;
    int total = 0;
    FILE *archivo = fopen(ARCHIVO, "r");

    if (archivo == NULL) {
        printf("Todavia no hay torneos guardados.\n");
        return;
    }

    printf("\n%-20s %-15s %5s %12s  %s\n",
           "NOMBRE", "DEPORTE", "EQUIPOS", "PREMIO", "ESTADO");

    while (leerTorneo(archivo, &t)) {
        if (!filtrar || t.premio >= premioMinimo) {
            mostrarTorneo(t);
            total++;
        }
    }

    fclose(archivo);

    if (total == 0) {
        printf("No se encontro ningun torneo.\n");
    } else {
        printf("\nTotal: %d torneo(s)\n", total);
    }
}

void dividirArchivos(void)
{
    torneo t;
    int contFin = 0, contCurso = 0;
    FILE *origen = fopen(ARCHIVO, "r");
    FILE *fin, *curso;

    if (origen == NULL) {
        printf("Todavia no hay torneos guardados.\n");
        return;
    }

    fin = fopen(ARCHIVO_FINALIZADOS, "w");
    curso = fopen(ARCHIVO_EN_CURSO, "w");

    if (fin == NULL || curso == NULL) {
        printf("No se pudieron crear los archivos de destino.\n");
        fclose(origen);
        return;
    }

    while (leerTorneo(origen, &t)) {
        if (t.terminado == 1) {
            escribirTorneo(fin, t);
            contFin++;
        } else {
            escribirTorneo(curso, t);
            contCurso++;
        }
    }

    fclose(origen);
    fclose(fin);
    fclose(curso);

    printf("\n%d torneo(s) finalizado(s) -> %s\n", contFin, ARCHIVO_FINALIZADOS);
    printf("%d torneo(s) en curso -> %s\n", contCurso, ARCHIVO_EN_CURSO);
}
