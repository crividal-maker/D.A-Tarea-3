#include "medicion.h"
#include "ordenamiento.h"
#include "deportista.h"
#include "time.h"
#include "merge.h"
#include "quicksort.h"
#include <string.h>

Deportista arreglo_a[1000];
Deportista arreglo_b[5000];
Deportista arreglo_c[10000];
Deportista arreglo_d[20000];

void guardar_tiempo(const char *ruta_archivo,int n,double tiempo){
    FILE *archivo = fopen(ruta_archivo, "a");

    if (archivo == NULL){
        printf("Error al acceder al archivo de guardado de tiempos\n");
        return;
    }

    fprintf(archivo, "%d , %.6f\n",n,tiempo);

    fclose(archivo);
}

void medicion_general(){

    FILE *archivo = fopen("db/medicion_general.csv", "w");

    if (archivo == NULL){
        printf("Error al acceder al archivo de mediciones generales\n");
        return;
    }

    fprintf(archivo,"n Deportistas , bubble, insertion, selection, cocktail, merge ,merge_insertion, quick_ultimo, quick_primer, quick_rand, quick_med_3\n");

    leer_deportistas_medicion(arreglo_a,1000,"mediciones/datos_1k.csv");
    medicion_experimento(archivo,1000);
    leer_deportistas_medicion(arreglo_b,5000,"mediciones/datos_5k.csv");
    medicion_experimento(archivo,5000);
    leer_deportistas_medicion(arreglo_c,10000,"mediciones/datos_10k.csv");
    medicion_experimento(archivo,10000);
    leer_deportistas_medicion(arreglo_d,20000,"mediciones/datos_20k.csv");
    medicion_experimento(archivo,20000);

    fclose(archivo);
    
    
}

void medicion_experimento(FILE *archivo, int cantidad){

    Deportista *original = NULL;

    if(cantidad == 1000){
        original = arreglo_a;
    }else if(cantidad == 5000){
        original = arreglo_b;
    }else if(cantidad == 10000){
        original = arreglo_c;
    }else if(cantidad == 20000){
        original = arreglo_d;
    }

    Deportista *copia = malloc(sizeof(Deportista) * cantidad);

    // Bubble sort
    memcpy(copia, original, sizeof(Deportista) * cantidad);
    clock_t ini_time_1 = clock();
    bubble_sort(copia, cantidad, 1);
    clock_t fin_time_1 = clock();

    // Insertion sort
    memcpy(copia, original, sizeof(Deportista) * cantidad);
    clock_t ini_time_2 = clock();
    insertion_sort(copia, cantidad, 1);
    clock_t fin_time_2 = clock();

    // Selection sort
    memcpy(copia, original, sizeof(Deportista) * cantidad);
    clock_t ini_time_3 = clock();
    selection_sort(copia, cantidad, 1);
    clock_t fin_time_3 = clock();

    // Cocktail sort
    memcpy(copia, original, sizeof(Deportista) * cantidad);
    clock_t ini_time_4 = clock();
    cocktail_shaker_sort(copia, cantidad, 1);
    clock_t fin_time_4 = clock();

    // Merge sort basico
    memcpy(copia, original, sizeof(Deportista) * cantidad);
    clock_t ini_time_5 = clock();
    merge_sort(copia, cantidad, 1, 1);
    clock_t fin_time_5 = clock();

    // Merge sort hibrido
    memcpy(copia, original, sizeof(Deportista) * cantidad);
    clock_t ini_time_6 = clock();
    merge_sort(copia, cantidad, 1, 500);
    clock_t fin_time_6 = clock();

    // Quicksort ultimo elemetno pivote
    memcpy(copia, original, sizeof(Deportista) * cantidad);
    clock_t ini_time_7 = clock();
    quick_sort(copia, cantidad, 1, 1);
    clock_t fin_time_7 = clock();

    // QUicksort primer elemento pivote
    memcpy(copia, original, sizeof(Deportista) * cantidad);
    clock_t ini_time_8 = clock();
    quick_sort(copia, cantidad, 1, 2);
    clock_t fin_time_8 = clock();

    // Quicksort elemento random pivote

    memcpy(copia, original, sizeof(Deportista) * cantidad);
    clock_t ini_time_9 = clock();
    quick_sort(copia, cantidad, 1, 3);
    clock_t fin_time_9 = clock();

    // Quicksort mediana de 3 pivote
    memcpy(copia, original, sizeof(Deportista) * cantidad);
    clock_t ini_time_10 = clock();
    quick_sort(copia, cantidad, 1, 4);
    clock_t fin_time_10 = clock();

    free(copia);

    double tiempo_1 = (double)(fin_time_1 - ini_time_1) / CLOCKS_PER_SEC;
    double tiempo_2 = (double)(fin_time_2 - ini_time_2) / CLOCKS_PER_SEC;
    double tiempo_3 = (double)(fin_time_3 - ini_time_3) / CLOCKS_PER_SEC;
    double tiempo_4 = (double)(fin_time_4 - ini_time_4) / CLOCKS_PER_SEC;
    double tiempo_5 = (double)(fin_time_5 - ini_time_5) / CLOCKS_PER_SEC;
    double tiempo_6 = (double)(fin_time_6 - ini_time_6) / CLOCKS_PER_SEC;
    double tiempo_7 = (double)(fin_time_7 - ini_time_7) / CLOCKS_PER_SEC;
    double tiempo_8 = (double)(fin_time_8 - ini_time_8) / CLOCKS_PER_SEC;
    double tiempo_9 = (double)(fin_time_9 - ini_time_9) / CLOCKS_PER_SEC;
    double tiempo_10 = (double)(fin_time_10 - ini_time_10) / CLOCKS_PER_SEC;

    fprintf(archivo,
    "%d,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n",cantidad,tiempo_1,tiempo_2,tiempo_3,tiempo_4,tiempo_5,tiempo_6,tiempo_7,tiempo_8,tiempo_9,tiempo_10);
}


void leer_deportistas_medicion(Deportista *arreglo, int cantidad_maxima, const char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s para leer.\n", nombre_archivo);
    }
    
    char linea[256];
    int contador = 0;
    
    // Leer la primera línea (cabecera) y descartarla
    if (fgets(linea, sizeof(linea), archivo) == NULL) {
        fclose(archivo);
    }
    
    // Leer línea por línea hasta terminar el archivo o llegar al límite
    while (fgets(linea, sizeof(linea), archivo) != NULL && contador < cantidad_maxima) {
        sscanf(linea, "%d,%[^,],%[^,],%f,%d", &arreglo[contador].id, arreglo[contador].nombre, arreglo[contador].equipo, &arreglo[contador].puntaje, &arreglo[contador].competencias);
        contador++;
    }
    
    fclose(archivo);
}