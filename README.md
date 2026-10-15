# Torneos deportivos

**Integrantes:** Denniston Tomás, Costa Agustin

Programa en C que guarda torneos deportivos en un archivo de texto y los maneja desde un menú.

## Archivos

- `torneos.c`: el código.
- `torneos.txt`: datos de prueba (10 torneos con valores inventados).

## Compilar y ejecutar

```bash
gcc torneos.c -o torneos
./torneos
```

`torneos.txt` tiene que estar en la misma carpeta que el ejecutable.

## Cómo funciona

Cada torneo es una estructura `t_torneo` con cinco campos: `nombre` y `deporte` (texto), `cantEquipos` (int), `premio` (double) y `finalizado` (booleano, hecho con un enum de `FALSO` y `VERDADERO`).

En `torneos.txt` cada línea es un torneo, con los campos separados por espacios:

```
Copa_Libertadores Futbol 32 23000000.00 0
```

Como el espacio separa los campos, el programa guarda los nombres con `_` en lugar de espacios y los vuelve a mostrar con espacios en pantalla. El último campo es `1` si el torneo terminó y `0` si sigue en curso.

### Opciones del menú

1. **Agregar:** pide los datos por teclado y agrega una línea al final del archivo (modo `"a"`).
2. **Listar todo:** muestra todos los torneos en una tabla (modo `"r"`).
3. **Listar con condición:** muestra solo los torneos con premio mayor o igual a un monto que ingresa el usuario.
4. **Dividir:** copia los torneos terminados a `torneos_finalizados.txt` y el resto a `torneos_en_curso.txt`.
5. **Salir.**

`leerTorneo` y `escribirTorneo` son las únicas funciones que conocen el formato del archivo, y las usan todas las opciones. El programa no tiene variables globales: los datos pasan de una función a otra por parámetro. Todo lo que se ingresa por teclado se valida antes de guardarlo.
