#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Constantes
#define N 10          // Tamaño de la red (N x N)
#define ITERACIONES 100000 // Número de iteraciones
#define T 0.0001  // Temperatura en K
#define K_BOLTZMANN 1.380649e-23// Constante de Boltzmann (J/K)

// Función para inicializar la red con espines aleatorios (+1 o -1)
//rand() % 2 genera números aleatorios entre 0 y 1 
void inicializarRed(int red[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            red[i][j] = (rand() % 2) * 2 - 1; // Genera +1 o -1
        }
    }
}

// Función para guardar la red en un archivo
void guardarRed(FILE *archivo, int red[N][N]) {
    int i; 
    int j;
    for (i = 0; i < N; i++) {
        for ( j = 0; j < N; j++) {
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
    int i; 
    int n, m; 
    int suma_vecinos;
    double deltaE; 
    double probabilidad; 
    double r; 

    FILE *archivo = fopen( "matriz_red.txt", "w");
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
            arriba = red[N - 1][m]; // Si está en la primera fila, el vecino de arriba está en la última fila
        } else {
            arriba = red[n - 1][m];
        }

        // Vecino de abajo
        if (n == N - 1) {
            abajo = red[0][m]; // Si está en la última fila, el vecino de abajo está en la primera fila
        } else {
            abajo = red[n + 1][m];
        }

        // Vecino de la izquierda
        if (m == 0) {
            izquierda = red[n][N - 1]; // Si está en la primera columna, el vecino de la izquierda está en la última columna
        } else {
            izquierda = red[n][m - 1];
        }

        // Vecino de la derecha
        if (m == N - 1) {
            derecha = red[n][0]; // Si está en la última columna, el vecino de la derecha está en la primera columna
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
    int i;
    int j;
    for ( i = 0; i < N; i++) {
        for ( j = 0; j < N; j++) {
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

    // Inicializar la red con espines aleatorios
    inicializarRed(red);

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