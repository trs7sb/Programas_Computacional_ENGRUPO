import numpy as np

import matplotlib.pyplot as plt

# Leer el archivo energias.txt
filename = "energias.txt"
data = np.loadtxt(filename)

# Extraer columnas de datos
T = data[:, 0]  # Energía cinética
V = data[:, 1]  # Energía potencial
E = data[:, 2]  # Energía mecánica total

# Crear un array de tiempo (en días)
days = np.arange(1, len(T) + 1)

# Representar las energías en función del tiempo
plt.figure(figsize=(10, 6))
plt.plot(days, T, label="Energía Cinética (T)", color="blue")
plt.plot(days, V, label="Energía Potencial (V)", color="green")
plt.plot(days, E, label="Energía Mecánica Total (E)", color="red")

# Configurar la gráfica
plt.title("Energías en función del tiempo (un año)")
plt.xlabel("Días")
plt.ylabel("Energía")
plt.legend()
plt.grid(True)

# Mostrar la gráfica
plt.tight_layout()
plt.show()