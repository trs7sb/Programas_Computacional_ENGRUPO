#include <stdio.h>
#include <math.h>
#include <time.h> 
#include <stdbool.h>


// Constantes físicas
#define G 6.67430e-11 // Constante gravitacional (m^3 kg^-1 s^-2)
#define AU 1.496e11   // Unidad astronómica (m)
#define DAY 86400     // Un día en segundos
#define YEAR 365.25   // Un año en días
#define NUM_PLANETS 20 // Número de planetas (incluyendo el Sol)
#define MASA_SOLAR 1.989e30 // Masa del Sol en kg

// Datos de los planetas (masas en kg, distancias iniciales en m, velocidades iniciales en la dirección y en m/s)
// typedef permite crear objetos de tipo struct sin tener que escribir la palabra struct cada vez 
typedef struct {
    char name[20]; // Nombre del planeta
    double mass;       // Masa del planeta
    double position[2]; // Posición (x, y) en m
    double velocity[2]; // Velocidad (vx, vy) en m/s

} Planet;       

// Inicializar datos reales de los planetas
void inicializarPlanetas(Planet planets[]) { 
    // Datos simplificados: nombre, masa (kg), posición inicial (m), velocidad inicial (m/s)
    Planet temp[NUM_PLANETS] = {
        {"Sol", MASA_SOLAR, {0, 0}, {0, 0}}, 
        {"Mercurio", 3.3011e23, {0.39 * AU, 0}, {0, 47400}},
        {"Venus", 4.8675e24, {0.72 * AU, 0}, {0, 35020}},
        {"Tierra", 5.97237e24, {1.0 * AU, 0}, {0, 29780}},
        {"Marte", 6.4171e23, {1.52 * AU, 0}, {0, 24070}},
        {"Júpiter", 1.8982e27, {5.2 * AU, 0}, {0, 13070}},
        {"Saturno", 5.6834e26, {9.58 * AU, 0}, {0, 9680}},
        {"Urano", 8.6810e25, {19.22 * AU, 0}, {0, 6800}},
        {"Neptuno", 1.02413e26, {30.05 * AU, 0}, {0, 5430}},
        {"Plutón", 1.30900e22, {39.48 * AU, 0}, {0, 4748}},
        // Nuevos planetas ficticios
        {"PlanetaX", 2.5e24, {50.0 * AU, 0}, {0, 4000}},
        {"Gaia", 3.0e24, {60.0 * AU, 0}, {0, 3500}},
        {"Zeus", 4.0e24, {70.0 * AU, 0}, {0, 3000}},
        {"Hera", 5.0e24, {80.0 * AU, 0}, {0, 2500}},
        {"Ares", 6.0e24, {90.0 * AU, 0}, {0, 2000}},
        {"Poseidon", 7.0e24, {100.0 * AU, 0}, {0, 1500}},
        {"Athena", 8.0e24, {110.0 * AU, 0}, {0, 1000}},
        {"Apollo", 9.0e24, {120.0 * AU, 0}, {0, 800}},
        {"Artemis", 1.0e25, {130.0 * AU, 0}, {0, 600}},
        {"Hades", 1.1e25, {140.0 * AU, 0}, {0, 400}}
    };
    for (int i = 0; i < NUM_PLANETS; i++) {
        planets[i] = temp[i];
    }
}

// Función para dividir la masa de los planetas entre la masa solar
void normalizarMasa(Planet planets[]) {
    for (int i = 0; i < NUM_PLANETS; i++) {
        planets[i].mass /= MASA_SOLAR;
    }
}

// Función para convertir distancias y velocidades a unidades astronómicas (UA y UA/s)
void convertirUnidadesAU(Planet planets[]) {
    for (int i = 0; i < NUM_PLANETS; i++) {
        // Convertir posición de metros a UA
        planets[i].position[0] /= AU;
        planets[i].position[1] /= AU;

        // Convertir velocidad de m/s a UA/s
        planets[i].velocity[0] /= AU;
        planets[i].velocity[1] /= AU;
    }
}

// Función para convertir de unidades rescaladas a unidades originales
void convertirAUnidadesOriginales(Planet planets[]) {
    for (int i = 0; i < NUM_PLANETS; i++) {
        // Convertir masa de masas solares a kilogramos
        planets[i].mass *= MASA_SOLAR;

        // Convertir posición de UA a metros
        planets[i].position[0] *= AU;
        planets[i].position[1] *= AU;

        // Convertir velocidad de UA/s a m/s
        planets[i].velocity[0] *= AU;
        planets[i].velocity[1] *= AU;
    }
}

