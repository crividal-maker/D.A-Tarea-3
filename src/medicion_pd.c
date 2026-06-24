#include "medicion_pd.h"
#include "pd.h"
#include "greedy.h"
#include "deportista.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Cantidad de repeticiones para promediar tiempos.
// Los greedy terminan en microsegundos, por debajo de la resolucion real
// de clock() (~1ms), asi que una sola corrida da puro ruido de medicion.
// Repitiendo varias veces y promediando el tiempo total se elimina ese ruido.
#define REPS_GREEDY 50
#define REPS_PD 5  // la PD ya tarda decimas de segundo, no necesita tantas repeticiones

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

    // Semilla fija solo para esta medicion: asi los datos generados son
    // siempre los mismos entre corridas y el informe (tablas, graficos y
    // el contraejemplo de optimalidad) queda reproducible. Al terminar
    // se vuelve a sembrar con time(NULL) para no afectar el resto del menu.
    srand(42);

    FILE *f_tiempos = fopen("db/medicion_pd_tiempos.csv", "w");
    if (f_tiempos == NULL) {
        printf("Error al abrir archivo de tiempos.\n");
        return;
    }
    fprintf(f_tiempos, "n,pd_memo,pd_tabu,g_puntaje,g_costo,g_ratio,g_sinrestr\n");

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

        Deportista *arreglo = malloc(n * sizeof(Deportista));
        if (arreglo == NULL) {
            printf("Error: no se pudo reservar memoria para n=%d.\n", n);
            continue;
        }
        generar_arreglo_local(arreglo, n);

        // ── PD Memoización (promedio de REPS_PD corridas) ────────────
        ResultadoPD res_memo;
        clock_t ini = clock();
        for (int r = 0; r < REPS_PD; r++) {
            if (r > 0) liberar_resultado_pd(&res_memo);
            res_memo = pd_memoizacion(arreglo, n, W_FIJO, K_FIJO);
        }
        clock_t fin = clock();
        double t_memo = ((double)(fin - ini) / CLOCKS_PER_SEC) / REPS_PD;

        // ── PD Tabulación (promedio de REPS_PD corridas) ─────────────
        ResultadoPD res_tabu;
        ini = clock();
        for (int r = 0; r < REPS_PD; r++) {
            if (r > 0) liberar_resultado_pd(&res_tabu);
            res_tabu = pd_tabulacion(arreglo, n, W_FIJO, K_FIJO);
        }
        fin = clock();
        double t_tabu = ((double)(fin - ini) / CLOCKS_PER_SEC) / REPS_PD;

        // ── Greedy mayor puntaje (promedio de REPS_GREEDY corridas) ──
        ResultadoGreedy res_g_puntaje;
        ini = clock();
        for (int r = 0; r < REPS_GREEDY; r++) {
            if (r > 0) liberar_resultado_greedy(&res_g_puntaje);
            res_g_puntaje = greedy_mayor_puntaje(arreglo, n, W_FIJO);
        }
        fin = clock();
        double t_g_puntaje = ((double)(fin - ini) / CLOCKS_PER_SEC) / REPS_GREEDY;

        // ── Greedy menor costo (promedio de REPS_GREEDY corridas) ────
        ResultadoGreedy res_g_costo;
        ini = clock();
        for (int r = 0; r < REPS_GREEDY; r++) {
            if (r > 0) liberar_resultado_greedy(&res_g_costo);
            res_g_costo = greedy_menor_costo(arreglo, n, W_FIJO);
        }
        fin = clock();
        double t_g_costo = ((double)(fin - ini) / CLOCKS_PER_SEC) / REPS_GREEDY;

        // ── Greedy ratio (promedio de REPS_GREEDY corridas) ──────────
        ResultadoGreedy res_g_ratio;
        ini = clock();
        for (int r = 0; r < REPS_GREEDY; r++) {
            if (r > 0) liberar_resultado_greedy(&res_g_ratio);
            res_g_ratio = greedy_mayor_ratio(arreglo, n, W_FIJO);
        }
        fin = clock();
        double t_g_ratio = ((double)(fin - ini) / CLOCKS_PER_SEC) / REPS_GREEDY;

        // ── Greedy sin restricción (promedio de REPS_GREEDY corridas) ─
        ResultadoGreedy res_g_sinrestr;
        ini = clock();
        for (int r = 0; r < REPS_GREEDY; r++) {
            if (r > 0) liberar_resultado_greedy(&res_g_sinrestr);
            res_g_sinrestr = greedy_sin_restriccion(arreglo, n, K_FIJO);
        }
        fin = clock();
        double t_g_sinrestr = ((double)(fin - ini) / CLOCKS_PER_SEC) / REPS_GREEDY;

        fprintf(f_tiempos, "%d,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n",
                n, t_memo, t_tabu, t_g_puntaje, t_g_costo, t_g_ratio, t_g_sinrestr);

        // La solucion no cambia entre repeticiones, por eso la calidad
        // se guarda con los resultados de la ultima corrida de cada algoritmo
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

    // Restaurar semilla aleatoria normal para el resto del programa
    srand((unsigned int)time(NULL));

    printf("[OK] Medicion completada (cada tiempo es el promedio de %d/%d corridas).\n",
        REPS_GREEDY, REPS_PD);
    printf("     Tiempos: db/medicion_pd_tiempos.csv\n");
    printf("     Calidad: db/medicion_pd_calidad.csv\n");
}