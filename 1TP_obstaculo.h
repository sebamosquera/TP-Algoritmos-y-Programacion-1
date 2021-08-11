#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <SDL2/SDL.h>

#include "poligono.h"
#include "leer.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct obstaculo {
    color_t color;
    movimiento_t movimiento;
    geometria_t geometria;
    poligono_t *poligono;
    float *parametros_mov;
    size_t n_parametros_mov;
} obstaculo_t;

// Crea un obstaculo
// Pre: Recibe un poligono creado, un puntero a parametros_mov, y el resto de variables para inicializar un obstaculo
// Post: Se devuelve un nuevo obstaculo inicializado con los argumentos recibidos
obstaculo_t *obstaculo_crear(color_t color, movimiento_t movimiento, geometria_t geometria, size_t n_parametros_mov, float *parametros_mov, poligono_t *poligono);

// Destruye el obstaculo
void obstaculo_destruir(void *obstaculo);

// Arreglo de funciones de movimiento
typedef void (*aplicar_movimiento_t)(obstaculo_t *); 

// Aplica el movimiento inmovil
// Pre: Recibe un obstaculo cargado (distinto de NULL)
bool obstaculo_aplicar_movimiento_inmovil(obstaculo_t *obstaculo, float *extra[]);

// Aplica un movimiento circular y actualiza la posicion del obstaculo recibido
// Pre: Recibe un obstaculo cargado (distinto de NULL)
bool obstaculo_aplicar_movimiento_circular(obstaculo_t *obstaculo, float *extra[]);

// Aplica un movimiento horizontal y actualiza la posicion del obstaculo recibido
// Pre: Recibe un obstaculo cargado (distinto de NULL)
bool obstaculo_aplicar_movimiento_horizontal(obstaculo_t *obstaculo, float *extra[]);

// Recibe un movimiento_t y ejecuta la funcion de movimiento correspondiente
// Pre: Recibe un obstaculo cargado (distinto de NULL)
bool (*funciones_de_aplicar_movimiento[3])(obstaculo_t *, float *extra[]);

// Recibe un obstaculo y llama a funciones_de_aplicar_movimiento pasandole el movimiento del obstaculo
// Pre: Recibe un obstaculo cargado (distinto de NULL)
bool obstaculo_aplicar_movimiento(void *obstaculo, void *extra[]);

// Si el obstaculo recibido fue tocado por la bola lo cambia de color a amarillo
// Pre: Recibe un obstaculo cargado (distinto de NULL)
// Post: Devuelve por la interfaz la normal, la velocidad actualizada (se aplica el factor de PLASTICIDAD)
bool obstaculo_toca_bola(void *obstaculo, void *parametros[]);

// Imprime un obstaculo en la ventana
// Pre: Recibe punteros validos
void obstaculo_imprimir_SDL2(void *renderer, void *obstaculo);

// Recibe un obstaculo y un color
// Devuleve true si el obstaculo es de ese color
bool obstaculo_color(void *obstaculo, void* color);

#endif // OBSTACULO_H