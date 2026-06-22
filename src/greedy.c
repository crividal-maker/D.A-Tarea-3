#include "greedy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =====================================================================
// FUNCIONES DE UTILIDAD
// =====================================================================

void liberar_resultado_greedy(ResultadoGreedy *resultado) {
    if (resultado->indices != NULL) {
        free(resultado->indices);
        resultado->indices = NULL;
    }
}

void imprimir_resultado_greedy(ResultadoGreedy *resultado, Deportista *arreglo, const char *nombre_criterio) {
    printf("\n--- EQUIPO OPTIMO (Greedy: %s) ---\n", nombre_criterio);
    printf("Deportistas seleccionados: %d\n", resultado->cantidad);
    printf("Puntaje total: %.1f\n", resultado->puntaje_total);
    printf("Costo total:   %d\n\n", resultado->costo_total);
    for (int i = 0; i < resultado->cantidad; i++) {
        imprimir_deportista(arreglo[resultado->indices[i]]);
    }
}

// =====================================================================
// MAYOR RATIO PUNTAJE/COSTO (Nicolás)
// =====================================================================

// Variables auxiliares para el comparador
static Deportista *arreglo_global = NULL;

static int comparar_ratio_real(const void *a, const void *b) {
    int idx_a = *(int *)a;
    int idx_b = *(int *)b;
    double ratio_a = (double)arreglo_global[idx_a].puntaje / arreglo_global[idx_a].costo;
    double ratio_b = (double)arreglo_global[idx_b].puntaje / arreglo_global[idx_b].costo;
    if (ratio_b > ratio_a) return 1;
    if (ratio_b < ratio_a) return -1;
    return 0;
}

ResultadoGreedy greedy_mayor_ratio(Deportista *arreglo, int n, int w) {
    ResultadoGreedy resultado;
    resultado.puntaje_total = 0.0f;
    resultado.costo_total   = 0;
    resultado.cantidad      = 0;
    resultado.indices       = malloc(n * sizeof(int));

    // Crear arreglo de índices y ordenar por ratio descendente
    int *orden = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) orden[i] = i;

    arreglo_global = arreglo;
    qsort(orden, n, sizeof(int), comparar_ratio_real);

    // Seleccionar deportistas mientras haya presupuesto
    int presupuesto_restante = w;
    for (int i = 0; i < n; i++) {
        int idx = orden[i];
        if (arreglo[idx].costo <= presupuesto_restante) {
            resultado.indices[resultado.cantidad++] = idx;
            resultado.puntaje_total += arreglo[idx].puntaje;
            resultado.costo_total   += arreglo[idx].costo;
            presupuesto_restante    -= arreglo[idx].costo;
        }
    }

    free(orden);
    return resultado;
}

// =====================================================================
// MAYOR PUNTAJE PRIMERO (Guillermo)
// =====================================================================
//...

// =====================================================================
// MENOR COSTO PRIMERO (Cristóbal)
// =====================================================================
//
// Idea: ordenar a todos los deportistas por costo ascendente y tomarlos
// en ese orden mientras no se supere el presupuesto W.
//
// Esto maximiza la CANTIDAD de deportistas que entran al equipo, pero NO
// necesariamente el puntaje total -> en el informe hay que mostrar un
// contraejemplo donde este criterio NO es optimo (ej: tomar 3 baratos y
// malos deja el mismo presupuesto que tomar 1 caro y muy bueno con mas
// puntaje total).
//
// Complejidad:
//   Tiempo:  O(n log n)  -> dominado por qsort
//   Espacio: O(n)        -> arreglo auxiliar de indices para ordenar

typedef struct {
    int indice;
    int costo;
    float puntaje;
} ItemAuxiliarCosto;

static int comparar_por_costo(const void *a, const void *b) {
    const ItemAuxiliarCosto *ia = (const ItemAuxiliarCosto *)a;
    const ItemAuxiliarCosto *ib = (const ItemAuxiliarCosto *)b;

    if (ia->costo != ib->costo) {
        return ia->costo - ib->costo;
    }
    // Empate en costo -> desempata por mayor puntaje (no cambia el criterio,
    // solo mejora la calidad de la solucion en caso de empate)
    if (ia->puntaje < ib->puntaje) return 1;
    if (ia->puntaje > ib->puntaje) return -1;
    return 0;
}

ResultadoGreedy greedy_menor_costo(Deportista *arreglo, int n, int w) {
    ResultadoGreedy resultado;
    resultado.puntaje_total = 0.0f;
    resultado.costo_total = 0;
    resultado.cantidad = 0;
    resultado.indices = malloc(n * sizeof(int));

    ItemAuxiliarCosto *items = malloc(n * sizeof(ItemAuxiliarCosto));
    for (int i = 0; i < n; i++) {
        items[i].indice = i;
        items[i].costo = arreglo[i].costo;
        items[i].puntaje = arreglo[i].puntaje;
    }

    qsort(items, n, sizeof(ItemAuxiliarCosto), comparar_por_costo);

    int presupuesto_restante = w;
    for (int i = 0; i < n; i++) {
        if (items[i].costo <= presupuesto_restante) {
            int idx = items[i].indice;
            resultado.indices[resultado.cantidad++] = idx;
            resultado.puntaje_total += arreglo[idx].puntaje;
            resultado.costo_total += arreglo[idx].costo;
            presupuesto_restante -= items[i].costo;
        }
        // Si no entra, se descarta (greedy no vuelve atras)
    }

    free(items);
    return resultado;
}

// =====================================================================
// SIN RESTRICCIÓN: K MEJORES (Guillermo)
// =====================================================================
//...