// Función para reescalar las velocidades según el factor de tiempo
void reescalarVelocidades(Planet planets[], double factor_tiempo) {
    for (int i = 0; i < NUM_PLANETS; i++) {
        planets[i].velocity[0] /= factor_tiempo;
        planets[i].velocity[1] /= factor_tiempo;
    }
}

// Función para deshacer el reescalado de las velocidades
void deshacerReescaladoVelocidades(Planet planets[], double factor_tiempo) {
    for (int i = 0; i < NUM_PLANETS; i++) {
        planets[i].velocity[0] *= factor_tiempo;
        planets[i].velocity[1] *= factor_tiempo;
    }
}

// Calcular la fuerza gravitacional entre dos planetas
void calcularFuerza(Planet *a, Planet *b, double *fx, double *fy) {
    double dx = b->position[0] - a->position[0];
    double dy = b->position[1] - a->position[1];
    double distancia = sqrt(dx * dx + dy * dy);
    double fuerza = (1*a->mass * b->mass) / (distancia * distancia); //G es la unidad debido al rescalamiento
    *fx = fuerza * dx / distancia; //Componente x del vector fuerza 
    *fy = fuerza * dy / distancia; //Componente y del vector fuerza
}
//b es un puntero de tipo Planet, apunta a una dirección de memoria de una variable de tipo Planet
//-> es un operador de acceso a una propiedad del struct Planet equivalente a (*b).position[0]


void calcularAceleraciones(Planet planets[], double a[NUM_PLANETS][2]) {

    double fuerzas[NUM_PLANETS][2] = {0};
    // Calcular fuerzas gravitacionales entre todos los planetas
    for (int i = 0; i < NUM_PLANETS; i++) {
        //j=i+1
        //Al calcular las fuerzas sobre el planeta i=3 no tengo que considerar la interacción con los planetas j=1,2 porque ya las he considerado en las interacciones anteriores con i=1,2
        //Así evito calcular la fuerza entre los planetas i y j dos veces
        //Tampoco considero la interacción del planeta i consigo mismo
        for (int j = i + 1; j < NUM_PLANETS; j++) {
            double fx, fy;
            calcularFuerza(&planets[i], &planets[j], &fx, &fy);
            fuerzas[i][0] += fx; //Fuerza de i sobre j en la dirección x
            fuerzas[i][1] += fy;
            fuerzas[j][0] -= fx; // Fuerza de i sobre j es igual y opuesta a la fuerza de j sobre i
            fuerzas[j][1] -= fy;
        }
    }
    // Calcular aceleraciones a partir de la suma de fuerzas
    for (int i = 0; i < NUM_PLANETS; i++) {
        a[i][0] = fuerzas[i][0] / planets[i].mass;
        a[i][1] = fuerzas[i][1] / planets[i].mass;
    }
}

// Calcular las energías del sistema (SIN RESCALAMIENTO)

void calcularEnergias(Planet planets[], double *energiaCinetica, double *energiaPotencial) {
    *energiaCinetica = 0;
    *energiaPotencial = 0;

    // Energía cinética del sistema
    for (int i = 0; i < NUM_PLANETS; i++) {
        double velocidad2 = planets[i].velocity[0] * planets[i].velocity[0] +
                            planets[i].velocity[1] * planets[i].velocity[1];
        *energiaCinetica += 0.5 * planets[i].mass * velocidad2;
    }

    // Energía potencial del sistema
    for (int i = 0; i < NUM_PLANETS; i++) {
        //j=i+1 evito calcular la energía potencial entre los planetas i y j dos veces
        for (int j = i + 1; j < NUM_PLANETS; j++) {
            double dx = planets[j].position[0] - planets[i].position[0];
            double dy = planets[j].position[1] - planets[i].position[1];
            double distancia = sqrt(dx * dx + dy * dy);
            *energiaPotencial -= (G * planets[i].mass * planets[j].mass) / distancia;
        }
    }
}


