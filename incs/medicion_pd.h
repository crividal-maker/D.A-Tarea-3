#ifndef MEDICION_PD_H
#define MEDICION_PD_H

#include "deportista.h"

// Tamaños de entrada a evaluar
#define TAMANIOS_PD 5
static const int TAMANIOS[] = {10, 100, 500, 1000, 5000};

// Parámetros fijos para comparación
#define W_FIJO 500
#define K_FIJO 5

// Ejecuta todas las mediciones y guarda resultados en CSV
void medicion_pd_general();

#endif // MEDICION_PD_H