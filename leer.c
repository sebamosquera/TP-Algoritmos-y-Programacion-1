#include "leer.h"
#include "poligono.h"
#include <math.h>

#define VERTICES_CIRCULO 15
#define N_R 15

const char *colores[] = {[AZUL] = "Azul",[NARANJA] = "Naranja", [VERDE] = "Verde", [GRIS] = "Gris"};

bool leer_encabezado(FILE *f, color_t *color, movimiento_t *movimiento, geometria_t *geometria) {

	uint8_t encabezado;
	if(fread(&encabezado, sizeof(uint8_t), 1, f) != 1) return false;

	*color = (encabezado & MSK_COLOR) >> SHIFT_COLOR;
	*movimiento = (encabezado & MSK_MOVIMIENTO) >> SHIFT_MOVIMIENTO;
	*geometria = (encabezado & MSK_GEOMETRIA) >> SHIFT_GEOMETRIA;

    if(*movimiento > 3 || *geometria > 3) return false;

	return true;
}

bool leer_movimiento_inmovil(FILE *f, int16_t parametros[], size_t *n_parametros) {
	
	if( (f == NULL) || (parametros == NULL) || (n_parametros) == NULL) return false;

	*n_parametros = 0;
	return true;
}

bool leer_movimiento_circular(FILE *f, int16_t parametros[], size_t *n_parametros) {
	*n_parametros = 3;

	return (fread(parametros,sizeof(int16_t), 3, f) == 3);
}

bool leer_movimiento_horizontal(FILE *f, int16_t parametros[], size_t *n_parametros) {
	*n_parametros = 3;

	return (fread(parametros, sizeof(int16_t), 3, f) == 3);
}

bool (*funciones_de_movimiento[])(FILE *, int16_t [], size_t *) = {
    [INMOVIL] = leer_movimiento_inmovil,
    [CIRCULAR] = leer_movimiento_circular,
    [HORIZONTAL] = leer_movimiento_horizontal
};

bool leer_movimiento(FILE *f, movimiento_t movimiento, int16_t parametros[], size_t *n_parametros) {
	return (funciones_de_movimiento[movimiento])(f, parametros, n_parametros);  
}

poligono_t *leer_geometria_circulo(FILE *f) {

    int16_t parametros[3];
    if(fread(parametros, sizeof(int16_t), 3, f) != 3)
        return NULL;
    
    int16_t cx = parametros[0];
    int16_t cy = parametros[1];
    int16_t r = parametros[2];

    float rad = (360/VERTICES_CIRCULO) * 3.14125 / 180;
   
    float vertices[VERTICES_CIRCULO][2];

    for(size_t i = 0; i < VERTICES_CIRCULO; i++) {
        vertices[i][0] = cx + r * cos(rad * i);
        vertices[i][1] = cy + r * sin(rad * i);
    }

    poligono_t *p = poligono_crear(vertices, VERTICES_CIRCULO);
    if(p == NULL) return NULL;

    return p;
}

poligono_t *leer_geometria_rectangulo(FILE *f) {

    int16_t parametros[5];
    if(fread(parametros, sizeof(int16_t), 5, f) != 5)
        return NULL;
    
    int16_t x = parametros[0];
    int16_t y = parametros[1];
    int16_t alto = parametros[2];
    int16_t ancho = parametros[3];
    float angulo = parametros[4];

    float vertices[N_R * 5][2];
    
    size_t i = 0;

    float puntos[4][2]= {{ x - alto/2, y - ancho/2}, {x + alto/2, y - ancho/2}, {x + alto/2, y + ancho/2}, {x - alto/2, y + ancho/2}};

    for(i = 0; i < 4; i++) {
        float dx = (puntos[i + 1][0] - puntos[i][0]) / (float)N_R;
        float dy = (puntos[i + 1][1] - puntos[i][1]) / (float)N_R;

        float aux_x = puntos[i][0];
        float aux_y = puntos[i][1];

        for (size_t j = 0; j < N_R; j++) {
            vertices[j + (N_R * i)][0] = aux_x;
            vertices[j + (N_R * i)][1] = aux_y;
            aux_x += dx;
            aux_y += dy;            
        }
    }

    float aux_x = puntos[3][0];
    float aux_y = puntos[3][1];

    for (size_t j = 0; j < N_R; j++) {
        vertices[j + (N_R * i)][0] = aux_x;
        vertices[j + (N_R * i)][1] = aux_y;
    }
           
    poligono_t *p = poligono_crear(vertices, N_R * 3);
    if(p == NULL) {
        return NULL;
    }

   	p->n=N_R*3;

   
    float radianes = ((3.14 * angulo) / (180));
    poligono_rotar_centrado(p, x, y, radianes);
    return p;
}

poligono_t *leer_geometria_poligono(FILE *f) {
    int16_t n;
    if(fread(&n, sizeof(int16_t), 1, f) != 1) {
        return NULL;
    }

    int16_t leido[n][2];
    
    if(fread(leido, sizeof(int16_t), 2 * n, f) != (2 * n))
		return NULL;

    float vertices[n][2];

    // si no lo hago de esta forma hay un error de casteo
    for (int i = 0; i < n; i++) {
    	vertices[i][0] = leido[i][0];
    	vertices[i][1] = leido[i][1];
    }

    poligono_t *p = poligono_crear(vertices, n);
    if(p == NULL) return NULL;

    p->n = n;

    return p;
}

poligono_t *(*funciones_de_geometria[])(FILE *) = {
    [CIRCULO] = leer_geometria_circulo,
    [RECTANGULO] = leer_geometria_rectangulo,
    [POLIGONO] = leer_geometria_poligono
};

poligono_t *leer_geometria(FILE*f, geometria_t geometria) {
	return funciones_de_geometria[geometria](f);  
}