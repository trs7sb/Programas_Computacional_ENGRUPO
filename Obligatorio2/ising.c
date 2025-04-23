#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Constantes
#define N 10          // Tamaño de la red (N x N)
#define ITERACIONES 100000 // Número de iteraciones
#define T 0.0001  
#define K_BOLTZMANN 1.0 // Constante de Boltzmann (J/K)

// Función para inicializar la red con espines aleatorios (+1 o -1)
void inicializarRed(int red[N][N], int sesgo) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            // Generar un número aleatorio entre 0 y 99
            int probabilidad = rand() % 100;
            // Asignar +1 si está dentro del porcentaje de sesgo, -1 en caso contrario
            if (probabilidad < sesgo) {
                red[i][j] = 1;
            } else {
                red[i][j] = -1;
            }
        }
    }
}

// Función para inicializar la red de forma ordenada (+1 o -1)
void inicializarRedOrdenada(int red[N][N], int valor) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            red[i][j] = valor; // Asigna el valor +1 o -1 a toda la red
        }
    }
}

// Función para guardar la red en un archivo
void guardarRed(FILE *archivo, int red[N][N]) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            fprintf(archivo, "%d", red[i][j]);
            if (j < N - 1) {
                fprintf(archivo, ","); // Separador entre columnas
            }
        }
        fprintf(archivo, "\n"); // Nueva línea al final de cada fila
    }
    fprintf(archivo, "\n"); // Línea en blanco para separar iteraciones
}

// Algoritmo de Monte Carlo para el modelo de Ising
void monteCarloIsing(int red[N][N], double beta, int iteraciones) {
    int i, n, m, suma_vecinos;
    double deltaE, probabilidad, r;

    FILE *archivo = fopen("matriz_red.txt", "w");
    if (archivo == NULL) {
        fprintf(stderr, "Error al abrir el archivo para guardar la red.\n");
        exit(1);
    }

    for (i = 0; i < iteraciones; i++) {
        // Elegir un espín aleatorio en la red
        n = rand() % N;
        m = rand() % N;

        // Calcular los vecinos con condiciones de contorno periódicas
        int arriba, abajo, izquierda, derecha;

        // Vecino de arriba
        if (n == 0) {
            arriba = red[N - 1][m]; // Si está en el borde superior, conecta con el último
        } else {
            arriba = red[n - 1][m];
        }

        // Vecino de abajo
        if (n == N - 1) {
            abajo = red[0][m]; // Si está en el borde inferior, conecta con el primero
        } else {
            abajo = red[n + 1][m];
        }

        // Vecino de la izquierda
        if (m == 0) {
            izquierda = red[n][N - 1]; // Si está en el borde izquierdo, conecta con el último
        } else {
            izquierda = red[n][m - 1];
        }

        // Vecino de la derecha
        if (m == N - 1) {
            derecha = red[n][0]; // Si está en el borde derecho, conecta con el primero
        } else {
            derecha = red[n][m + 1];
        }

        // Calcular el cambio de energía cuando se invierte el espín
        suma_vecinos = arriba + abajo + izquierda + derecha;
        deltaE = 2 * red[n][m] * suma_vecinos;

        // Calcular la probabilidad de transición
        probabilidad = exp(-beta * deltaE);
        if (probabilidad > 1.0) {
            probabilidad = 1.0;
        }

        // Generar un número aleatorio con probabilidad uniforme entre 0 y 1 para decidir si aceptar el cambio
        r = (double)rand() / RAND_MAX;
        if (r < probabilidad) {
            red[n][m] *= -1; // Si se acepta el cambio, invertir el signo del espín
        }

        // Guardar la red en el archivo después de cada iteración
        guardarRed(archivo, red);
    }

    fclose(archivo);
}

// Función para imprimir la red
void imprimirRed(int red[N][N]) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%2d ", red[i][j]);
        }
        printf("\n");
    }
}

int main() {
    double beta = 1.0 / (K_BOLTZMANN * T); // Beta = 1 / (k_B * T)

    // Inicializa la semilla de números aleatorios
    srand(time(NULL));

    int red[N][N];
    int opcion, sesgo;

    // Solicitar al usuario cómo inicializar la red
    printf("Seleccione cómo inicializar la red:\n");
    printf("1. Aleatoria\n");
    printf("2. Con sesgo de espines paralelos o antiparalelos\n");
    printf("Ingrese su opción (1 o 2): ");
    scanf("%d", &opcion);

    if (opcion == 1) {
        // Inicializar la red de forma completamente aleatoria
        inicializarRed(red, 50); // 50% de probabilidad para +1 o -1
    } else if (opcion == 2) {
        // Solicitar el porcentaje de sesgo
        printf("Ingrese el porcentaje de sesgo para espines paralelos (0-100): ");
        scanf("%d", &sesgo);
        if (sesgo < 0 || sesgo > 100) {
            printf("Porcentaje inválido. Usando 50%% por defecto.\n");
            sesgo = 50;
        }
        inicializarRed(red, sesgo);
    } else {
        printf("Opción no válida. Inicializando de forma aleatoria por defecto.\n");
        inicializarRed(red, 50);
    }

    // Imprimir la configuración inicial
    printf("Configuración inicial de la red:\n");
    imprimirRed(red);

    // Ejecutar el algoritmo de Monte Carlo
    monteCarloIsing(red, beta, ITERACIONES);

    // Imprimir la configuración final
    printf("\nConfiguración final de la red:\n");
    imprimirRed(red);

    return 0;
}