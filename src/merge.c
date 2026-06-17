#include "merge.h"
#include "ordenamiento.h"

// Función auxiliar para ordenar sub-rangos pequeños usando Insertion Sort
static void insertion_sort_rango(Deportista *arreglo, int inicio, int fin, int campo) {
    for (int i = inicio + 1; i <= fin; i++) {
        Deportista clave = arreglo[i];
        int j = i - 1;
        while (j >= inicio && comparar_deportistas(arreglo[j], clave, campo) > 0) {
            arreglo[j + 1] = arreglo[j];
            j = j - 1;
        }
        arreglo[j + 1] = clave;
    }
}

// 1. Función auxiliar para fusionar dos subarreglos ordenados
static void merge(Deportista *arreglo, int inicio, int medio, int fin, int campo) {
    int n1 = medio - inicio + 1;
    int n2 = fin - medio;

    // Crear arreglos temporales
    Deportista L[n1], R[n2];

    // Copiar datos a los arreglos temporales L[] y R[]
    for (int i = 0; i < n1; i++) {
        L[i] = arreglo[inicio + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arreglo[medio + 1 + j];
    }

    // Fusionar los arreglos temporales de vuelta al arreglo original
    int i = 0; 
    int j = 0; 
    int k = inicio; 

    while (i < n1 && j < n2) {
        // Comparar los elementos de los arreglos temporales
        if (comparar_deportistas(L[i], R[j], campo) <= 0) {
            arreglo[k] = L[i];
            i++;
        } else {
            arreglo[k] = R[j];
            j++;
        }
        k++;
    }

    // Copiar los elementos restantes de L[], si hay alguno
    while (i < n1) {
        arreglo[k] = L[i];
        i++;
        k++;
    }

    // Copiar los elementos restantes de R[], si hay alguno
    while (j < n2) {
        arreglo[k] = R[j];
        j++;
        k++;
    }
}

// 2. Función recursiva principal de Merge Sort optimizado
static void merge_sort_recursivo(Deportista *arreglo, int inicio, int fin, int campo, int umbral) {
    // Optimización: Si el tamaño del subarreglo es menor o igual al umbral, aplicamos Insertion Sort
    if (fin - inicio + 1 <= umbral) {
        insertion_sort_rango(arreglo, inicio, fin, campo);
        return;
    }

    if (inicio < fin) {
        // Encuentra el punto medio para dividir el arreglo en dos mitades
        int medio = inicio + (fin - inicio) / 2;

        // Llamadas recursivas para la primera y segunda mitad
        merge_sort_recursivo(arreglo, inicio, medio, campo, umbral);
        merge_sort_recursivo(arreglo, medio + 1, fin, campo, umbral);

        // Fusiona las dos mitades ya ordenadas
        merge(arreglo, inicio, medio, fin, campo);
    }
}

// 3. Función envoltura (Wrapper) para exponer al main
void merge_sort(Deportista *arreglo, int cantidad, int campo, int umbral) {
    if (cantidad > 1) {
        merge_sort_recursivo(arreglo, 0, cantidad - 1, campo, umbral);
    }
}