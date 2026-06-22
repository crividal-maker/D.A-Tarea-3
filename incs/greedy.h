#ifndef GREEDY_H
#define GREEDY_H

#include "deportista.h"

// =====================================================================
// RESULTADO GREEDY
// =====================================================================

typedef struct {
    float puntaje_total;
    int costo_total;
    int cantidad;
    int *indices;
} ResultadoGreedy;

void liberar_resultado_greedy(ResultadoGreedy *resultado);
void imprimir_resultado_greedy(ResultadoGreedy *resultado, Deportista *arreglo, const char *nombre_criterio);

// =====================================================================
// CON RESTRICCIÓN DE PRESUPUESTO (W)
// =====================================================================

// Mayor puntaje primero (Guillermo)
ResultadoGreedy greedy_mayor_puntaje(Deportista *arreglo, int n, int w);

// Menor costo primero (Cristóbal)
ResultadoGreedy greedy_menor_costo(Deportista *arreglo, int n, int w);

// Mayor ratio puntaje/costo primero (Nicolás)
ResultadoGreedy greedy_mayor_ratio(Deportista *arreglo, int n, int w);

// =====================================================================
// SIN RESTRICCIÓN DE PRESUPUESTO (selecciona exactamente K)
// =====================================================================

// Selecciona exactamente K deportistas con mayor puntaje (Guillermo)
ResultadoGreedy greedy_sin_restriccion(Deportista *arreglo, int n, int k);

#endif // GREEDY_H