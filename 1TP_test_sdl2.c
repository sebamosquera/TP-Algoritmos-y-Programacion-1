#include <SDL2/SDL.h>
#include <stdbool.h>

#include <assert.h>
#include "config.h"

#include "lista.h"
#include "obstaculo.h"
#include "poligono.h"
#include "leer.h"

#define CANT_NIVELES 7
#define BOLA_VERTICES 7

#define DT (1.0 / JUEGO_FPS)

//#define MAX_MOV_PARAM 4

double computar_velocidad(double vi, double a, double dt) {
    return dt*a + vi; 
}

double computar_posicion(double pi, double vi, double dt) {
    return dt*vi + pi;
}

lista_t *leer_nivel(FILE *f) {

    lista_t *lista_obstaculos = lista_crear();

    int16_t obstaculos;

    fread(&obstaculos, sizeof(int16_t), 1, f);

        for(size_t obstaculo = 0; obstaculo < obstaculos; obstaculo++) {

            color_t color;
            movimiento_t movimiento;
            geometria_t geometria;

            if(! leer_encabezado(f, &color, &movimiento, &geometria)) {
                lista_destruir(lista_obstaculos, obstaculo_destruir);
                return NULL;
            }

            float *parametros_mov = malloc(4 * sizeof(float));
            if(parametros_mov == NULL) {
                lista_destruir(lista_obstaculos, obstaculo_destruir);
                return NULL;
            }

            size_t n_parametros_mov;
            int16_t aux[4];

            if(! leer_movimiento(f, movimiento, aux, &n_parametros_mov)) {
                lista_destruir(lista_obstaculos, obstaculo_destruir);
                return NULL;
            }

            for (size_t i = 0; i < 4; i++) {
                parametros_mov[i] = aux[i];
            }

            poligono_t *poligono = leer_geometria(f, geometria);
            if(poligono == NULL) {
                lista_destruir(lista_obstaculos, obstaculo_destruir);
                return NULL;
            }

            obstaculo_t *obstaculo = obstaculo_crear(color, movimiento, geometria, n_parametros_mov, parametros_mov, poligono);
            if(obstaculo == NULL) {
                poligono_destruir(poligono);
                lista_destruir(lista_obstaculos, obstaculo_destruir);
                return NULL;
            }

            if(! lista_agregar_al_final(lista_obstaculos, obstaculo)) {
                poligono_destruir(poligono);
                obstaculo_destruir(obstaculo);
                lista_destruir(lista_obstaculos, obstaculo_destruir);
                return NULL;
            }
        
        }

        return lista_obstaculos;
}

#ifdef TTF
#include <SDL2/SDL_ttf.h>