// Actualizar posiciones y velocidades usando el método de Verlet
void actualizarPlanetas(Planet planets[], double dt) {
    double a[NUM_PLANETS][2] = {0};

    //Calcula las aceleraciones en el tiempo t a partir de las fuerzas con las posiciones en el tiempo t
    calcularAceleraciones(planets, a); 

    double w[NUM_PLANETS][2] = {0};

    //Almacena en un array w las velocidades y aceleraciones en el tiempo t 

    for (int i = 1; i < NUM_PLANETS; i++) {
        w[i][0] = planets[i].velocity[0] + 0.5 * dt * a[i][0];
        w[i][1] = planets[i].velocity[1] + 0.5 * dt * a[i][1];
    }

    // Actualizar posiciones al tiempo t+dt
    for (int i = 0; i < NUM_PLANETS; i++) {
        planets[i].position[0] += planets[i].velocity[0] * dt + 0.5 * a[i][0] * dt * dt;
        planets[i].position[1] += planets[i].velocity[1] * dt + 0.5 * a[i][1] * dt * dt;
    }

    // Calcular la aceleración con las posiciones actualizadas
    calcularAceleraciones(planets, a);

    //Calcular las nuevas velocidades al tiempo t+dt a partir de las aceleraciones en el tiempo t+dt y el array w
    for (int i = 0; i < NUM_PLANETS; i++) {
        planets[i].velocity[0] = w[i][0] + 0.5 * a[i][0] * dt;
        planets[i].velocity[1] = w[i][1] + 0.5 * a[i][1] * dt;
    }
}

// Imprimir las posiciones de los planetas en un instante de tiempo
void imprimirPosiciones(Planet planets[], double tiempo) {
    printf("Tiempo: %.2f días\n", tiempo / DAY);

    for (int i = 0; i < NUM_PLANETS; i++) {
        printf("%s: x = %.2e, y = %.2e\n", planets[i].name, planets[i].position[0], planets[i].position[1]);
    }
    printf("\n"); //salto de línea
}

// Función para guardar las posiciones de los planetas en un archivo
void guardarPosiciones(Planet planets[], FILE *archivo_posiciones) {
    for (int i = 0; i < NUM_PLANETS; i++) {
        fprintf(archivo_posiciones, "%.6e, %.6e\n", planets[i].position[0], planets[i].position[1]);
    }
    fprintf(archivo_posiciones, "\n"); // Línea en blanco para separar instantes de tiempo
}

// Función para calcular los períodos de los planetas usando la Tercera Ley de Kepler
void calcularPeriodosKepler(Planet planets[], double periodos[]) {
    for (int i = 0; i < NUM_PLANETS; i++) {
        // Calcular la distancia media al Sol (en UA)
        double a = sqrt(planets[i].position[0] * planets[i].position[0] +
                        planets[i].position[1] * planets[i].position[1]);

        // Usar la Tercera Ley de Kepler: T^2 = a^3
        periodos[i] = sqrt(a * a * a); // Período en años
    }
}
 
double calcularMomentoAngularTotal(Planet planets[]) {
    double momento_angular_total = 0.0;

    for (int i = 0; i < NUM_PLANETS; i++) {
        // Módulo de la posición
        double r = sqrt(planets[i].position[0] * planets[i].position[0] +
                        planets[i].position[1] * planets[i].position[1]);

        // Módulo de la velocidad
        double v = sqrt(planets[i].velocity[0] * planets[i].velocity[0] +
                        planets[i].velocity[1] * planets[i].velocity[1]);

        // Momento angular del planeta
        double momento_angular = planets[i].mass * r * v;

        // Sumar al momento angular total
        momento_angular_total += momento_angular;
    }

    return momento_angular_total;
}


