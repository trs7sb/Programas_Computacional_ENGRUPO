import matplotlib.pyplot as plt

def graficar_momento_angular(fichero):
    # Leer los valores del momento angular desde el fichero
    with open(fichero, 'r') as archivo:
        momento_angular = [float(line.strip()) for line in archivo]

    # Crear el eje de tiempo (un año con pasos de un día)
    tiempo = [i for i in range(len(momento_angular))]  # Tiempo en días

    # Graficar el momento angular en función del tiempo
    plt.figure(figsize=(10, 6))
    plt.plot(tiempo, momento_angular, label="Momento Angular Total", color="blue")
    plt.xlabel("Tiempo (días)")
    plt.ylabel("Momento Angular Total (kg·m²/s)")
    plt.title("Momento Angular Total en función del Tiempo")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()

    # Mostrar la gráfica
    plt.show()

# Llamar a la función con el fichero de datos
graficar_momento_angular("momento_angular_total.txt")