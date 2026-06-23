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

void imprimir_resultado_pd(ResultadoPD *resultado, Deportista *arreglo, const char *metodo) {
    if (!resultado->factible) {
        printf("\n[!] No existe solucion factible para los parametros ingresados (%s).\n", metodo);
        return;
    }

    printf("\n--- EQUIPO OPTIMO (%s) ---\n", metodo);
    printf("Deportistas seleccionados: %d\n", resultado->cantidad);
    printf("Puntaje total: %.1f\n", resultado->puntaje_total);
    printf("Costo total:   %d\n\n", resultado->costo_total);

    for (int i = 0; i < resultado->cantidad; i++) {
        imprimir_deportista(arreglo[resultado->indices[i]]);
    }
}

// =====================================================================
// PROGRAMACIÓN DINÁMICA: TABULACIÓN (BOTTOM-UP) (Guillermo)
// =====================================================================

ResultadoPD pd_tabulacion(Deportista *arreglo, int n, int w, int k) {
    ResultadoPD resultado;
    resultado.factible      = 0;
    resultado.puntaje_total = 0.0f;
    resultado.costo_total   = 0;
    resultado.cantidad      = 0;
    resultado.indices       = NULL;

    // Validaciones iniciales elementales
    if (n <= 0 || w <= 0 || k <= 0 || k > n) return resultado;

    // 1. Asignar memoria dinámica para la tabla 3D: dp[n + 1][w + 1][k + 1]
    int ***dp = malloc((n + 1) * sizeof(int **));
    for (int i = 0; i <= n; i++) {
        dp[i] = malloc((w + 1) * sizeof(int *));
        for (int j = 0; j <= w; j++) {
            dp[i][j] = malloc((k + 1) * sizeof(int));
            for (int l = 0; l <= k; l++) {
                dp[i][j][l] = -1; // -1 indica un estado inválido o no alcanzable
            }
        }
    }

    // Caso base: Requerir exactamente 0 deportistas siempre aporta 0 puntaje
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= w; j++) {
            dp[i][j][0] = 0;
        }
    }

    // 2. Llenado iterativo de la tabla (Bottom-Up)
    for (int i = 1; i <= n; i++) {
        int costo_item = arreglo[i - 1].costo;
        int puntaje_item_x10 = (int)(arreglo[i - 1].puntaje * 10);

        for (int j = 0; j <= w; j++) {
            for (int l = 1; l <= k; l++) {
                // Opción A: No incluir al deportista actual (i - 1)
                int sin_incluir = dp[i - 1][j][l];

                // Opción B: Incluir al deportista actual (i - 1) si hay presupuesto y el estado previo era factible
                int con_incluir = -1;
                if (j >= costo_item && dp[i - 1][j - costo_item][l - 1] != -1) {
                    con_incluir = dp[i - 1][j - costo_item][l - 1] + puntaje_item_x10;
                }

                // Guardar la opción óptima para el estado actual
                if (sin_incluir != -1 && con_incluir != -1) {
                    dp[i][j][l] = (sin_incluir > con_incluir) ? sin_incluir : con_incluir;
                } else if (sin_incluir != -1) {
                    dp[i][j][l] = sin_incluir;
                } else if (con_incluir != -1) {
                    dp[i][j][l] = con_incluir;
                } else {
                    dp[i][j][l] = -1;
                }
            }
        }
    }

    // 3. Verificar si el estado objetivo final es factible
    int puntaje_optimo_x10 = dp[n][w][k];
    if (puntaje_optimo_x10 == -1) {
        // Liberar memoria antes de retornar una solución no factible
        for (int i = 0; i <= n; i++) {
            for (int j = 0; j <= w; j++) free(dp[i][j]);
            free(dp[i]);
        }
        free(dp);
        return resultado;
    }

    // Asignar espacio para la estructura de retorno
    resultado.factible = 1;
    resultado.indices = malloc(k * sizeof(int));
    resultado.cantidad = k;

    // 4. Reconstrucción de la solución óptima caminando hacia atrás
    int w_actual = w;
    int k_actual = k;
    for (int i = n; i > 0 && k_actual > 0; i--) {
        int costo_item = arreglo[i - 1].costo;
        int puntaje_item_x10 = (int)(arreglo[i - 1].puntaje * 10);

        // Si se cumple la condición de transición con inclusión, este deportista fue seleccionado
        if (w_actual >= costo_item && dp[i - 1][w_actual - costo_item][k_actual - 1] != -1) {
            if (dp[i][w_actual][k_actual] == dp[i - 1][w_actual - costo_item][k_actual - 1] + puntaje_item_x10) {
                resultado.indices[k_actual - 1] = i - 1; // Se almacena manteniendo el orden relativo
                w_actual -= costo_item;
                k_actual--;
            }
        }
    }

    // 5. Calcular los totales reales basándonos en los elementos seleccionados
    for (int i = 0; i < resultado.cantidad; i++) {
        int idx = resultado.indices[i];
        resultado.puntaje_total += arreglo[idx].puntaje;
        resultado.costo_total   += arreglo[idx].costo;
    }

    // 6. Liberar la memoria de la tabla 3D
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= w; j++) free(dp[i][j]);
        free(dp[i]);
    }
    free(dp);

    return resultado;
}