int main() {
    time_t inicio = time(NULL); // Guardar el tiempo de inicio de la simulación

    int vueltas[NUM_PLANETS];
    Planet planets[NUM_PLANETS];


    inicializarPlanetas(planets);

    // Rescalar masas (a masa solar) y posiciones y velocidades a UA y UA/s
    normalizarMasa(planets);
    convertirUnidadesAU(planets);

    // Calcular el factor de reescalado del tiempo
    double factor_tiempo = sqrt(G * MASA_SOLAR / pow(AU, 3));

    // Reescalar las velocidades según el factor de tiempo
    reescalarVelocidades(planets, factor_tiempo);

    // Reescalar el tiempo
    double dt = DAY * factor_tiempo; 
    double tiempo_total = 50*YEAR * DAY * factor_tiempo; 

    FILE *archivo = fopen("energias.txt", "w");
    if (!archivo) {
        perror("Error al abrir el archivo");
        return 1;
    }

    // Abrir archivo para guardar las posiciones
    FILE *archivo_posiciones = fopen("posiciones_planetas.txt", "w");
     if (!archivo_posiciones) {
        perror("Error al abrir el archivo de posiciones");
        return 1;
    }
    
    // Abrir archivo para guardar el momento angular total
    FILE *archivo_momento_total = fopen("momento_angular_total.txt", "w");
    if (!archivo_momento_total) {
    perror("Error al abrir el archivo de momento angular total");
    return 1;
    }

    //Inicializar el número de vueltas completas de cada planeta
    for (int i = 0; i < NUM_PLANETS; i++) {
        vueltas[i] = 0;
    }

    bool cruzo_eje[NUM_PLANETS]; // Indica si el planeta ha cruzado el eje y
    for (int i = 0; i < NUM_PLANETS; i++) {
        cruzo_eje[i] = false; // Inicialmente, ningún planeta ha cruzado el eje
    }


    //CON EL TIEMPO Y LAS CONDICIONES INICIALES RESCALADAS
    for (double t = 0; t < tiempo_total; t += dt) {

        //Calcular posiciones y velocidades en el tiempo t+dt
        actualizarPlanetas(planets, dt);
        // Guardar las posiciones de los planetas para cada tiempo.
        guardarPosiciones(planets, archivo_posiciones);
        
        // Convertir a unidades originales antes de calcular las energías
        convertirAUnidadesOriginales(planets);
        // Deshacer el reescalado de las velocidades por el factor tiempo 
        deshacerReescaladoVelocidades(planets, factor_tiempo);
        

        double energiaCinetica, energiaPotencial;
        //Devuelve la energía cinética y potencial del sistema en el tiempo t + dt  en (m, kg, s)
        calcularEnergias(planets, &energiaCinetica, &energiaPotencial);
        double energiaMecanica = energiaCinetica + energiaPotencial;

        //Guarda las energías en el archivo. El tiempo en días se tiene en cuenta en el código de python 
        fprintf(archivo, "%.6e %.6e %.6e\n", energiaCinetica, energiaPotencial, energiaMecanica);

        // Calcular el momento angular total y guardarlo en el archivo
        double momento_angular_total = calcularMomentoAngularTotal(planets);
         fprintf(archivo_momento_total, "%.6e\n", momento_angular_total);

        if ((int)(t / dt) % 30 == 0) { // Imprimir cada 30 días
            imprimirPosiciones(planets, t / factor_tiempo); // Tiempo en unidades originales
        }

        // Volver a normalizar las unidades para continuar la simulación
        normalizarMasa(planets);
        convertirUnidadesAU(planets);

        // Reescalar las velocidades nuevamente para continuar la simulación
        reescalarVelocidades(planets, factor_tiempo);

        //Calcular el número de vueltas completas de cada planeta y guardarlo en un array
        for (int i = 0; i < NUM_PLANETS; i++) {
            // Detectar si el planeta cruza el eje x
            if (planets[i].position[1] < 0 && !cruzo_eje[i]) {
                cruzo_eje[i] = true; // Marca que el planeta ha cruzado el eje
            } else if (planets[i].position[1] >= 0 && cruzo_eje[i]) {
                // Si vuelve a cruzar hacia y > 0, cuenta una vuelta completa
                vueltas[i]++;
                cruzo_eje[i] = false; // Reinicia el estado para el próximo cruce
            }
        }

    }

    // Imprimir el número de vueltas completas
    printf("Número de vueltas completas de los planetas:\n");
    for (int i = 0; i < NUM_PLANETS; i++) {
        printf("%s: %d vueltas\n", planets[i].name, vueltas[i]);
    }
    
    // Calcular e imprimir el período orbital de cada planeta
    printf("\nPeríodos orbitales de los planetas.:\n");
    for (int i = 0; i < NUM_PLANETS; i++) {
     if (vueltas[i] > 0) {
        double periodo = (50*YEAR) / vueltas[i]; // Tiempo total dividido por el número de vueltas
        printf("%s: %.2f años\n", planets[i].name, periodo/YEAR); 
    } else {
        printf("%s: No completó ninguna vuelta.\n", planets[i].name);
    }
}

    fclose(archivo);
    fclose(archivo_posiciones);
    fclose(archivo_momento_total);
    time_t fin = time(NULL); // Guardar el tiempo de finalización de la simulación
    printf("Tiempo de inicio: %s", ctime(&inicio)); // Imprimir el tiempo de inicio
    printf("Tiempo de finalización: %s", ctime(&fin)); // Imprimir el tiempo de finalización
    double tiempo_total_simulacion = difftime(fin, inicio); // Calcular el tiempo total de la simulación
    printf("Tiempo total de simulación: %.8f segundos\n", tiempo_total_simulacion); // Imprimir el tiempo total de la simulación
    return 0;
}