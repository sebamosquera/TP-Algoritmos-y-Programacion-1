#include "obstaculo.h"
/*
La matriz_color[][] tiene almacenado los colores utilizados en el juego.
Se la utiliza dentro de la función:
void obstaculo_imprimir_SDL2(SDL_Renderer *renderer, obstaculo_t *obstaculo);
para imprimirla con el color leido.
*/
#ifndef MATRIZ_COLOR
#define MATRIZ_COLOR
uint8_t matriz_color[][5] = {
        [AZUL] =    {0, 0, 0XFF, 0},
        [NARANJA] = {0XFF, 0X80, 0, 0},
        [VERDE] =   {0, 0XFF,0 ,0},
        [GRIS] =    {0X9B, 0X9B, 0X9B, 0},
        [AMARILLO]= {0XFF, 0XFF, 0, 0}
};
#endif // MATRIZ_COLOR
#include <stdio.h>

const int puntos[5] = {
    [AZUL] = 10,
    [NARANJA] = 100,
    [VERDE] = 10,
    [GRIS] = 0,
    [AMARILLO] = 0,
};

const int multiplicador[22] = {
    [10] = 2,
    [15] = 3,
    [19] = 5,
    [21] = 10,
};

obstaculo_t *obstaculo_crear(color_t color, movimiento_t movimiento, geometria_t geometria, size_t n_parametros_mov, float *parametros_mov, poligono_t *poligono) {
    
    obstaculo_t *obstaculo = malloc(sizeof(obstaculo_t));
    if (obstaculo == NULL) {
        return NULL;
    }

    obstaculo->color = color;
    obstaculo->movimiento = movimiento;
    obstaculo->geometria = geometria;
    obstaculo->poligono = poligono;
    obstaculo->n_parametros_mov = n_parametros_mov;
    obstaculo->parametros_mov = parametros_mov;
    
    return obstaculo;
}

static void _obstaculo_destruir(obstaculo_t *obstaculo) {
    if(obstaculo != NULL) {
        free(obstaculo->parametros_mov);
        poligono_destruir(obstaculo->poligono);
        free(obstaculo);
    }
}

/// Wrapper
void obstaculo_destruir(void *obstaculo) {
    _obstaculo_destruir(obstaculo);
}

bool obstaculo_aplicar_movimiento_inmovil(obstaculo_t *obstaculo, float *extra[]) {
    return true;
}

bool obstaculo_aplicar_movimiento_circular(obstaculo_t *obstaculo, float *extra[]) {
    float radianes = (3.14) * obstaculo->parametros_mov[2]/180.0;    
    poligono_rotar_centrado(obstaculo->poligono, obstaculo->parametros_mov[0], obstaculo->parametros_mov[1], radianes);
    
    return true;
}

bool obstaculo_aplicar_movimiento_horizontal(obstaculo_t *obstaculo, float *extra[]) {
    float fps = *extra[0];

    float x1 = obstaculo->parametros_mov[0];
    float xi = obstaculo->parametros_mov[1];
    float dx = obstaculo->parametros_mov[2];

    if(x1<xi)
        dx = (-1) * abs(dx);

    if(xi<0)
        dx = (1) * abs(dx);

    xi += dx/fps;
    
    obstaculo->parametros_mov[1] = xi;
    obstaculo->parametros_mov[2] = dx;    

    poligono_trasladar(obstaculo->poligono->vertices, obstaculo->poligono->n, (dx/fps), 0);

    return true;
}

bool (*funciones_de_aplicar_movimiento[])(obstaculo_t *, float **) = {
    [INMOVIL] = obstaculo_aplicar_movimiento_inmovil, 
    [CIRCULAR] = obstaculo_aplicar_movimiento_circular,
    [HORIZONTAL] = obstaculo_aplicar_movimiento_horizontal,
};

static bool _obstaculo_aplicar_movimiento(obstaculo_t *obstaculo, void *extra[]) {
    return (funciones_de_aplicar_movimiento[obstaculo->movimiento])(obstaculo, (float **)extra);  
}

/// Wrapper
bool obstaculo_aplicar_movimiento(void *obstaculo, void *extra[]) {
    return _obstaculo_aplicar_movimiento(obstaculo, extra);
}

static bool _obstaculo_toca_bola(obstaculo_t *obstaculo, float *parametros[]) {

    float cx = *parametros[0];
    float cy = *parametros[1];
    float *norm_x = parametros[2];
    float *norm_y = parametros[3];
    float *vx = parametros[4];
    float *vy = parametros[5];
    float bola_radio = *parametros[6];
    float plasticidad = *parametros[7];
    int *puntaje = (int *)parametros[8];
    int *cant_naranjas = (int *)parametros[9];
    color_t color = obstaculo->color;
    
    if(poligono_distancia(obstaculo->poligono, cx, cy, norm_x, norm_y) <= bola_radio) {
        if(obstaculo->color != GRIS) {
            obstaculo->color = AMARILLO;
        }
        
        reflejar(*norm_x, *norm_y, &cx, &cy, vx, vy);
        *vx = (*vx) * plasticidad;
        *vy = (*vy) * plasticidad;

        // logica de puntaje
        if(color == NARANJA) { 
            *cant_naranjas += 1;
            int multiplo = *cant_naranjas < 22 ? multiplicador[*cant_naranjas] : 0;
            *puntaje *= multiplo ? multiplo : 1;
        }
        *puntaje += puntos[color];
        //

        return false;
    }
    return true;
}

/// Wrapper
bool obstaculo_toca_bola(void *obstaculo, void *parametros[]) {
    return _obstaculo_toca_bola(obstaculo, (float **)parametros);
}

static bool _obstaculo_color(obstaculo_t *obstaculo, color_t color) {
    return (obstaculo->color == color);
}

/// Wrapper
bool obstaculo_color(void *obstaculo, void *color) {
    return _obstaculo_color(obstaculo, (color_t)color);
}

static void _obstaculo_imprimir_SDL2(SDL_Renderer *renderer, obstaculo_t *obstaculo) {//, uint8_t *matriz_color[4]) {
    color_t color_obstaculo = obstaculo->color;
    SDL_SetRenderDrawColor(renderer, matriz_color[color_obstaculo][0], matriz_color[color_obstaculo][1], matriz_color[color_obstaculo][2], matriz_color[color_obstaculo][3]);
    poligono_imprimir_SDL2(renderer, obstaculo->poligono);
}

/// Wrapper
void obstaculo_imprimir_SDL2(void *renderer, void *obstaculo) {
    _obstaculo_imprimir_SDL2(renderer, obstaculo);
} 
