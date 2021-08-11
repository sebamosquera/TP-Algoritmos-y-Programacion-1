#include "poligono.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

poligono_t *poligono_crear(float vertices[][2], size_t n) {

	poligono_t *poligono = malloc(sizeof(poligono_t));

	if(poligono == NULL) {
		return NULL;
	}

	poligono->n = n;

	poligono->vertices = malloc(2*n*sizeof(float));

	if(poligono->vertices == NULL) {
		poligono_destruir(poligono);
		return NULL;
	}

    memcpy(poligono->vertices, vertices, 2*n*sizeof(float));

	return poligono;
}

poligono_t *poligono_crear_circulo(float cx, float cy, int r, int n_vertices) {
    float rad = (360/n_vertices) * 3.14125 / 180;
   
    float vertices[n_vertices][2];

    for(size_t i = 0; i < n_vertices; i++) {
        vertices[i][0] = cx + r * cos(rad * i);
        vertices[i][1] = cy + r * sin(rad * i);
    }

    poligono_t *p = poligono_crear(vertices, n_vertices);
    if(p == NULL) return NULL;

    return p;
}

void poligono_destruir(poligono_t *poligono) {
	if (poligono) {
		if (poligono->vertices) {
			free(poligono->vertices);
		}
		free(poligono);
	}
}

size_t poligono_cantidad_vertices(const poligono_t *poligono) {
	return poligono->n;
}

bool poligono_obtener_vertice(const poligono_t *poligono, size_t pos, float *x, float *y) {

	if(pos > ((poligono->n) - 1)) {
		return false;
	}

	*x = poligono->vertices[pos][0];
	*y = poligono->vertices[pos][1];

	return true;
}

bool poligono_agregar_vertice(poligono_t *poligono, float x, float y) {

	float (*aux)[2] = malloc((2*((poligono->n) + 1)*sizeof(float)));

	if(aux == NULL) {
		return false;
	}

	memcpy(aux, poligono->vertices, (2*(poligono->n)*sizeof(float)));

	free(poligono->vertices);

	poligono->vertices = aux;

	poligono->vertices[(poligono->n)][0] = x;

	poligono->vertices[(poligono->n)][1] = y;

	(poligono->n) = (poligono->n) + 1;

	return true;
 
}

poligono_t *poligono_clonar(const poligono_t *poligono) {

	if (poligono == NULL) {
		return NULL;
	}

	poligono_t *clon_poligono = poligono_crear(poligono->vertices, poligono->n);

	return clon_poligono;

}

void poligono_trasladar(float poligono[][2], size_t n, float dx, float dy){
	for(size_t i = 0; i<n; i++) {
		poligono[i][0] += dx;
		poligono[i][1] += dy;
	}
}

void poligono_rotar(float poligono[][2], size_t n, double rad){
	float coseno = cos(rad);
	float seno = sin(rad);

	for(size_t i = 0; i<n; i++) {
		float aux_x = poligono[i][0]; 
		poligono[i][0] = aux_x*coseno - poligono[i][1]*seno;
		poligono[i][1] = aux_x*seno + poligono[i][1]*coseno;
	}
}

void poligono_rotar_centrado(poligono_t *p, float cx, float cy, double radianes) {
	poligono_trasladar(p->vertices, p->n, -cx, -cy);
    poligono_rotar(p->vertices, p->n, radianes);		
    poligono_trasladar(p->vertices, p->n, cx, cy);
}

double poligono_distancia(const poligono_t *p, float xp, float yp, float *nor_x, float *nor_y) {
    double d = 1 / 0.0;
    size_t idx = 0;

    for(size_t i = 0; i < p->n; i++) {
        float xi, yi;
        punto_mas_cercano(p->vertices[i][0], p->vertices[i][1], p->vertices[(i + 1) % p->n][0], p->vertices[(i + 1) % p->n][1], xp, yp, &xi, &yi);
        double di = distancia(xp, yp, xi, yi);

        if(di < d) {
            d = di;
            idx = i;
        }
    }
    
    float nx = p->vertices[(idx + 1) % p->n][1] - p->vertices[idx][1];
    float ny = p->vertices[idx][0] - p->vertices[(idx + 1) % p->n][0];
    float dn = distancia(nx, ny, 0, 0);

    nx /= dn;
    ny /= dn;

    *nor_x = nx;
    *nor_y = ny;

    return d;
}

void reflejar(float norm_x, float norm_y, float *cx, float *cy, float *vx, float *vy) {
    float proy = producto_interno(norm_x, norm_y, *vx, *vy);

    if(proy >= 0)
        return;

    *vx -= 2 * norm_x * proy;
    *vy -= 2 * norm_y * proy;

    *cx += norm_x * 0.1;
    *cy += norm_y * 0.1;
}

void punto_mas_cercano(float x0, float y0, float x1, float y1, float xp, float yp, float *x, float *y) {
    float ax = xp - x0;
    float ay = yp - y0;
    float bx = x1 - x0;
    float by = y1 - y0;

    float alfa = producto_interno(ax, ay, bx, by) / producto_interno(bx, by, bx, by);

    if(alfa <= 0) {
        *x = x0;
        *y = y0;
    }
    else if(alfa >= 1) {
        *x = x1;
        *y = y1;
    }
    else {
        *x = alfa * bx + x0;
        *y = alfa * by + y0;
    }
}

double distancia(float xp, float yp, float xi, float yi) {
    return sqrtf(((xp - xi)*(xp - xi)) + ((yp - yi)*(yp - yi)));
}

float producto_interno(float norm_x, float norm_y, float vx, float vy) {
    return (norm_x * vx) + (norm_y * vy);
}

bool poligono_imprimir_SDL2(SDL_Renderer *renderer, poligono_t *poligono) {    
    size_t i;
    
    for (i = 0; i < (poligono->n - 1); i++) {
        SDL_RenderDrawLine(renderer, poligono->vertices[i][0], poligono->vertices[i][1], poligono->vertices[i + 1][0], poligono->vertices[i + 1][1]);
    }

    SDL_RenderDrawLine(renderer, poligono->vertices[i][0], poligono->vertices[i][1], poligono->vertices[0][0], poligono->vertices[0][1]);

    return true;

}