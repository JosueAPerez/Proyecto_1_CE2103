# Proyecto #1 — Arreglos Paginados
**Algoritmos y Estructuras de Datos II (CE 2103)**  
Instituto Tecnológico de Costa Rica — I Semestre 2026

## Descripción

Este proyecto implementa una clase `PagedArray` en C++ que permite manejar arreglos de gran tamaño sin mantener todo su contenido en memoria principal, aplicando el concepto de paginación de administración de memoria. El proyecto incluye dos programas:

- **generator**: genera archivos binarios grandes con números enteros aleatorios.
- **sorter**: ordena archivos binarios usando la clase `PagedArray` con distintos algoritmos de ordenamiento.

---

## Requisitos

- Compilador `g++` con soporte para C++17 o superior
- Sistema operativo: Linux o Windows

---

## Estructura del Proyecto

```
paged-array/
├── README.md
├── src/
│   ├── generator/
│   │   └── generator.cpp
│   └── sorter/
│       ├── sorter.cpp
│       ├── PagedArray.h
│       ├── PagedArray.cpp
│       ├── Page.h
│       └── algorithms/
│           ├── bubble_sort.h
│           ├── insertion_sort.h
│           ├── merge_sort.h
│           ├── quick_sort.h
│           └── quick_sort_m3.h
└── docs/
    └── Proyecto_1_CE2103.pdf
```

---

## Compilación

### Linux

```bash
# Compilar el generador
g++ src/generator/generator.cpp -o generator

# Compilar el ordenador
g++ src/sorter/sorter.cpp src/sorter/PagedArray.cpp -o sorter
```

### Windows

```bat
:: Compilar el generador
g++ src\generator\generator.cpp -o generator.exe

:: Compilar el ordenador
g++ src\sorter\sorter.cpp src\sorter\PagedArray.cpp -o sorter.exe
```

---

## Uso del Generador

```
./generator -size <SIZE> -output <OUTPUT FILE PATH>
```

### Argumentos

| Argumento | Descripción |
|-----------|-------------|
| `-size`   | Tamaño del archivo a generar: `SMALL`, `MEDIUM`, `LARGE` o `TEST` |
| `-output` | Ruta donde se guardará el archivo binario generado |

### Tamaños disponibles

| Valor    | Tamaño  | Enteros generados |
|----------|---------|-------------------|
| `TEST`   | ~512 KB | 131,072           |
| `SMALL`  | 128 MB  | 33,554,432        |
| `MEDIUM` | 256 MB  | 67,108,864        |
| `LARGE`  | 512 MB  | 134,217,728       |

### Ejemplos

```bash
./generator -size TEST   -output test.bin
./generator -size SMALL  -output small.bin
./generator -size MEDIUM -output medium.bin
./generator -size LARGE  -output large.bin
```

---

## Uso del Ordenador

```
./sorter -input <INPUT FILE PATH> -output <OUTPUT FILE PATH> -alg <ALGORITMO> -pageSize <PAGE-SIZE> -pageCount <PAGE-COUNT>
```

### Argumentos

| Argumento    | Descripción |
|--------------|-------------|
| `-input`     | Ruta del archivo binario a ordenar |
| `-output`    | Ruta base para los archivos de salida (se generan `.bin` y `.txt`) |
| `-alg`       | Nombre del algoritmo de ordenamiento |
| `-pageSize`  | Cantidad máxima de enteros por página |
| `-pageCount` | Cantidad de páginas simultáneas en memoria |

### Algoritmos disponibles

| Nombre      | Algoritmo                        | Tamaños soportados        |
|-------------|----------------------------------|---------------------------|
| `merge`     | Merge Sort                       | SMALL, MEDIUM, LARGE      |
| `quick`     | Quick Sort                       | SMALL, MEDIUM, LARGE      |
| `quickm3`   | Quick Sort Mediana de Tres       | SMALL, MEDIUM, LARGE      |
| `bubble`    | Bubble Sort                      | TEST únicamente           |
| `insertion` | Insertion Sort                   | TEST únicamente           |

### Ejemplos

```bash
# Ordenar archivo SMALL con MergeSort
./sorter -input small.bin -output resultado_small -alg merge -pageSize 1000000 -pageCount 5

# Ordenar archivo MEDIUM con QuickSort
./sorter -input medium.bin -output resultado_medium -alg quick -pageSize 1000000 -pageCount 5

# Ordenar archivo LARGE con QuickSort Mediana de Tres
./sorter -input large.bin -output resultado_large -alg quickm3 -pageSize 1000000 -pageCount 5

# Ordenar archivo TEST con BubbleSort
./sorter -input test.bin -output resultado_test -alg bubble -pageSize 1000 -pageCount 5
```

### Archivos de salida

El sorter genera dos archivos a partir del argumento `-output`:

| Archivo            | Descripción                                      |
|--------------------|--------------------------------------------------|
| `<output>.bin`     | Archivo binario con los enteros ordenados        |
| `<output>.txt`     | Archivo de texto con los enteros separados por comas |

### Reporte final

Al terminar, el sorter imprime en consola:

```
Tiempo: X.XX segundos
Algoritmo: merge
Page faults: XX
Page hits: XX
```

---

## Flujo Completo de Ejemplo

```bash
# 1. Compilar ambos programas
g++ src/generator/generator.cpp -o generator
g++ src/sorter/sorter.cpp src/sorter/PagedArray.cpp -o sorter

# 2. Generar un archivo de prueba
./generator -size SMALL -output small.bin

# 3. Ordenar el archivo
./sorter -input small.bin -output resultado -alg merge -pageSize 1000000 -pageCount 5

# 4. Verificar el resultado
head -c 200 resultado.txt
```

---

## Notas

- El repositorio no incluye archivos binarios, ejecutables ni compilados.
- Los algoritmos `bubble` e `insertion` están restringidos al tamaño `TEST` debido a su complejidad O(n²), que los hace inviables para archivos más grandes.
- Se recomienda usar `pageSize` de 1,000,000 y `pageCount` de 5 para obtener los mejores tiempos en archivos grandes.
