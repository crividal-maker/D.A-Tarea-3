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

// Arreglo original, usado por los comparadores de qsort
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

    // Ordenar los indices por ratio puntaje/costo descendente
    int *orden = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) orden[i] = i;

    arreglo_global = arreglo;
    qsort(orden, n, sizeof(int), comparar_ratio_real);

    // Tomar deportistas en ese orden mientras quepan en el presupuesto
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

// Ordena indices por puntaje descendente; en caso de empate, por menor costo
static int comparar_por_puntaje(const void *a, const void *b) {
    int idx_a = *(int *)a;
    int idx_b = *(int *)b;

    if (arreglo_global[idx_a].puntaje < arreglo_global[idx_b].puntaje) return 1;
    if (arreglo_global[idx_a].puntaje > arreglo_global[idx_b].puntaje) return -1;

    return arreglo_global[idx_a].costo - arreglo_global[idx_b].costo;
}

ResultadoGreedy greedy_mayor_puntaje(Deportista *arreglo, int n, int w) {
    ResultadoGreedy resultado;
    resultado.puntaje_total = 0.0f;
    resultado.costo_total   = 0;
    resultado.cantidad      = 0;
    resultado.indices       = malloc(n * sizeof(int));

    // Indices auxiliares para no alterar el orden original del arreglo
    int *orden = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        orden[i] = i;
    }

    arreglo_global = arreglo;
    qsort(orden, n, sizeof(int), comparar_por_puntaje);

    // Selección voraz: se toma cada deportista si entra en el presupuesto restante
    int presupuesto_restante = w;
    for (int i = 0; i < n; i++) {
        int idx = orden[i];

        if (arreglo[idx].costo <= presupuesto_restante) {
            resultado.indices[resultado.cantidad++] = idx;
            resultado.puntaje_total += arreglo[idx].puntaje;
            resultado.costo_total   += arreglo[idx].costo;
            presupuesto_restante    -= arreglo[idx].costo;
        }
        // Si no entra por presupuesto se descarta, el greedy no vuelve atras
    }

    free(orden);
    return resultado;
}

// =====================================================================
// MENOR COSTO PRIMERO (Cristóbal)
// =====================================================================
//
// Ordena por costo ascendente y toma deportistas mientras no se supere
// el presupuesto W. Maximiza la cantidad de deportistas incluidos, pero
// no necesariamente el puntaje total -> en el informe se muestra un
// contraejemplo donde este criterio no es optimo.
//
// Complejidad:
//   Tiempo:  O(n log n)  -> dominado por qsort
//   Espacio: O(n)        -> arreglo auxiliar de indices

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
    // Empate en costo -> desempata por mayor puntaje
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
    }

    free(items);
    return resultado;
}

// =====================================================================
// SIN RESTRICCIÓN: K MEJORES (Guillermo)
// =====================================================================

ResultadoGreedy greedy_sin_restriccion(Deportista *arreglo, int n, int k) {
    ResultadoGreedy resultado;
    resultado.puntaje_total = 0.0f;
    resultado.costo_total   = 0;
    resultado.cantidad      = 0;
    resultado.indices       = malloc(k * sizeof(int)); // solo se necesitan k espacios

    int *orden = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        orden[i] = i;
    }

    // Ordenar por puntaje descendente reutilizando el comparador de mayor puntaje
    arreglo_global = arreglo;
    qsort(orden, n, sizeof(int), comparar_por_puntaje);

    // Sin restriccion de presupuesto, tomar directamente los k mejores
    for (int i = 0; i < k; i++) {
        int idx = orden[i];
        resultado.indices[resultado.cantidad++] = idx;
        resultado.puntaje_total += arreglo[idx].puntaje;
        resultado.costo_total   += arreglo[idx].costo; // se registra solo a modo informativo
    }

    free(orden);

    return resultado;
}