void escribir_texto(SDL_Renderer *renderer, TTF_Font *font, const char *s, int x, int y) {
    SDL_Color color = {255, 255, 255};  // EstarÃ­a bueno si la funciÃ³n recibe un enumerativo con el color, Â¿no?
    SDL_Surface *surface = TTF_RenderText_Solid(font, s, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = surface->w;
    rect.h = surface->h;

    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
#endif

int main(int argc, char *argv[]) {

    /// 1) leer algun archivo
    if(argc != 2) {
        fprintf(stderr, "Uso: %s <archivo>\n", argv[0]);
        return 1;
    }
    
    FILE *f = fopen(argv[1], "rb");
    if(f == NULL) {
        fprintf(stderr, "No pudo abrirse \"%s\"\n", argv[1]);
        return 1;
    }
    ///

    char nivel_texto[3] = "1"; /// 999 niveles maximo
    int nivel = 1;
    char puntaje_texto[8] = "0"; /// 10 millones de ptos maximo
    int puntaje = 0;
    int cant_naranjas = 0;

    SDL_Init(SDL_INIT_VIDEO);

#ifdef TTF
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("FreeSansBold.ttf", 24);
#endif

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    SDL_CreateWindowAndRenderer(VENTANA_ANCHO, VENTANA_ALTO, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Peggle");

    int dormir = 0;

    /// 2) leo primer nivel, creo lista y la cargo
    lista_t *lista_obstaculos = leer_nivel(f);
    if(lista_obstaculos == NULL) {
        fprintf(stderr, "Error al leer nivel...");
        return 1;
    }
    ///
    float juego_fps = JUEGO_FPS;
    float bola_radio = BOLA_RADIO;
    float plasticidad = PLASTICIDAD;

    float norm_x;
    float norm_y;

    float canon_angulo = 0;
    bool cayendo = false;

    float cx, cy;
    float vx, vy;

    unsigned int ticks = SDL_GetTicks();

    while(1) {
        if(nivel == CANT_NIVELES) 
            break;
        
        if(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                break;

            if(event.type == SDL_MOUSEBUTTONDOWN) {
                if(! cayendo)
                    cayendo = true;
            }

            else if (event.type == SDL_MOUSEMOTION) {
                canon_angulo = atan2(event.motion.x - CANON_X, event.motion.y - CANON_Y);
                if(canon_angulo > CANON_MAX)
                    canon_angulo = CANON_MAX;
                if(canon_angulo < -CANON_MAX)
                    canon_angulo = -CANON_MAX;
            }

            continue;
        }

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);

        /// 3) muevo obstaculos
        void *extra[] = {&juego_fps}; // parametro que es necesario para mov horizontal
        lista_iterar(lista_obstaculos, obstaculo_aplicar_movimiento, extra);
        ///

        /// 4) dibujo lista
        lista_imprimir_SDL2(renderer, lista_obstaculos, obstaculo_imprimir_SDL2);
        ////

#ifdef TTF
        escribir_texto(renderer, font, "Peggle", 100, 20);
        escribir_texto(renderer, font, "Nivel: ", 200, 20);
        escribir_texto(renderer, font, nivel_texto, 275, 20);
        escribir_texto(renderer, font, "Puntaje: ", 300, 20);
        escribir_texto(renderer, font, puntaje_texto, 405, 20);
#endif

        if(cayendo) {
            vy = computar_velocidad(vy, G, DT);
            vx *= ROZAMIENTO;
            vy *= ROZAMIENTO;
            cx = computar_posicion(cx, vx, DT);
            cy = computar_posicion(cy, vy, DT);
        }
        else {
            cx = CANON_X + CANON_LARGO * sin(canon_angulo);
            cy = CANON_Y + CANON_LARGO * cos(canon_angulo);
            vx = BOLA_VI * sin(canon_angulo);
            vy = BOLA_VI * cos(canon_angulo);
        }

        if(cx < MIN_X + BOLA_RADIO || cx > MAX_X - BOLA_RADIO) vx = - vx;
        if(cy < MIN_Y + BOLA_RADIO) vy = -vy;
    
        if(cy > MAX_Y - BOLA_RADIO) {
            /// 5) elimino obstaculos de color amarillo
            lista_eliminar_condicionalmente(lista_obstaculos, obstaculo_color, (void *)AMARILLO, obstaculo_destruir);
            ///
            cayendo = false;
        }

        SDL_RenderDrawLine(renderer, CANON_X, CANON_Y, CANON_X + sin(canon_angulo) * CANON_LARGO, CANON_Y + cos(canon_angulo) * CANON_LARGO);

        /// creo y dibujo bola (circulo)
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);
        poligono_t *bola = poligono_crear_circulo(cx, cy, BOLA_RADIO, BOLA_VERTICES);
        if(bola != NULL)
            poligono_imprimir_SDL2(renderer, bola);
        poligono_destruir(bola);
        /*
        SDL_RenderDrawLine(renderer, cx - BOLA_RADIO, cy, cx, cy + BOLA_RADIO);
        SDL_RenderDrawLine(renderer, cx, cy + BOLA_RADIO, cx + BOLA_RADIO, cy);
        SDL_RenderDrawLine(renderer, cx + BOLA_RADIO, cy, cx, cy - BOLA_RADIO);
        SDL_RenderDrawLine(renderer, cx, cy - BOLA_RADIO, cx - BOLA_RADIO, cy);
        */

        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0x00);
        SDL_RenderDrawLine(renderer, MIN_X, MIN_Y, MAX_X, MIN_Y);
        SDL_RenderDrawLine(renderer, MIN_X, MAX_Y, MAX_X, MAX_Y);
        SDL_RenderDrawLine(renderer, MIN_X, MAX_Y, MIN_X, MIN_Y);
        SDL_RenderDrawLine(renderer, MAX_X, MAX_Y, MAX_X, MIN_Y);

        SDL_RenderDrawLine(renderer, cx, cy, cx + vx, cy + vy);

        /// 6) detecto choques
        void *parametros[] = {&cx, &cy, &norm_x, &norm_y, &vx, &vy, &bola_radio, &plasticidad, &puntaje, &cant_naranjas};
        lista_iterar(lista_obstaculos, obstaculo_toca_bola, parametros);
        ///

        /// 7) Actualizo puntaje
        sprintf(puntaje_texto, "%d", puntaje);
        sprintf(nivel_texto, "%d", nivel);
        ///
        
        /// 8) leo siguiente nivel si este termino
        if(lista_no_contiene(lista_obstaculos, obstaculo_color, (void *)NARANJA)) {
            lista_destruir(lista_obstaculos, obstaculo_destruir);
            cayendo = false;
            lista_obstaculos = leer_nivel(f);
            nivel++;
        }
        ///
        
        SDL_RenderPresent(renderer);
        ticks = SDL_GetTicks() - ticks;
        if(dormir) {
            SDL_Delay(dormir);
            dormir = 0;
        }
        else if(ticks < 1000 / JUEGO_FPS)
            SDL_Delay(1000 / JUEGO_FPS - ticks);
        ticks = SDL_GetTicks();
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    /// 9) destruyo la ultima lista que quedo
    lista_destruir(lista_obstaculos, obstaculo_destruir);
    ///
    
    /// 10) cierro el archivo
    fclose(f);
    ///

#ifdef TTF
    TTF_CloseFont(font);
    TTF_Quit();
#endif
    SDL_Quit();
    return 0;
}
