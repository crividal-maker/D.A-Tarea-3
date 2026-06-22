#ifndef PD_H
#define PD_H

#include "deportista.h"

// =====================================================================
// PROGRAMACIÓN DINÁMICA
// =====================================================================

typedef struct {
    float puntaje_total;    // Puntaje total del equipo seleccionado
    int costo_total;        // Costo total del equipo seleccionado
    int cantidad;           // Cantidad de deportistas seleccionados
    int *indices;           // Índices de los deportistas seleccionados
    int factible;           // 1 si existe solución, 0 si no
} ResultadoPD;

// =====================================================================
// MEMOIZACIÓN (TOP-DOWN)
// =====================================================================

// Función principal: selecciona exactamente k deportistas
// maximizando puntaje sin superar presupuesto w

// Retorna ResultadoPD con la solución óptima
ResultadoPD pd_memoizacion(Deportista *arreglo, int n, int w, int k);

// Liberar memoria del resultado e imprimirla
void liberar_resultado_pd(ResultadoPD *resultado);
void imprimir_resultado_pd(ResultadoPD *resultado, Deportista *arreglo);

#endif // PD_H