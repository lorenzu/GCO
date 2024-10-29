# Descripción del Programa de Predicción de Ratings

Este programa tiene como objetivo predecir los ratings faltantes en una matriz de utilidad de usuario-ítem, utilizando técnicas de filtrado colaborativo basado en vecinos más cercanos. 

## Archivos del Proyecto

### main.cc

El archivo `main.cc` contiene la función principal del programa, que sigue el siguiente flujo de ejecución:

1. **Lectura de Parámetros de Entrada:**
   - `filename`: Nombre del archivo que contiene la matriz de utilidad (user-item ratings).
   - `metric`: Métrica de similitud a utilizar para encontrar vecinos (`pearson`, `cosine`, `euclidean`).
   - `k`: Número de vecinos más cercanos a considerar.
   - `predictionType`: Tipo de predicción a utilizar (`simple`, `mean`).

2. **Lectura del Archivo de Entrada:**
   - La función `readInputFile` es utilizada para leer el archivo y llenar la matriz de utilidad con ratings de usuarios para diferentes ítems. Los ratings faltantes se representan con `-1`. Además, se obtienen los valores mínimo y máximo de los ratings posibles.

3. **Cálculo de Predicciones:**
   - Para cada elemento faltante en la matriz de utilidad:
     - Se encuentran los `k` vecinos más cercanos utilizando la métrica de similitud especificada (Pearson, Coseno o Euclidiana).
     - Se calcula la predicción del rating faltante utilizando el método especificado:
       - `simple`: Predicción mediante un promedio ponderado simple de los ratings de los vecinos.
       - `mean`: Predicción mediante la diferencia con el promedio de los ratings de los vecinos.

4. **Impresión de la Matriz de Utilidad con Predicciones:**
   - La matriz de utilidad con los ratings predichos reemplazando los valores faltantes es impresa en la salida estándar.

### tools.cc

El archivo `tools.cc` contiene la implementación de diversas funciones auxiliares que son utilizadas en `main.cc`. A continuación se describen estas funciones:

#### Funciones para la Lectura de Datos

- **readInputFile:**
  - Lee el archivo de entrada y llena la matriz de utilidad con los ratings de usuarios para ítems. También obtiene los valores mínimo y máximo de los ratings posibles.
  - Parámetros:
    - `filename`: Nombre del archivo de entrada.
    - `utilityMatrix`: Matriz donde se almacenarán los ratings de usuarios para ítems.
    - `minRating`: Valor mínimo de rating.
    - `maxRating`: Valor máximo de rating.

#### Funciones para el Cálculo de Similitud

- **pearsonCorrelation:**
  - Calcula la correlación de Pearson entre dos vectores, que representa la similitud entre dos usuarios.
  - Parámetros:
    - `a`: Primer vector de ratings.
    - `b`: Segundo vector de ratings.
  - Retorna:
    - Coeficiente de correlación de Pearson entre los dos vectores.

- **cosineSimilarity:**
  - Calcula la similitud coseno entre dos vectores, que representa la similitud entre dos usuarios.
  - Parámetros:
    - `a`: Primer vector de ratings.
    - `b`: Segundo vector de ratings.
  - Retorna:
    - Similitud coseno entre los dos vectores.

- **euclideanDistance:**
  - Calcula la distancia euclidiana entre dos vectores, que representa la disimilitud entre dos usuarios.
  - Parámetros:
    - `a`: Primer vector de ratings.
    - `b`: Segundo vector de ratings.
  - Retorna:
    - Distancia euclidiana entre los dos vectores.

#### Funciones para Encontrar Vecinos y Predecir Ratings

- **findNeighbors:**
  - Encuentra los `k` vecinos más cercanos para un usuario y un ítem dados, utilizando la métrica de similitud especificada.
  - Parámetros:
    - `utilityMatrix`: Matriz de utilidad de usuario-ítem.
    - `userIndex`: Índice del usuario para el cual se buscan vecinos.
    - `itemIndex`: Índice del ítem para el cual se buscan vecinos.
    - `k`: Número de vecinos a encontrar.
    - `metric`: Métrica de similitud a utilizar (`pearson`, `cosine`, `euclidean`).
  - Retorna:
    - Vector de pares, donde cada par contiene el índice de un vecino y su puntaje de similitud.

