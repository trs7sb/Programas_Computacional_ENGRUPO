import matplotlib.pyplot as plt
import numpy as np

# Nombre del archivo que contiene los datos
archivo_momento = "momento_angular.txt"

# Leer los datos del archivo
with open(archivo_momento, "r") as file:
    # Cada línea corresponde a un instante de tiempo
    data = [list(map(float, line.split())) for line in file if line.strip()]

# Convertir los datos a un array de NumPy para facilitar el manejo
data = np.array(data)

# Crear un eje temporal (número de líneas = número de días)
tiempos = np.arange(data.shape[0])  # Días desde el inicio

# Graficar el momento angular de cada planeta
for i in range(data.shape[1]):  # Iterar sobre las columnas (planetas)
    plt.plot(tiempos, data[:, i], label=f"Planeta {i+1}")

# Configurar la gráfica
plt.xlabel("Tiempo (días)")
plt.ylabel("Momento Angular (L)")
plt.title("Evolución del Momento Angular de los Planetas")
plt.legend()
plt.grid()

# Mostrar la gráfica
plt.show()