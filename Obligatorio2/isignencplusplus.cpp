#include <iostream>
#include <fstream>
#include <cmath>
#include <random> // Para std::philox_engine
#include <ctime>

using namespace std;

// Constantes
const int N = 10;          // Tamaño de la red (N x N)
const int ITERACIONES = 100000; // Número de iteraciones
const double T = 0.0001;
const double K_BOLTZMANN = 1.0; // Constante de Boltzmann (J/K)

// Inicializar el generador de números aleatorios
std::random_device rd; // Semilla aleatoria
std::philox_engine<uint64_t, 10, 256> rng(rd()); // Motor Philox
std::uniform_real_distribution<double> uniform_dist(0.0, 1.0); // Distribución uniforme [0, 1]

// Función para inicializar la red con espines aleatorios (+1 o -1)
void inicializarRed(int red[N][N], int sesgo) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            // Generar un número aleatorio entre 0 y 99
            int probabilidad = static_cast<int>(uniform_dist(rng) * 100);
            // Asignar +1 si está dentro del porcentaje de sesgo, -1 en caso contrario
            red[i][j] = (probabilidad < sesgo) ? 1 : -1;
        }
    }
}

// Función para inicializar la red de forma ordenada (+1 o -1)
void inicializarRedOrdenada(int red[N][N], int valor) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            red[i][j] = valor; // Asigna el valor +1 o -1 a toda la red
        }
    }
}

// Función para guardar la red en un archivo
void guardarRed(ofstream &archivo, int red[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            archivo << red[i][j];
            if (j < N - 1) {
                archivo << ","; // Separador entre columnas
            }
        }
        archivo << "\n"; // Nueva línea al final de cada fila
    }
    archivo << "\n"; // Línea en blanco para separar iteraciones
}

// Algoritmo de Monte Carlo para el modelo de Ising
void monteCarloIsing(int red[N][N], double beta, int iteraciones) {
    ofstream archivo("matriz_red.txt");
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo para guardar la red." << endl;
        exit(1);
    }

    for (int i = 0; i < iteraciones; i++) {
        // Elegir un espín aleatorio en la red
        int n = static_cast<int>(uniform_dist(rng) * N);
        int m = static_cast<int>(uniform_dist(rng) * N);

        // Calcular los vecinos con condiciones de contorno periódicas
        int arriba = (n == 0) ? red[N - 1][m] : red[n - 1][m];
        int abajo = (n == N - 1) ? red[0][m] : red[n + 1][m];
        int izquierda = (m == 0) ? red[n][N - 1] : red[n][m - 1];
        int derecha = (m == N - 1) ? red[n][0] : red[n][m + 1];

        // Calcular el cambio de energía cuando se invierte el espín
        int suma_vecinos = arriba + abajo + izquierda + derecha;
        double deltaE = 2 * red[n][m] * suma_vecinos;

        // Calcular la probabilidad de transición
        double probabilidad = exp(-beta * deltaE);
        if (probabilidad > 1.0) {
            probabilidad = 1.0;
        }

        // Generar un número aleatorio con probabilidad uniforme entre 0 y 1 para decidir si aceptar el cambio
        double r = uniform_dist(rng);
        if (r < probabilidad) {
            red[n][m] *= -1; // Si se acepta el cambio, invertir el signo del espín
        }

        // Guardar la red en el archivo después de cada iteración
        guardarRed(archivo, red);
    }

    archivo.close();
}

// Función para imprimir la red
void imprimirRed(int red[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << red[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    double beta = 1.0 / (K_BOLTZMANN * T); // Beta = 1 / (k_B * T)

    int red[N][N];
    int opcion, sesgo;

    // Solicitar al usuario cómo inicializar la red
    cout << "Seleccione cómo inicializar la red:" << endl;
    cout << "1. Aleatoria" << endl;
    cout << "2. Con sesgo de espines paralelos o antiparalelos" << endl;
    cout << "Ingrese su opción (1 o 2): ";
    cin >> opcion;

    if (opcion == 1) {
        // Inicializar la red de forma completamente aleatoria
        inicializarRed(red, 50); // 50% de probabilidad para +1 o -1
    } else if (opcion == 2) {
        // Solicitar el porcentaje de sesgo
        cout << "Ingrese el porcentaje de sesgo para espines paralelos (0-100): ";
        cin >> sesgo;
        if (sesgo < 0 || sesgo > 100) {
            cout << "Porcentaje inválido. Usando 50% por defecto." << endl;
            sesgo = 50;
        }
        inicializarRed(red, sesgo);
    } else {
        cout << "Opción no válida. Inicializando de forma aleatoria por defecto." << endl;
        inicializarRed(red, 50);
    }

    // Imprimir la configuración inicial
    cout << "Configuración inicial de la red:" << endl;
    imprimirRed(red);

    // Ejecutar el algoritmo de Monte Carlo
    monteCarloIsing(red, beta, ITERACIONES);

    // Imprimir la configuración final
    cout << "\nConfiguración final de la red:" << endl;
    imprimirRed(red);

    return 0;
}