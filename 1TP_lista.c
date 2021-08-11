#include"lista.h"

typedef struct nodo {
    void *dato;
    struct nodo *prox;
} nodo_t;

struct lista {
    nodo_t *prim;
};

lista_t *lista_crear(void) {
    lista_t *lista = malloc(sizeof(lista_t));
    if (lista == NULL) return NULL;
    lista->prim = NULL;
    return lista;
}

void lista_destruir(lista_t *lista, void destruir_dato(void *)) {
    while(lista->prim) {
        nodo_t *nodo = lista->prim;
        if(destruir_dato)
            destruir_dato(nodo->dato);
        lista->prim = nodo->prox;
        free(nodo);
    }
    free(lista);
}

static nodo_t *crear_nodo(void *dato, nodo_t *prox) {
    nodo_t *nuevo = malloc(sizeof(nodo_t));

    if(nuevo == NULL)
        return NULL;
    
    nuevo->dato = dato;
    nuevo->prox = prox;

    return nuevo;
}
  
bool lista_agregar_al_final(lista_t *lista, void *dato) {
    nodo_t *nuevo = crear_nodo(dato, NULL);

    if(nuevo == NULL)
        return false;
   
    nodo_t *actual = lista->prim;
    
    if (actual == NULL)
        lista->prim = nuevo;
       
    else {
        while (actual->prox != NULL) {
            actual = actual->prox;
        }
        actual->prox = nuevo;
    }

    return true;
}

size_t lista_largo(lista_t *lista) {
    size_t n  = 0;
    nodo_t *actual = lista->prim;

    while(actual!=NULL) {
        n++;
        actual = actual->prox;
    }

    return n;
}

void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra[]), void *parametros[]) {
    nodo_t *nodo_act = lista->prim;
    while(nodo_act) {
        if(! visitar(nodo_act->dato, parametros))
            return;
        nodo_act = nodo_act->prox;
    }
}

bool lista_eliminar_condicionalmente(lista_t *lista, bool (*funcion_condicional)(void *, void *), void* condicion, void destruir_dato(void *)) {
    nodo_t *act = lista->prim;

    if(act != NULL) {
        while(act->prox != NULL) {
            if(funcion_condicional(act->prox->dato, condicion)) {
                nodo_t *aux = act->prox;
                act->prox = act->prox->prox;
                destruir_dato(aux->dato);
                free(aux);
            }
            else
                act = act->prox;
        }
    }

    nodo_t *primero = lista->prim;

    if(primero != NULL && funcion_condicional(primero->dato, condicion)) {
        lista->prim = primero->prox;
        destruir_dato(primero->dato);
        free(primero);
    }

    return true; 
}

bool lista_no_contiene(lista_t *lista, bool (*funcion_condicional)(void *, void *), void* condicion) {
    nodo_t *act = lista->prim;
    
    if(act == NULL) {
        return true;
    }

    while(act != NULL) {
        if(funcion_condicional(act->dato, condicion)) {
            return false;
        }
        act = act->prox;
    }
    return true;
}

void lista_imprimir_SDL2(SDL_Renderer *renderer, lista_t *lista, void (*imprimir_dato)(void *, void *)) {
    nodo_t *actual = lista->prim;
    while(actual != NULL) {
        imprimir_dato(renderer, actual->dato);
        actual = actual->prox;
    }
}
