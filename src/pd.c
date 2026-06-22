#include "pd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =====================================================================
// ESTRUCTURAS INTERNAS
// =====================================================================

// Tabla de memoización: memo[i][w][k]
// i = índice del deportista actual
// w = presupuesto restante
// k = cantidad de deportistas restantes a seleccionar
// Valor -1 indica que no ha sido calculado aún
static int ***memo = NULL;
static int n_global, w_global, k_global;

// =====================================================================
// FUNCIONES AUXILIARES
// =====================================================================

static void inicializar_memo(int n, int w, int k) {
    memo = malloc((n + 1) * sizeof(int **));
    for (int i = 0; i <= n; i++) {
        memo[i] = malloc((w + 1) * sizeof(int *));
        for (int j = 0; j <= w; j++) {
            memo[i][j] = malloc((k + 1) * sizeof(int));
            for (int l = 0; l <= k; l++) {
                memo[i][j][l] = -1;
            }
        }
    }
}

static void liberar_memo(int n, int w) {
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= w; j++) {
            free(memo[i][j]);
        }
        free(memo[i]);
    }
    free(memo);
    memo = NULL;
}

// =====================================================================
// FUNCIÓN RECURSIVA CON MEMOIZACIÓN
// =====================================================================

// Retorna el puntaje máximo (×10) seleccionando exactamente k deportistas
// entre arreglo[i..n-1] con presupuesto restante w
// Retorna -1 si no existe solución factible
static int memo_recursivo(Deportista *arreglo, int i, int w, int k) {
    // Caso base: necesitamos exactamente 0 deportistas más
    if (k == 0) return 0;

    // Caso base: no quedan deportistas pero aún necesitamos k
    if (i == n_global) return -1;

    // Caso base: presupuesto insuficiente
    if (w < 0) return -1;

    // Verificar si ya fue calculado
    if (memo[i][w][k] != -1) return memo[i][w][k];

    // Opción 1: NO incluir al deportista i
    int sin_incluir = memo_recursivo(arreglo, i + 1, w, k);

    // Opción 2: incluir al deportista i (si hay presupuesto)
    int con_incluir = -1;
    if (arreglo[i].costo <= w) {
        int sub = memo_recursivo(arreglo, i + 1,
            w - arreglo[i].costo,
            k - 1);
        if (sub != -1) {
            con_incluir = (int)(arreglo[i].puntaje * 10) + sub;
        }
    }

    // Elegir el mejor entre incluir y no incluir
    int resultado;
    if (con_incluir == -1 && sin_incluir == -1) {
        resultado = -1; // No hay solución factible
    } else if (con_incluir == -1) {
        resultado = sin_incluir;
    } else if (sin_incluir == -1) {
        resultado = con_incluir;
    } else {
        resultado = con_incluir > sin_incluir ? con_incluir : sin_incluir;
    }

    memo[i][w][k] = resultado;
    return resultado;
}

// =====================================================================
// RECONSTRUCCIÓN DE LA SOLUCIÓN
// =====================================================================

// Recorre la tabla de memo para identificar qué deportistas fueron seleccionados
static void reconstruir_solucion(Deportista *arreglo, int *indices,
                                  int *cantidad, int w, int k) {
    *cantidad = 0;
    int w_actual = w;
    int k_actual = k;

    for (int i = 0; i < n_global && k_actual > 0; i++) {
        // Verificar si el deportista i fue incluido
        int con_incluir = -1;

        if (arreglo[i].costo <= w_actual) {
            int sub = memo_recursivo(arreglo, i + 1,
                w_actual - arreglo[i].costo,
                k_actual - 1);
            if (sub != -1) {
                con_incluir = (int)(arreglo[i].puntaje * 10) + sub;
            }
        }

        // Si incluir da el mismo resultado óptimo, lo incluimos
        int optimo = memo[i][w_actual][k_actual];
        if (con_incluir != -1 && con_incluir == optimo) {
            indices[*cantidad] = i;
            (*cantidad)++;
            w_actual -= arreglo[i].costo;
            k_actual--;
        }
    }
}

// =====================================================================
// FUNCIÓN PRINCIPAL MEMOIZACIÓN
// =====================================================================

ResultadoPD pd_memoizacion(Deportista *arreglo, int n, int w, int k) {
    ResultadoPD resultado;
    resultado.factible    = 0;
    resultado.puntaje_total = 0.0f;
    resultado.costo_total   = 0;
    resultado.cantidad      = 0;
    resultado.indices       = NULL;

    // Validaciones
    if (n <= 0 || w <= 0 || k <= 0 || k > n) return resultado;

    // Inicializar variables globales y tabla
    n_global = n;
    w_global = w;
    k_global = k;
    inicializar_memo(n, w, k);

    // Resolver
    int puntaje_x10 = memo_recursivo(arreglo, 0, w, k);

    if (puntaje_x10 == -1) {
        // No existe solución factible
        liberar_memo(n, w);
        return resultado;
    }

    // Existe solución, reconstruir
    resultado.factible  = 1;
    resultado.indices   = malloc(k * sizeof(int));
    reconstruir_solucion(arreglo, resultado.indices,
        &resultado.cantidad, w, k);

    // Calcular puntaje y costo totales reales
    for (int i = 0; i < resultado.cantidad; i++) {
        int idx = resultado.indices[i];
        resultado.puntaje_total += arreglo[idx].puntaje;
        resultado.costo_total   += arreglo[idx].costo;
    }

    liberar_memo(n, w);
    return resultado;
}

// =====================================================================
// FUNCIONES DE UTILIDAD
// =====================================================================

void liberar_resultado_pd(ResultadoPD *resultado) {
    if (resultado->indices != NULL) {
        free(resultado->indices);
        resultado->indices = NULL;
    }
}

void imprimir_resultado_pd(ResultadoPD *resultado, Deportista *arreglo) {
    if (!resultado->factible) {
        printf("\n[!] No existe solucion factible para los parametros ingresados.\n");
        return;
    }

    printf("\n--- EQUIPO OPTIMO (Memoizacion) ---\n");
    printf("Deportistas seleccionados: %d\n", resultado->cantidad);
    printf("Puntaje total: %.1f\n", resultado->puntaje_total);
    printf("Costo total:   %d\n\n", resultado->costo_total);

    for (int i = 0; i < resultado->cantidad; i++) {
        imprimir_deportista(arreglo[resultado->indices[i]]);
    }
}