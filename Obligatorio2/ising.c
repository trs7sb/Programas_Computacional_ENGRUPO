#include <stdio.h>
#include <stdlib.h>
#include <math.h> // Required for log() and sqrt() functions
#include <time.h>

// Constantes
<<<<<<< HEAD
#define N 10         // Tamaño de la red (N x N)
#define ITERACIONES 10000// Número de iteraciones
#define T 1.5//T_c=2.0 / log(1.0 + sqrt(2.0))
=======
#define N 200    // Tamaño de la red (N x N)
#define ITERACIONES 100*N^2 // Número de iteraciones
#define T 3.0 
>>>>>>> bd25ce9801706c8d60fa55d84c2754f551a3afc5
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

void calcularCalorEspecifico(double energias[], int num_energias, double *cv) {
    double suma_E = 0.0, suma_E2 = 0.0;
    int i;

    // Calcular la suma de las energías y la suma de los cuadrados de las energías
    for (i = 0; i < num_energias; i++) {
        suma_E += energias[i];
        suma_E2 += energias[i] * energias[i];
    }

      // Calcular los promedios
      double promedio_E = suma_E / num_energias;
      double promedio_E2 = suma_E2 / num_energias;
  
      // Calcular la varianza
      double varianza_E = promedio_E2 - (promedio_E * promedio_E);
  
      // Calcular el calor específico a volumen constante
      *cv = varianza_E / (N*N*T);
}

// Algoritmo de Monte Carlo para el modelo de Ising
void monteCarloIsing(int red[N][N], double beta, int iteraciones,double energias[]) {
    int i, n, m, suma_vecinos;
    double deltaE, probabilidad, r;
    int energia_index = 0;
    double energia_anterior=0.0; // Para almacenar la energía de la iteración anterior
    double energia_actual; // Para almacenar la energía de la iteración actual
    double umbral_convergencia = 1e-6; // Umbral para determinar la convergencia

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
<<<<<<< HEAD

            // Calcular la energía total del sistema y guardarla en el array
            energia_index++;
            energias[energia_index] = calcularEnergia(red);
            energia_actual=energias[energia_index]; 

            // Guardar la red en el archivo después de cada iteración
              guardarRed(archivo, red);

               // Verificar la convergencia
             if (fabs(energia_actual - energia_anterior) < umbral_convergencia) {
                 printf("Convergencia alcanzada en la iteración %d.\n", i + 1);
                break;
             }

            energia_anterior = energia_actual;
        }

        /*
        Para que el algoritmo de Ising salga del bucle for cuando converja, 
        agregamos una condición de parada basada en la estabilidad de la energía del sistema. 
        Esto implica verificar si la energía total del sistema no cambia significativamente entre iteraciones consecutivas. 
        Si la diferencia entre las energías de dos iteraciones consecutivas es menor que un umbral definido (por ejemplo, 1e-6), 
        se puede considerar que el sistema ha convergido.
        */
=======
            // Guardar la red en el archivo si se acepta el cambio
            guardarRed(archivo, red);
        }

>>>>>>> bd25ce9801706c8d60fa55d84c2754f551a3afc5
        
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

    // Medir el tiempo de inicio

    double beta = 1.0 / (K_BOLTZMANN * T); // Beta = 1 / (k_B * T)

    // Inicializa la semilla de números aleatorios
    srand(time(NULL));

    int red[N][N];
    int opcion, sesgo;

        // Array para almacenar las energías
        double energias[ITERACIONES] = {0};

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

    clock_t inicio = clock();

    // Imprimir la configuración inicial
    printf("Configuración inicial de la red:\n");
    imprimirRed(red);

    // Ejecutar el algoritmo de Monte Carlo
    monteCarloIsing(red, beta, ITERACIONES,energias);

    // Imprimir la configuración final
    printf("\nConfiguración final de la red:\n");
    imprimirRed(red);

<<<<<<< HEAD
    FILE *archivo_energias = fopen("energias_vs_iteraciones.txt", "w");
    if (archivo_energias == NULL) {
        fprintf(stderr, "Error al abrir el archivo para guardar las energías.\n");
        exit(1);
    }
    
    int num_energias = 0;
    for (int i = 0; i < ITERACIONES&& energias[i] != 0; i++) {
        num_energias++;
        fprintf(archivo_energias, "%d %.2f\n", i + 1, energias[i]); // Iteración y energía
    }

    // Calcular el calor específico
    double cv = 0.0;
    calcularCalorEspecifico(energias, num_energias, &cv);

    // Imprimir el resultado
    printf("Calor específico a volumen constante (C_V): %.5f\n", cv);

    fclose(archivo_energias);

  
=======
     // Medir el tiempo de finalización
     clock_t fin = clock();
     double tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;
     printf("\nTiempo de ejecución: %.2f segundos.\n", tiempo);
>>>>>>> bd25ce9801706c8d60fa55d84c2754f551a3afc5

    return 0;
}