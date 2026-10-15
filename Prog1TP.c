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