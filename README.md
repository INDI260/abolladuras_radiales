# abolladuras_radiales

Implementación de abolladuras sobre superficies 2D usando CGAL y funciones de base radial.

## Dependencias

- CGAL (>= 5.x)
- CMake (>= 3.10)
- Python 3 con matplotlib y numpy (para visualizacion)

## Compilacion

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Script: `test/generate_surface.sh`

Genera un par de archivos (`.obj` + `_dents.txt`) con un timestamp como ID comun.

### Uso

```bash
./test/generate_surface.sh <filas> <columnas> <abolladuras> [rangos...]
```

### Parametros

| Posicion | Parametro | Default |
|----------|-----------|---------|
| 1 | filas | — |
| 2 | columnas | — |
| 3 | abolladuras | — |
| 4-5 | x_min x_max | 0 100 |
| 6-7 | y_min y_max | 0 100 |
| 8-9 | z_min z_max | -5 5 |
| 10-11 | r_min r_max | 3 20 |
| 12-13 | i_min i_max | 0.1 1.0 |

Los puntos se generan como un grid estructurado de `filas x columnas`
con coordenadas `(x,y)` espaciadas uniformemente y altura `z` aleatoria
dentro del rango especificado. Las abolladuras se distribuyen aleatoriamente
en el mismo plano.

### Ejemplos

```bash
# Grid 50x30 plano con 3 abolladuras, valores por defecto
./test/generate_surface.sh 50 30 3

# Grid 100x100 con alturas pequenas y abolladuras chicas
./test/generate_surface.sh 100 100 5 0 10 0 10 -2 2 0.5 3 0 1
```

## Binario: `bash_surface`

Aplica las abolladuras sobre un `.obj` usando funciones de base radial (RBF).

### Uso

```bash
./build/bash_surface entrada.obj dents.txt rbf_type salida.obj [before.obj]
```

`rbf_type`: `gaussian`, `multiquadric` o `inverse_multiquadric`.

Si se pasa `before.obj`, guarda la triangulacion antes de aplicar las abolladuras.

### Ejemplos

```bash
./build/bash_surface entrada.obj dents.txt gaussian salida.obj
./build/bash_surface entrada.obj dents.txt gaussian salida.obj antes.obj
```

## Visualizacion: `test/visualize.py`

Genera una imagen PNG con los puntos coloreados por altura y las abolladuras como circulos.

```bash
python3 test/visualize.py puntos.obj dents.txt
# Genera: vis_<timestamp>.png
```