- **predictSimple:**
  - Predice el rating de un ítem para un usuario utilizando un promedio ponderado simple de los ratings de los vecinos más cercanos.
  - Parámetros:
    - `utilityMatrix`: Matriz de utilidad de usuario-ítem.
    - `userIndex`: Índice del usuario para el cual se predice el rating.
    - `itemIndex`: Índice del ítem para el cual se predice el rating.
    - `neighbors`: Lista de vecinos y sus puntajes de similitud.
  - Retorna:
    - Rating predicho.

- **predictDiffWithMean:**
  - Predice el rating de un ítem para un usuario utilizando la diferencia con el promedio de los ratings de los vecinos.
  - Parámetros:
    - `utilityMatrix`: Matriz de utilidad de usuario-ítem.
    - `userIndex`: Índice del usuario para el cual se predice el rating.
    - `itemIndex`: Índice del ítem para el cual se predice el rating.
    - `neighbors`: Lista de vecinos y sus puntajes de similitud.
  - Retorna:
    - Rating predicho.

#### Función para Imprimir la Matriz

- **printMatrix:**
  - Imprime una matriz de utilidad en la salida estándar, reemplazando los valores faltantes (`-1`) con un guión.
  - Parámetros:
    - `matrix`: Matriz a imprimir.

#### Función de Uso

- **Usage:**
  - Muestra el uso correcto del programa y verifica que los argumentos de entrada sean correctos.
  - Parámetros:
    - `argc`: Número de argumentos de entrada.
    - `argv`: Vector de argumentos de entrada.

# Ejemplo de Uso del Programa de Predicción de Ratings

## Archivos Necesarios

### Archivo de Entrada (input.txt)

El archivo de entrada debe contener la matriz de utilidad de usuario-ítem, así como los valores mínimo y máximo de los ratings posibles. A continuación se muestra un ejemplo de cómo debe estar estructurado este archivo:

1.000
5.000
4.000 3.000 - 5.000 1.000
5.000 - 2.000 4.000 3.000
3.000 2.000 4.000 - 5.000
- 4.000 3.000 2.000 1.000
2.000 1.000 5.000 3.000 -


En este archivo:
- La primera línea (`1`) representa el rating mínimo.
- La segunda línea (`5`) representa el rating máximo.
- Las siguientes líneas representan la matriz de utilidad, donde los valores faltantes se denotan con un guión (`-`).

### Matriz de Utilidad

La matriz de utilidad, derivada del archivo de entrada anterior, puede representarse de la siguiente manera:

| Usuario / Ítem | 1 | 2 | 3 | 4 | 5 |
|-----------------|---|---|---|---|---|
| **1**           | 4 | 3 | - | 5 | 1 |
| **2**           | 5 | - | 2 | 4 | 3 |
| **3**           | 3 | 2 | 4 | - | 5 |
| **4**           | - | 4 | 3 | 2 | 1 |
| **5**           | 2 | 1 | 5 | 3 | - |

### Archivo main.cc

Este archivo contiene la función principal del programa, que procesa el archivo de entrada, calcula las predicciones y muestra la matriz de utilidad con los ratings predichos. 

### Archivo tools.cc

Este archivo contiene las funciones auxiliares para leer el archivo de entrada, calcular similitudes, encontrar vecinos, predecir ratings e imprimir la matriz.

## Compilación del Programa

Para compilar el programa, ejecutamos `make` y después `./bin/recommender <filename> <metric> <k> <predictionType>`

### Parámetros

- **`<filename>`**: El nombre del archivo de entrada que contiene la matriz de utilidad.
- **`<metric>`**: La métrica de similitud que deseas utilizar. Puede ser una de las siguientes:
  - `pearson`: Correlación de Pearson.
  - `cosine`: Similitud del coseno.
  - `euclidean`: Distancia euclidiana.
  
- **`<k>`**: El número de vecinos más cercanos que se utilizarán para hacer las predicciones.
  
- **`<predictionType>`**: El tipo de predicción que se desea utilizar. Puede ser una de las siguientes opciones:
  - `simple`: Predicción utilizando un promedio ponderado simple.
  - `mean`: Predicción utilizando la diferencia con el promedio.

