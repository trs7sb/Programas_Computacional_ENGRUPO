#include <stdio.h>
#include <stdlib.h>
#include <math.h> // Required for log() and sqrt() functions
#include <time.h>

// Constantes
#define N 200 // Tamaño de la red (N x N)
#define pasosmontecarlo 100 
#define T 3.0
#define K_BOLTZMANN 1.0 // Constante de Boltzmann (J/K)

// Función para inicializar la red con espines aleatorios (+1 o -1)
void inicializarRed(int red[N][N], int sesgo) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            // Generar un número aleatorio entre 0 y 99 (ambos incluidos)
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

// Función para calcular la energía total del sistema
double calcularEnergia(int red[N][N]) {
    double energia = 0.0;
    int i, j;
    int arriba, abajo, izquierda, derecha;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            // Condiciones de contorno periódicas
            // Vecino de arriba
            if (i == 0) {
                arriba = red[N - 1][j]; // Si está en el borde superior, conecta con el último
            } else {
                arriba = red[i - 1][j];
            }

            // Vecino de abajo
            if (i == N - 1) {
                abajo = red[0][j]; // Si está en el borde inferior, conecta con el primero
            } else {
                abajo = red[i + 1][j];
            }

            // Vecino de la izquierda
            if (j == 0) {
                izquierda = red[i][N - 1]; // Si está en el borde izquierdo, conecta con el último
            } else {
                izquierda = red[i][j - 1];
            }

            // Vecino de la derecha
            if (j == N - 1) {
                derecha = red[i][0]; // Si está en el borde derecho, conecta con el primero
            } else {
                derecha = red[i][j + 1];
            }

            // Sumar las interacciones con los vecinos
            energia += red[i][j] * (arriba + abajo + izquierda + derecha);
        }
    }
    return -energia / 2.0; // Dividir entre 2 para evitar contar interacciones duplicadas
}


// Algoritmo de Monte Carlo para el modelo de Ising
void monteCarloIsing(int red[N][N], double beta) {
    int i, j, n, m, suma_vecinos;
    double deltaE, probabilidad, r;
    double energia_actual;

    // Array circular para almacenar las últimas 6 energías
    double energias[1000] = {0};
    for (int k = 0; k < 1000; k++) {
        energias[k] = calcularEnergia(red);
    }

    FILE *archivo_red = fopen("matriz_red.txt", "w");
    if (archivo_red == NULL) {
        fprintf(stderr, "Error al abrir el archivo para guardar la red.\n");
        exit(1);
    }

    FILE *archivo_energias = fopen("energias.txt", "w");
    if (archivo_energias == NULL) {
        fprintf(stderr, "Error al abrir el archivo para guardar las energías.\n");
        fclose(archivo_red);
        exit(1);
    }

    for (i = 0; i < pasosmontecarlo; i++) {
        if (i==0){
            guardarRed(archivo_red, red);
        }

        for (int j = 0; j < N * N; j++) {
            // Elegir un espín aleatorio en la red
            n = rand() % N;
            m = rand() % N;

            // Calcular los vecinos con condiciones de contorno periódicas
            int arriba, abajo, izquierda, derecha;

            // Vecino de arriba
            if (n == 0) {
                arriba = red[N - 1][m];
            } else {
                arriba = red[n - 1][m];
            }

            // Vecino de abajo
            if (n == N - 1) {
                abajo = red[0][m];
            } else {
                abajo = red[n + 1][m];
            }

            // Vecino de la izquierda
            if (m == 0) {
                izquierda = red[n][N - 1];
            } else {
                izquierda = red[n][m - 1];
            }

            // Vecino de la derecha
            if (m == N - 1) {
                derecha = red[n][0];
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
        }

        // Guardar la red en el fichero cada paso montecarlo
        guardarRed(archivo_red, red);

        energia_actual = calcularEnergia(red);

        // Guardar la energía en el archivo
        fprintf(archivo_energias, "%d %.6f\n", i, energia_actual);

        // Verificar convergencia: comparar energía actual con la de 10 pasos antes
        if (i >= 1000) {
            if (fabs(energia_actual - energias[i % 1000]) < 2) {
                printf("Convergencia alcanzada en el paso montecarlo %d.\n", i);
                break;
            }
        }
        // Guardar la energía en el array circular
         energias[i % 1000] = energia_actual;
    }

    fclose(archivo_red);
    fclose(archivo_energias);
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
    // Medir el tiempo de inicio
    double beta = 1.0 / (K_BOLTZMANN * T); // Beta = 1 / (k_B * T)

    // Inicializa la semilla de números aleatorios
    srand(time(NULL));

    int red[N][N];
    int opcion;

    // Solicitar al usuario cómo inicializar la red
    printf("Seleccione cómo inicializar la red:\n");
    printf("1. Aleatoria\n");
    printf("2. Ordenada (+1)\n");
    printf("Ingrese su opción (1 o 2): ");
    scanf("%d", &opcion);

    if (opcion == 1) {
        // Inicializar la red de forma completamente aleatoria
        inicializarRed(red, 50); // 50% de probabilidad para +1 o -1
    } else if (opcion == 2) {
        // Inicializar la red de forma ordenada (+1)
        inicializarRedOrdenada(red, 50);
    } else {
        printf("Opción no válida. Inicializando de forma aleatoria por defecto.\n");
        inicializarRed(red, 50);
    }

    clock_t inicio = clock();

    // Imprimir la configuración inicial
    printf("Configuración inicial de la red:\n");
    imprimirRed(red);

    // Ejecutar el algoritmo de Monte Carlo
    monteCarloIsing(red, beta);

    // Imprimir la configuración final
    printf("\nConfiguración final de la red:\n");
    imprimirRed(red);

    // Medir el tiempo de finalización
    clock_t fin = clock();
    double tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;
    printf("\nTiempo de ejecución: %.2f segundos.\n", tiempo);

    return 0;
}