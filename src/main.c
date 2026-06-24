#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "deportista.h"
#include "menu.h"
#include "ordenamiento.h"
#include "busqueda.h"
#include "merge.h"
#include "quicksort.h"
#include "medicion.h"
#include "pd.h"
#include "greedy.h"
#include "medicion_pd.h"

#define MAX_DEPORTISTAS 50
#define CANTIDAD_GENERAR 50
#define ARCHIVO_DATOS "db/datos.csv"

// variables globales para tiempos
int p_opcion_temp_tiempos = 0;
int umbral_temp_tiempos = 0;

int main() {
    // Inicializar la semilla para los números aleatorios
    srand(time(NULL));

    Deportista arreglo[MAX_DEPORTISTAS];
    int cantidad_actual = 0;
    int opcion=-1;

    // Intentar cargar datos al iniciar el programa
    cantidad_actual = leer_deportistas_csv(arreglo, MAX_DEPORTISTAS, ARCHIVO_DATOS);
    if (cantidad_actual > 0) {
        printf("Se cargaron %d registros desde '%s' automaticamente.\n", cantidad_actual, ARCHIVO_DATOS);
    }

    do {
        mostrar_menu();
        if (scanf("%d", &opcion) != 1) {
            while(getchar() != '\n'); // Limpiar el buffer si el usuario ingresa letras
            continue;
        }

        switch(opcion) {
            case 1: 
                int tipo_generacion;
                menu_generacion_datos(); 
                if (scanf("%d", &tipo_generacion) != 1) {
                    while(getchar() != '\n');
                    printf("Entrada invalida.\n");
                    break;
                }

                printf("Generando %d deportistas...\n", CANTIDAD_GENERAR);
                
                switch(tipo_generacion) {
                    case 1: // Aleatorios (mezclados)
                        reiniciar_generador_id();
                        for (int i = 0; i < CANTIDAD_GENERAR; i++) {
                            arreglo[i] = generar_deportista();
                        }
                        cantidad_actual = CANTIDAD_GENERAR;
                        mezclar_deportistas(arreglo, cantidad_actual);
                        printf("Datos aleatorios generados y mezclados.\n");
                        break;
                    case 2: // Ordenados por ID (Mejor Caso)
                        generar_datos_ordenados(arreglo, CANTIDAD_GENERAR);
                        cantidad_actual = CANTIDAD_GENERAR;
                        printf("Datos ordenados por ID generados (Mejor Caso).\n");
                        break;
                    case 3: // Inversamente ordenados por ID (Peor Caso)
                        generar_datos_inversos(arreglo, CANTIDAD_GENERAR);
                        cantidad_actual = CANTIDAD_GENERAR;
                        printf("Datos inversamente ordenados por ID generados (Peor Caso).\n");
                        break;
                    default:
                        printf("Opcion de generacion invalida.\n");
                        break;
                }

                if (cantidad_actual > 0) { 
                    guardar_deportistas_csv(arreglo, cantidad_actual, ARCHIVO_DATOS);
                    printf("Datos guardados en '%s'.\n", ARCHIVO_DATOS);
                }
                break;
            
            case 2: 
                cantidad_actual = leer_deportistas_csv(arreglo, MAX_DEPORTISTAS, ARCHIVO_DATOS);
                if (cantidad_actual > 0) {
                    printf("Se cargaron %d registros exitosamente.\n", cantidad_actual);
                } else {
                    printf("No se pudo cargar o el archivo '%s' esta vacio.\n", ARCHIVO_DATOS);
                }
                break;
            
            case 3: 
                if (cantidad_actual == 0) {
                    printf("No hay datos cargados en memoria.\n");
                    break;
                }
                printf("\n--- REGISTROS ACTUALES ---\n");
                for (int i = 0; i < cantidad_actual; i++) {
                    imprimir_deportista(arreglo[i]);
                }
                break;
            
            case 4:
                if (cantidad_actual == 0) {
                    printf("No hay datos para ordenar.\n");
                    break;
                }
                int campo;
                menu_ordenamiento();
                scanf("%d", &campo);
                if (campo < 1 || campo > 5) {
                    printf("Campo invalido.\n");
                    break;
                }

                int algoritmo;
                printf("\n--- ALGORITMO ---\n");
                printf("1. Bubble Sort\n");
                printf("2. Insertion Sort\n");
                printf("3. Selection Sort\n");
                printf("4. Cocktail Shaker Sort\n");
                printf("5. Merge Sort (Optimizado)\n");
                printf("6. Quick Sort (Multivariante)\n");
                printf("Seleccione algoritmo: ");
                scanf("%d", &algoritmo);

                int umbral = 1; // Para Merge Sort
                TipoPivote tipo_pivote = PIVOTE_ULTIMO; // Para Quick Sort

                if (algoritmo == 5) {
                    printf("Ingrese el umbral para optimizacion (1 para clasico, >1 para hibrido): "); // REVISAR
                    scanf("%d", &umbral);
                    umbral_temp_tiempos = umbral;
                } else if (algoritmo == 6) {
                    printf("\n--- SELECCION DE PIVOTE ---\n");
                    printf("1. Ultimo elemento\n");
                    printf("2. Primer elemento\n");
                    printf("3. Elemento aleatorio\n");
                    printf("4. Mediana de tres\n");
                    printf("Seleccione tipo de pivote: ");
                    int p_opcion;
                    scanf("%d", &p_opcion);
                    p_opcion_temp_tiempos = p_opcion;
                    tipo_pivote = (TipoPivote)p_opcion;
                }

                clock_t inicio_tiempo = clock();

                switch(algoritmo) {
                    case 1: bubble_sort(arreglo, cantidad_actual, campo); break;
                    case 2: insertion_sort(arreglo, cantidad_actual, campo); break;
                    case 3: selection_sort(arreglo, cantidad_actual, campo); break;
                    case 4: cocktail_shaker_sort(arreglo, cantidad_actual, campo); break;
                    case 5: merge_sort(arreglo, cantidad_actual, campo, umbral); break;
                    case 6: quick_sort(arreglo, cantidad_actual, campo, tipo_pivote); break;
                    default: printf("Algoritmo invalido.\n"); break;
                }

                if (algoritmo >= 1 && algoritmo <= 6) {
                    clock_t fin_tiempo = clock();
                    double tiempo_gastado = (double)(fin_tiempo - inicio_tiempo) / CLOCKS_PER_SEC;
                    printf("\n[OK] Registros ordenados en %f segundos.\n", tiempo_gastado);
                    
                    // Switch para guardar los datos ; DEPENDIENTE DE  : algoritmo , tiempo gastado , umbral_temp_tiempos, p_opcion_temp_tiempos
                    switch(algoritmo){
                        case 1: guardar_tiempo("db/bubble_sort.csv",cantidad_actual,tiempo_gastado); break;
                        case 2: guardar_tiempo("db/insertion_sort.csv",cantidad_actual,tiempo_gastado); break;
                        case 3: guardar_tiempo("db/selection_sort.csv",cantidad_actual,tiempo_gastado); break;
                        case 4: guardar_tiempo("db/cocktail_sort.csv",cantidad_actual,tiempo_gastado); break;
                        case 5: 
                            if(umbral_temp_tiempos == 1){
                                guardar_tiempo("db/merge/merge_clasico.csv",cantidad_actual,tiempo_gastado);
                            }else if(umbral_temp_tiempos !=1){
                                guardar_tiempo("db/merge/merge_insertion.csv",cantidad_actual,tiempo_gastado);
                            }
                        
                        ;break;
                        case 6: 
                            switch(p_opcion_temp_tiempos){
                            case 1:
                                guardar_tiempo("db/quicksort/ultimo_elemento.csv",cantidad_actual,tiempo_gastado);break;
                            case 2:
                                guardar_tiempo("db/quicksort/primer_elemento.csv",cantidad_actual,tiempo_gastado);break;

                            case 3:
                                guardar_tiempo("db/quicksort/aleatorio_pivote.csv",cantidad_actual,tiempo_gastado);break;

                            case 4:
                                guardar_tiempo("db/quicksort/mediana_tres.csv",cantidad_actual,tiempo_gastado);break;

                            default:
                                printf("No se detecta opcion valida para el guardado de los datos de los pivotes en Quick Sort");break;
                            };break;

                        default:printf("No se guardaron tiempos de ejecucion");break;
                    }

                }
                break;
            
            case 5:
                if (cantidad_actual == 0) {
                    printf("No hay datos cargados.\n");
                    break;
                }

                int id_buscar, metodo;
                printf("Ingrese el ID a buscar: ");
                scanf("%d", &id_buscar);

                printf("\n--- METODO DE BUSQUEDA ---\n");
                printf("1. Busqueda Secuencial\n");
                printf("2. Busqueda Binaria Iterativa\n");
                printf("3. Busqueda Binaria Recursiva\n");
                printf("4. Busqueda Binaria por Rangos\n");
                printf("5. Busqueda Exponencial\n");
                printf("6. Busqueda por Interpolacion\n");
                printf("Seleccione metodo: ");
                scanf("%d", &metodo);

                int indice_encontrado = -1;
                clock_t inicio_tiempo_a = clock();

                if (metodo == 1) {
                    clock_t inicio_tiempo_1 = clock();
                    for(int i = 0; i<cantidad_actual/50; i++){
                        indice_encontrado = busqueda_secuencial(arreglo, cantidad_actual, id_buscar);
                    }
                    clock_t fin_tiempo_1 = clock();

                    double tiempo_gastado_1 = (double)(fin_tiempo_1 - inicio_tiempo_1) / CLOCKS_PER_SEC;
                    guardar_tiempo("db/busquedas/busqueda_secuencial.csv",cantidad_actual,tiempo_gastado_1);
                    
                    
                } else if (metodo == 2) {

                    printf("[!] Ordenando por ID con Quick Sort para busqueda binaria...\n");
                    quick_sort(arreglo, cantidad_actual, 1, PIVOTE_MEDIANA);

                    clock_t inicio_tiempo_2 = clock();
                    for(int i = 0; i<cantidad_actual/50; i++){
                        
                        indice_encontrado = busqueda_binaria_iterativa(arreglo, cantidad_actual, id_buscar);
                    }
                    clock_t fin_tiempo_2 = clock();

                    double tiempo_gastado_2 = (double)(fin_tiempo_2 - inicio_tiempo_2) / CLOCKS_PER_SEC;
                    guardar_tiempo("db/busquedas/busqueda_binaria_iterativa.csv",cantidad_actual,tiempo_gastado_2);

                } else if (metodo == 3) {
                    printf("[!] Ordenando por ID con Quick Sort para busqueda binaria...\n");
                    quick_sort(arreglo, cantidad_actual, 1, PIVOTE_MEDIANA);
                    clock_t inicio_tiempo_3 = clock();
                    for(int i = 0; i<cantidad_actual/50; i++){
                        
                        indice_encontrado = busqueda_binaria_recursiva(arreglo, 0, cantidad_actual - 1, id_buscar);
                    }
                    clock_t fin_tiempo_3 = clock();

                    double tiempo_gastado_3 = (double)(fin_tiempo_3 - inicio_tiempo_3) / CLOCKS_PER_SEC;
                    guardar_tiempo("db/busquedas/busqueda_binaria_recursiva.csv",cantidad_actual,tiempo_gastado_3);
                    
                } else if (metodo == 4) {
                    int id_min, id_max;
                    printf("Ingrese el ID minimo del rango: ");
                    scanf("%d", &id_min);
                    printf("Ingrese el ID maximo del rango: ");
                    scanf("%d", &id_max);

                    printf("[!] Ordenando por ID con Quick Sort para busqueda binaria...\n");
                    quick_sort(arreglo, cantidad_actual, 1, PIVOTE_MEDIANA);

                    int primera_pos, ultima_pos;
                    
                    clock_t inicio_tiempo_4 = clock();
                    for(int i = 0; i<cantidad_actual/50; i++){
                        busqueda_binaria_rangos(arreglo, cantidad_actual, id_min, id_max, &primera_pos, &ultima_pos);
                    }
                    clock_t fin_tiempo_4 = clock();

                    double tiempo_gastado_4 = (double)(fin_tiempo_4 - inicio_tiempo_4) / CLOCKS_PER_SEC;
                    guardar_tiempo("db/busquedas/busqueda_binaria_rangos.csv",cantidad_actual,tiempo_gastado_4);
    

                } else if (metodo == 5) {

                    printf("[!] Ordenando por ID con Quick Sort para busqueda exponencial...\n");
                    quick_sort(arreglo, cantidad_actual, 1, PIVOTE_MEDIANA);

                    clock_t inicio_tiempo_5 = clock();
                    for(int i = 0; i<cantidad_actual/50; i++){
                        indice_encontrado = busqueda_exponencial(arreglo, cantidad_actual, id_buscar);
                    }
                    clock_t fin_tiempo_5 = clock();
                        
                    double tiempo_gastado_5 = (double)(fin_tiempo_5 - inicio_tiempo_5) / CLOCKS_PER_SEC;
                    guardar_tiempo("db/busquedas/busqueda_exponencial.csv",cantidad_actual,tiempo_gastado_5);
                

                } else if (metodo == 6) {   
                    printf("[!] Ordenando por ID con Quick Sort para busqueda por interpolacion...\n");
                    quick_sort(arreglo, cantidad_actual, 1, PIVOTE_MEDIANA);

                    clock_t inicio_tiempo_6 = clock();
                    for(int i = 0; i<cantidad_actual/50; i++){
                        indice_encontrado = busqueda_interpolacion(arreglo, cantidad_actual, id_buscar);
                    }
                        clock_t fin_tiempo_6 = clock();
                        
                        double tiempo_gastado_6 = (double)(fin_tiempo_6 - inicio_tiempo_6) / CLOCKS_PER_SEC;
                        guardar_tiempo("db/busquedas/busqueda_interpolacion.csv",cantidad_actual,tiempo_gastado_6);
                    
                }
                clock_t fin_tiempo_a = clock();
                double tiempo_gastado_a = (double)(fin_tiempo_a - inicio_tiempo_a) / CLOCKS_PER_SEC;

                if (indice_encontrado != -1) {
                    printf("\nEncontrado en %f segundos!\n", tiempo_gastado_a);
                    imprimir_deportista(arreglo[indice_encontrado]);
                } else {
                    printf("\nNo encontrado (%f segundos).\n", tiempo_gastado_a);
                }
            
            break;
            case 6:
                if (cantidad_actual == 0) {
                    printf("No hay datos.\n");
                    break;
                }
                int top_n;
                printf("Cantidad para el ranking: ");
                scanf("%d", &top_n);
                if (top_n > cantidad_actual) top_n = cantidad_actual;
                ordenar_ranking_descendente(arreglo, cantidad_actual);
                for (int i = 0; i < top_n; i++) imprimir_deportista(arreglo[i]);
                break;
            case 7:
                printf("Realizando medicion de tiempos de ordenamientos\n");
                medicion_general();
                break;
            case 8: // O el número que le asignes en tu menú
                if (cantidad_actual == 0) {
                    printf("No hay datos cargados.\n");
                    break;
                }
                int k_buscar;
                printf("Ingrese la posicion (k) del mejor deportista que desea buscar (ej: 1 para el mejor, 3 para el tercero): ");
                scanf("%d", &k_buscar);

                if (k_buscar > 0 && k_buscar <= cantidad_actual) {
                    Deportista kesimo = quick_select(arreglo, 0, cantidad_actual - 1, k_buscar);
                    printf("\n--- El %d-esimo MEJOR DEPORTISTA (por puntaje) ---\n", k_buscar);
                    imprimir_deportista(kesimo);
                } else {
                    printf("Posicion k invalida. Debe ser entre 1 y %d.\n", cantidad_actual);
                }
                break;

            case 9: {
                if (cantidad_actual == 0) {
                    printf("No hay datos cargados.\n");
                    break;
                }

                int presupuesto, k_equipo;
                printf("Ingrese el presupuesto maximo (W): ");
                scanf("%d", &presupuesto);
                printf("Ingrese la cantidad exacta de deportistas (K): ");
                scanf("%d", &k_equipo);

                if (presupuesto <= 0 || k_equipo <= 0 || k_equipo > cantidad_actual) {
                    printf("Parametros invalidos.\n");
                    break;
                }

                printf("\n[!] Ejecutando PD Memoizacion (n=%d, W=%d, K=%d)...\n",
                    cantidad_actual, presupuesto, k_equipo);

                clock_t ini = clock();
                ResultadoPD resultado = pd_memoizacion(arreglo, cantidad_actual,
                                                        presupuesto, k_equipo);
                clock_t fin = clock();
                double tiempo = (double)(fin - ini) / CLOCKS_PER_SEC;

                imprimir_resultado_pd(&resultado, arreglo, "Memoizacion (Top-Down)");
                printf("Tiempo de ejecucion: %f segundos\n", tiempo);

                liberar_resultado_pd(&resultado);
                break;
            }

            case 10: {
                if (cantidad_actual == 0) {
                    printf("No hay datos cargados.\n");
                    break;
                }

                int presupuesto, k_equipo;
                printf("Ingrese el presupuesto maximo (W): ");
                scanf("%d", &presupuesto);
                printf("Ingrese la cantidad exacta de deportistas (K): ");
                scanf("%d", &k_equipo);

                if (presupuesto <= 0 || k_equipo <= 0 || k_equipo > cantidad_actual) {
                    printf("Parametros invalidos.\n");
                    break;
                }

                printf("\n[!] Ejecutando PD Tabulacion (n=%d, W=%d, K=%d)...\n",
                    cantidad_actual, presupuesto, k_equipo);

                clock_t ini = clock();
                
                ResultadoPD resultado = pd_tabulacion(arreglo, cantidad_actual, presupuesto, k_equipo);
                clock_t fin = clock();
                double tiempo = (double)(fin - ini) / CLOCKS_PER_SEC;

                imprimir_resultado_pd(&resultado, arreglo, "Tabulacion (Bottom-Up)");
                printf("Tiempo de ejecucion: %f segundos\n", tiempo);

                liberar_resultado_pd(&resultado);
                break;
            }

            case 11: {
                if (cantidad_actual == 0) {
                    printf("No hay datos cargados.\n");
                    break;
                }
                int presupuesto;
                printf("Ingrese el presupuesto maximo (W): ");
                scanf("%d", &presupuesto);

                if (presupuesto <= 0) {
                    printf("Presupuesto invalido.\n");
                    break;
                }

                clock_t ini = clock();
                ResultadoGreedy resultado = greedy_menor_costo(arreglo, cantidad_actual, presupuesto);
                clock_t fin = clock();
                double tiempo = (double)(fin - ini) / CLOCKS_PER_SEC;

                imprimir_resultado_greedy(&resultado, arreglo, "Menor costo primero");
                printf("Tiempo de ejecucion: %f segundos\n", tiempo);

                liberar_resultado_greedy(&resultado);
                break;
            }

            case 12: {
                if (cantidad_actual == 0) {
                    printf("No hay datos cargados.\n");
                    break;
                }
                int presupuesto;
                printf("Ingrese el presupuesto maximo (W): ");
                scanf("%d", &presupuesto);

                if (presupuesto <= 0) {
                    printf("Presupuesto invalido.\n");
                    break;
                }

                clock_t ini = clock();
                ResultadoGreedy resultado = greedy_mayor_puntaje(arreglo, cantidad_actual, presupuesto);
                clock_t fin = clock();
                double tiempo = (double)(fin - ini) / CLOCKS_PER_SEC;

                // Nota: Los indices de resultado apuntan a la 'copia' ordenada internamente
                imprimir_resultado_greedy(&resultado, arreglo, "Mayor puntaje primero");
                printf("Tiempo de ejecucion: %f segundos\n", tiempo);

                liberar_resultado_greedy(&resultado);
                break;
            }

            case 13: {
                if (cantidad_actual == 0) {
                    printf("No hay datos cargados.\n");
                    break;
                }

                int k_mejores;
                printf("Ingrese la cantidad exacta de deportistas a seleccionar (K): ");
                scanf("%d", &k_mejores);

                // K no puede ser negativo ni mayor al total de deportistas cargados
                if (k_mejores <= 0 || k_mejores > cantidad_actual) {
                    printf("Cantidad K invalida. Debe ser entre 1 y %d.\n", cantidad_actual);
                    break;
                }

                clock_t ini = clock();
                ResultadoGreedy resultado = greedy_sin_restriccion(arreglo, cantidad_actual, k_mejores);
                clock_t fin = clock();
                double tiempo = (double)(fin - ini) / CLOCKS_PER_SEC;

                imprimir_resultado_greedy(&resultado, arreglo, "Sin restriccion - K mejores");
                printf("Tiempo de ejecucion: %f segundos\n", tiempo);

                liberar_resultado_greedy(&resultado);
                break;
            }

            case 14: {
                if (cantidad_actual == 0) {
                    printf("No hay datos cargados.\n");
                    break;
                }
                int presupuesto;
                printf("Ingrese el presupuesto maximo (W): ");
                scanf("%d", &presupuesto);

                if (presupuesto <= 0) {
                    printf("Presupuesto invalido.\n");
                    break;
                }

                clock_t ini = clock();
                ResultadoGreedy resultado = greedy_mayor_ratio(arreglo, cantidad_actual, presupuesto);
                clock_t fin = clock();
                double tiempo = (double)(fin - ini) / CLOCKS_PER_SEC;

                imprimir_resultado_greedy(&resultado, arreglo, "Mayor ratio puntaje/costo");
                printf("Tiempo de ejecucion: %f segundos\n", tiempo);

                liberar_resultado_greedy(&resultado);
                break;
            }

            case 15: {
                medicion_pd_general();
                break;
            }
            
            case 0:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
    } while (opcion != 0);

    return 0;
}