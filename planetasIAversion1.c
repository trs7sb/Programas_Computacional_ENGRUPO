#include <stdio.h>
#include <math.h>

// Constantes físicas
#define G 6.67430e-11 // Constante gravitacional (m^3 kg^-1 s^-2)
#define AU 1.496e11   // Unidad astronómica (m)
#define DAY 86400     // Un día en segundos
#define YEAR 365.25   // Un año en días
#define NUM_PLANETS 8 // Número de planetas

// Datos de los planetas (masas en kg, distancias iniciales en AU, velocidades iniciales en m/s)
typedef struct {
    char name[20];
    double mass;       // Masa del planeta
    double position[2]; // Posición (x, y) en m
    double velocity[2]; // Velocidad (vx, vy) en m/s
} Planet;

// Inicializar datos reales de los planetas
void inicializarPlanetas(Planet planets[]) {
    // Datos simplificados: nombre, masa (kg), posición inicial (AU), velocidad inicial (m/s)
    Planet temp[NUM_PLANETS] = {
        {"Mercurio", 3.3011e23, {0.39 * AU, 0}, {0, 47400}},
        {"Venus", 4.8675e24, {0.72 * AU, 0}, {0, 35020}},
        {"Tierra", 5.97237e24, {1.0 * AU, 0}, {0, 29780}},
        {"Marte", 6.4171e23, {1.52 * AU, 0}, {0, 24070}},
        {"Júpiter", 1.8982e27, {5.2 * AU, 0}, {0, 13070}},
        {"Saturno", 5.6834e26, {9.58 * AU, 0}, {0, 9680}},
        {"Urano", 8.6810e25, {19.22 * AU, 0}, {0, 6800}},
        {"Neptuno", 1.02413e26, {30.05 * AU, 0}, {0, 5430}}
    };
    for (int i = 0; i < NUM_PLANETS; i++) {
        planets[i] = temp[i];
    }
}

// Calcular la fuerza gravitacional entre dos planetas
void calcularFuerza(Planet *a, Planet *b, double *fx, double *fy) {
    double dx = b->position[0] - a->position[0];
    double dy = b->position[1] - a->position[1];
    double distancia = sqrt(dx * dx + dy * dy);
    double distancia_cubica = distancia*distancia*distancia; // Distancia al cubo
    double fuerza = (G * a->mass * b->mass) / (distancia * distancia);
    *fx = fuerza * dx /distancia_cubica;
    *fy = fuerza * dy / distancia_cubica;
}


void calcularAceleraciones(Planet planets[], double a[NUM_PLANETS][2]) {
        // Inicializar fuerzas
    double fuerzas[NUM_PLANETS][2] = {0};
        // Calcular fuerzas gravitacionales entre todos los planetas
        for (int i = 0; i < NUM_PLANETS; i++) {
            for (int j = i + 1; j < NUM_PLANETS; j++) {
                double fx, fy;
                calcularFuerza(&planets[i], &planets[j], &fx, &fy);
                fuerzas[i][0] += fx;
                fuerzas[i][1] += fy;
                fuerzas[j][0] -= fx;
                fuerzas[j][1] -= fy;
            }
        }
        // Calcular aceleraciones
        for (int i = 0; i < NUM_PLANETS; i++) {
            a[i][0] = fuerzas[i][0] / planets[i].mass;
            a[i][1] = fuerzas[i][1] / planets[i].mass;
        }
    }


// Actualizar posiciones y velocidades usando el método de Verlet
void actualizarPlanetas(Planet planets[], double dt) {
    
    double a[NUM_PLANETS][2] = {0};

    calcularAceleraciones(planets, a);
    
    double w[NUM_PLANETS][2] = {0};

    for (int i = 1; i < NUM_PLANETS; i++)
        {
            w[i][0] =  planets[i].velocity[0] + 0.5 * dt * a[i][0];
            w[i][1] =  planets[i].velocity[1]  + 0.5 * dt * a[i][0];
        }

    // Actualizar posiciones y velocidades
    for (int i = 0; i < NUM_PLANETS; i++) 
        {
        // Actualizar posición
        planets[i].position[0] += planets[i].velocity[0] * dt + 0.5 * a[i][0] * dt * dt;
        planets[i].position[1] += planets[i].velocity[1] * dt + 0.5 * a[i][0] * dt * dt;
        }

    //Calcular la aceleración con las posiciones actualizadas
     calcularAceleraciones(planets,a);

    for (int i = 0; i < NUM_PLANETS; i++) 
        {
        // Actualizar velocidad
        planets[i].velocity[0] += w[i][0] + 0.5*a[i][0] * dt;
        planets[i].velocity[1] += w[i][1] + 0.5*a[i][1] * dt;
         }

    }
// Imprimir las posiciones de los planetas
void imprimirPosiciones(Planet planets[], double tiempo) {
    printf("Tiempo: %.2f días\n", tiempo / DAY);

    for (int i = 0; i < NUM_PLANETS; i++) 
        {
        printf("%s: x = %.2e, y = %.2e\n", planets[i].name, planets[i].position[0], planets[i].position[1]);
        }
    printf("\n");
}

int main() {

    Planet planets[NUM_PLANETS];
    inicializarPlanetas(planets);

    double dt = DAY; // Paso de tiempo (1 día)
    double tiempo_total = YEAR * DAY; // Simular un año

    for (double t = 0; t < tiempo_total; t += dt) 
    {
        actualizarPlanetas(planets, dt);
        if ((int)(t / dt) % 30 == 0) 
            { 
            // Imprimir cada 30 días
            imprimirPosiciones(planets, t);
            }
    }

    return 0;
}