#ifndef POLIGONO_H
#define POLIGONO_H

#include <SDL2/SDL.h>

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct poligono {
    float (*vertices)[2];
    size_t n;
} poligono_t;

// Crea un poligono
// Post: Devuelve un nuevo poligono
poligono_t *poligono_crear(float vertices[][2], size_t n);

// Crea un poligono circulo en base a la cantidad de vertices que recibe
// Recibe una componente de centro en x, una en y, un radio y una cant de vertices
poligono_t *poligono_crear_circulo(float cx, float cy, int r, int n_vertices);

// Destruye el poligono
// Pre: Recibe un puntero a poligono valido (puntero distinto de NULL)
void poligono_destruir(poligono_t *poligono);

// Devuelve la cantida de vertices del poligono recibido
// Pre: Recibe un puntero a un poligono valido
size_t poligono_cantidad_vertices(const poligono_t *poligono);

// Devuelve el vertice cuta posicion es la recibida
// Pre: Recibe un puntero a un poligono valido
bool poligono_obtener_vertice(const poligono_t *poligono, size_t pos, float *x, float *y);

// Agrega un vertice al poligono
// Pre: Recibe un puntero a un poligono valido
bool poligono_agregar_vertice(poligono_t *poligono, float x, float y);

// Recibe un puntero a un poligono, lo clona y devuelvo un nuevo poligono
poligono_t *poligono_clonar(const poligono_t *poligono);

// Rota el poligono recibido segun el angulo recibido en radianes
// Pre: Recibe un poligono, la cantidad de vertices y un angulo en radianes
// Post: Devuelve el poligono rotado
void poligono_rotar(float poligono[][2], size_t n, double rad);

// Traslada el poligono recibido segun el la distancia recibida
// Pre: Recibe un poligono, la cantidad de vertices y una distancia en x e y
// Post: Devuelve el poligono trasladado
void poligono_trasladar(float poligono[][2], size_t n, float dx, float dy);

// Traslada y rota el poligono recibido segun el angulo en radianes y el centro de rotacion recibido
// Pre: Recibe un poligono, la cantidad de vertices, un centro de rotacion y un angulo en radianes
// Post: Devuelve el poligono rotado segun el centro cx, cy
void poligono_rotar_centrado(poligono_t *p, float cx, float cy, double radianes);

/*
Recibe un polígono p, un punto (cx, cy) y devuelve la distancia del punto al polígono por el nombre 
y las componentes (norm_x, norm_y) de la normal al polígono en el punto más cercano.
Permite evaluar si hay un impacto y la dirección contra la que debe rebotar la bola.
*/
double poligono_distancia(const poligono_t *p, float xp, float yp, float *nor_x, float *nor_y);

// Reflejamos según P' = P - 2 D(P.D)
void reflejar(float norm_x, float norm_y, float *cx, float *cy, float *vx, float *vy);

/*
Para encontrar la distancia de un punto a una recta se proyecta el punto
ortogonalmente sobre la recta.
El producto [(X.P) / (X.X)] X es la proyección del punto P sobre X.
El coeficiente entre corchetes será la proporción de P sobre X.
Como estamos trabajando con segmentos de recta, si el coeficiente es menor a
cero o mayor a uno nos caímos del segmento.
*/
void punto_mas_cercano(float x0, float y0, float x1, float y1, float xp, float yp, float *x, float *y);

float producto_interno(float norm_x, float norm_y, float vx, float vy);

double distancia(float xp, float yp, float xi, float yi);

// Imprime un poligono en la ventana
// Recibe punteros validos (distintos de NULL)
bool poligono_imprimir_SDL2(SDL_Renderer *renderer, poligono_t *poligono);

#endif