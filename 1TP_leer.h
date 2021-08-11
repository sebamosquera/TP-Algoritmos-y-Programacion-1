#ifndef LEER_H
#define LEER_H

#include "poligono.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define MSK_COLOR 0XC0 // 0B 1100 0000
#define MSK_MOVIMIENTO 0X30 // 0B 0011 0000
#define MSK_GEOMETRIA 0X0F // 0B 0000 1111
#define SHIFT_COLOR 6
#define SHIFT_MOVIMIENTO 4 
#define SHIFT_GEOMETRIA 0 

typedef enum {AZUL, NARANJA, VERDE, GRIS, AMARILLO} color_t;
typedef enum {INMOVIL, CIRCULAR, HORIZONTAL} movimiento_t;
typedef enum {CIRCULO, RECTANGULO, POLIGONO} geometria_t;

/*
Lee del archivo f un byte y devuelve su color, movimiento y geometria.
Devolve true si todo es correcto.
El encabezado es un byte que codifica el tipo, movimiento y color y los parámetros son 
una sucesión de valores y su cantidad y su significado dependerá del tipo.
Pre: Recibe un archivo abierto.
La función debe recibir punteros válidos(distintos de NULL).
*/
bool leer_encabezado(FILE *f, color_t *color, movimiento_t *movimiento, geometria_t *geometria);

/*
No realiza ninguna accion. 
Devuelva true por el nombre y 0 en n_parametros.
Pre: Recibe un archivo abierto y un vector con memoria para n_parametros 
La función debe recibir punteros válidos(distintos de NULL).
*/
bool leer_movimiento_inmovil(FILE *f, int16_t parametros[], size_t *n_parametros);

/*
Lee del archivo f los 3 parámetros que representan el movimiento circular y los
almacene en parametros, devuelve por el nombre true en caso de estar todo correcto
y por la interfaz n_parametros la cantidad de parámetros leída.
Pre: Recibe un archivo abierto y un vector con memoria para n_parametros 
La función debe recibir punteros válidos(distintos de NULL).
*/
bool leer_movimiento_circular(FILE *f, int16_t parametros[], size_t *n_parametros);

/*
Es análoga a las anteriores pero para el movimiento horizontal.
Pre: Recibe un archivo abierto y un vector con memoria para n_parametros 
La función debe recibir punteros válidos(distintos de NULL).
*/
bool leer_movimiento_horizontal(FILE *f, int16_t parametros[], size_t *n_parametros);

/*
Lee del archivo f el movimiento correspondiente al movimiento dado.
Se usan tablas de búsqueda con las funciones previamente desarrolladas para resolver esta función.
Pre: Recibe un archivo abierto y un vector con memoria para n_parametros 
La función debe recibir punteros válidos(distintos de NULL).
*/
bool leer_movimiento(FILE *f, movimiento_t movimiento, int16_t parametros[], size_t *n_parametros);

/*
Recibe como argumento un archivo abierto, del cual se encarga de extraer la información sobre la geometria
del circulo, es decir, los siguientes 3 parámetros: x, y, radio.
Pre: Recibe un archivo abierto.
Post: Devuelve un puntero a un poligono cargado con los vertices leidos + vertices intermedios
La función debe recibir un puntero válido(distinto de NULL).
*/
poligono_t *leer_geometria_circulo(FILE *f);

/*
Recibe como argumento un archivo abierto, del cual se encarga de extraer la información sobre la geometria
del rectangulo, es decir, los siguientes 5 parámetros: x, y, ancho, alto y angulo.
Pre: Recibe un archivo abierto.
Post: Devuelve un puntero a un poligono cargado con los vertices leidos + vertices intermedios
La función debe recibir un puntero válido(distinto de NULL).
*/
poligono_t *leer_geometria_rectangulo(FILE *f);

/*
Recibe como argumento un archivo abierto, del cual se encarga de extraer la información sobre la geometria
del poligono, es decir, los siguientes parámetros: cantidad de vertices, y cada uno de los vertices.
Pre: Recibe un archivo abierto.
Post: Devuelve un puntero a un poligono cargado con los vertices leidos + vertices intermedios
La función debe recibir un puntero válido(distinto de NULL).
*/
poligono_t *leer_geometria_poligono(FILE *f);

/*
// Recibe una geometria_t y ejecuta la funcion de geometria correspondiente
*/
poligono_t *leer_geometria(FILE*f, geometria_t geometria);

#endif