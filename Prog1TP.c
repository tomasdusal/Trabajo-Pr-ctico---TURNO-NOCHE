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
