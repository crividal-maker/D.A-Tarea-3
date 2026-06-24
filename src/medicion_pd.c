#include "medicion_pd.h"
#include "pd.h"
#include "greedy.h"
#include "deportista.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// =====================================================================
// FUNCIÓN AUXILIAR: genera arreglo de n deportistas aleatorios
// =====================================================================

static void generar_arreglo_local(Deportista *arreglo, int n) {
    for (int i = 0; i < n; i++) {
        arreglo[i].id = i + 1;
        generar_nombre_aleatorio(arreglo[i].nombre, MAX_LONGITUD_NOMBRE);
        arreglo[i].puntaje = 50.0f + ((float)(rand() % 501) / 10.0f);
        arreglo[i].costo = (rand() % 100) + 1;
        arreglo[i].competencias = (rand() % 40) + 1;
        strncpy(arreglo[i].equipo, "Medicion", MAX_LONGITUD_EQUIPO);
    }
}

// =====================================================================
// FUNCIÓN PRINCIPAL DE MEDICIÓN
// =====================================================================

void medicion_pd_general() {
    printf("\n[!] Iniciando medicion experimental (W=%d, K=%d)...\n",
        W_FIJO, K_FIJO);

    // Archivo de tiempos
    FILE *f_tiempos = fopen("db/medicion_pd_tiempos.csv", "w");
    if (f_tiempos == NULL) {
        printf("Error al abrir archivo de tiempos.\n");
        return;
    }
    fprintf(f_tiempos, "n,pd_memo,pd_tabu,g_puntaje,g_costo,g_ratio,g_sinrestr\n");

    // Archivo de calidad (puntaje obtenido)
    FILE *f_calidad = fopen("db/medicion_pd_calidad.csv", "w");
    if (f_calidad == NULL) {
        printf("Error al abrir archivo de calidad.\n");
        fclose(f_tiempos);
        return;
    }
    fprintf(f_calidad,
        "n,"
        "pd_memo_puntaje,pd_memo_k,"
        "pd_tabu_puntaje,pd_tabu_k,"
        "g_puntaje_puntaje,g_puntaje_k,"
        "g_costo_puntaje,g_costo_k,"
        "g_ratio_puntaje,g_ratio_k,"
        "g_sinrestr_puntaje,g_sinrestr_k\n");

    for (int t = 0; t < TAMANIOS_PD; t++) {
        int n = TAMANIOS[t];
        printf("  Midiendo n=%d...\n", n);

        // Generar datos
        Deportista *arreglo = malloc(n * sizeof(Deportista));
        if (arreglo == NULL) {
            printf("Error: no se pudo reservar memoria para n=%d.\n", n);
            continue;
        }
        generar_arreglo_local(arreglo, n);

        // ── PD Memoización ──────────────────────────────────────────
        clock_t ini = clock();
        ResultadoPD res_memo = pd_memoizacion(arreglo, n, W_FIJO, K_FIJO);
        clock_t fin = clock();
        double t_memo = (double)(fin - ini) / CLOCKS_PER_SEC;

        // ── PD Tabulación ────────────────────────────────────────────
        ini = clock();
        ResultadoPD res_tabu = pd_tabulacion(arreglo, n, W_FIJO, K_FIJO);
        fin = clock();
        double t_tabu = (double)(fin - ini) / CLOCKS_PER_SEC;

        // ── Greedy mayor puntaje ─────────────────────────────────────
        ini = clock();
        ResultadoGreedy res_g_puntaje = greedy_mayor_puntaje(arreglo, n, W_FIJO);
        fin = clock();
        double t_g_puntaje = (double)(fin - ini) / CLOCKS_PER_SEC;

        // ── Greedy menor costo ───────────────────────────────────────
        ini = clock();
        ResultadoGreedy res_g_costo = greedy_menor_costo(arreglo, n, W_FIJO);
        fin = clock();
        double t_g_costo = (double)(fin - ini) / CLOCKS_PER_SEC;

        // ── Greedy ratio ─────────────────────────────────────────────
        ini = clock();
        ResultadoGreedy res_g_ratio = greedy_mayor_ratio(arreglo, n, W_FIJO);
        fin = clock();
        double t_g_ratio = (double)(fin - ini) / CLOCKS_PER_SEC;

        // ── Greedy sin restricción ───────────────────────────────────
        ini = clock();
        ResultadoGreedy res_g_sinrestr = greedy_sin_restriccion(arreglo, n, K_FIJO);
        fin = clock();
        double t_g_sinrestr = (double)(fin - ini) / CLOCKS_PER_SEC;

        // Guardar tiempos
        fprintf(f_tiempos, "%d,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n",
                n, t_memo, t_tabu, t_g_puntaje, t_g_costo, t_g_ratio, t_g_sinrestr);

        // Guardar calidad (puntaje obtenido y cantidad seleccionada)
        fprintf(f_calidad, "%d,%.1f,%d,%.1f,%d,%.1f,%d,%.1f,%d,%.1f,%d,%.1f,%d\n",
                n,
                res_memo.factible     ? res_memo.puntaje_total     : 0.0f,
                res_memo.factible     ? res_memo.cantidad           : 0,
                res_tabu.factible     ? res_tabu.puntaje_total     : 0.0f,
                res_tabu.factible     ? res_tabu.cantidad           : 0,
                res_g_puntaje.puntaje_total, res_g_puntaje.cantidad,
                res_g_costo.puntaje_total,   res_g_costo.cantidad,
                res_g_ratio.puntaje_total,   res_g_ratio.cantidad,
                res_g_sinrestr.puntaje_total, res_g_sinrestr.cantidad);

        // Liberar memoria
        liberar_resultado_pd(&res_memo);
        liberar_resultado_pd(&res_tabu);
        liberar_resultado_greedy(&res_g_puntaje);
        liberar_resultado_greedy(&res_g_costo);
        liberar_resultado_greedy(&res_g_ratio);
        liberar_resultado_greedy(&res_g_sinrestr);
        free(arreglo);
    }

    fclose(f_tiempos);
    fclose(f_calidad);

    printf("[OK] Medicion completada.\n");
    printf("     Tiempos: db/medicion_pd_tiempos.csv\n");
    printf("     Calidad: db/medicion_pd_calidad.csv\n");
}