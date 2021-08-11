#ifndef LISTA_H
#define LISTA_H

#include <SDL2/SDL.h>

#include <stdlib.h>
#include <stdbool.h>

struct lista;
typedef struct lista lista_t;

// Crea una lista
// Post: Se devuelve una nueva lista.
lista_t *lista_crear(void);

// Destruye la lista
// Pre: Recibe un lista no vacia, y un puntero a una funcion para destruir el tipo de dato especifico
void lista_destruir(lista_t *lista, void destruir_dato(void *));

// Agrega un nodo al final de la lista
// Pre: Recibe un lista no vacia (puntero distinto de NULL)
//      Recibe un nodo creado con un dato
// Post: Se agregó un nodo al final de la lista
bool lista_agregar_al_final(lista_t *lista, void *dato);

// Obtener largo
// Pre: La lista fue creada
// Post: Devuelve la cantidad de elementos almacenados
size_t lista_largo(lista_t *lista);

////// nuevas
// Itera la lista y aplica la funcion visitar a cada nodo
void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra[]), void *parametros[]);

// Itera la lista y aolica la funcion condicional con el parametro condicion
// si la funcion condicional devuelve true se elimina el elemento
bool lista_eliminar_condicionalmente(lista_t *lista, bool (*funcion_condicional)(void *, void *), void* condicion, void destruir_dato(void *));

// Itera la lista y aplica la funcion de impresion especifica para el tipo de dato
void lista_imprimir_SDL2(SDL_Renderer *renderer, lista_t *lista, void (*imprimir_dato)(void *, void *));

// Itera la lista y aplica la funcion condicional
// devuelve true si la lista no contiene ningun elemento que cumpla con la condicion
bool lista_no_contiene(lista_t *lista, bool (*funcion_condicional)(void *, void *), void* condicion);

#endif // LISTA_H