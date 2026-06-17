#include "ordenamiento.h"
#include <string.h> // Para strcmp

// Función auxiliar para comparar deportistas según el campo seleccionado
// Retorna > 0 si a > b, < 0 si a < b, 0 si son iguales
int comparar_deportistas(Deportista a, Deportista b, int campo) {
    switch(campo) {
        case 1: return a.id - b.id;
        case 2: return strcmp(a.nombre, b.nombre);
        case 3: return strcmp(a.equipo, b.equipo);
        case 4: return (a.puntaje > b.puntaje) - (a.puntaje < b.puntaje); // Evita problemas de truncamiento con floats
        case 5: return a.competencias - b.competencias;
        default: return 0;
    }
}

// Implementación de Bubble Sort optimizado para generar el ranking (Puntaje Descendente)
void ordenar_ranking_descendente(Deportista *arreglo, int cantidad) {
    for (int i = 0; i < cantidad - 1; i++) {
        int intercambiado = 0; // Optimización para detectar si ya está ordenado
        for (int j = 0; j < cantidad - i - 1; j++) {
            if (arreglo[j].puntaje < arreglo[j + 1].puntaje) {
                Deportista temp = arreglo[j];
                arreglo[j] = arreglo[j + 1];
                arreglo[j + 1] = temp;
                intercambiado = 1;
            }
        }
        if (!intercambiado) break; // Si no hubo cambios, terminamos temprano
    }
}

// 1. Bubble Sort (Multicampo) con optimización
void bubble_sort(Deportista *arreglo, int cantidad, int campo) {
    for (int i = 0; i < cantidad - 1; i++) {
        int intercambiado = 0;
        for (int j = 0; j < cantidad - i - 1; j++) {
            if (comparar_deportistas(arreglo[j], arreglo[j + 1], campo) > 0) {
                Deportista temp = arreglo[j];
                arreglo[j] = arreglo[j + 1];
                arreglo[j + 1] = temp;
                intercambiado = 1;
            }
        }
        if (!intercambiado) break; // Optimización solicitada
    }
}

// 2. Insertion Sort (Estándar)
void insertion_sort(Deportista *arreglo, int cantidad, int campo) {
    for (int i = 1; i < cantidad; i++) {
        Deportista clave = arreglo[i];
        int j = i - 1;
        while (j >= 0 && comparar_deportistas(arreglo[j], clave, campo) > 0) {
            arreglo[j + 1] = arreglo[j];
            j = j - 1;
        }
        arreglo[j + 1] = clave;
    }
}

// 3. Selection Sort (Optimizado para evitar intercambios innecesarios)
void selection_sort(Deportista *arreglo, int cantidad, int campo) {
    for (int i = 0; i < cantidad - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < cantidad; j++) {
            if (comparar_deportistas(arreglo[j], arreglo[min_idx], campo) < 0) {
                min_idx = j;
            }
        }
        if (min_idx != i) { // Optimización solicitada
            Deportista temp = arreglo[i];
            arreglo[i] = arreglo[min_idx];
            arreglo[min_idx] = temp;
        }
    }
}

// 4. Cocktail Shaker Sort (Estándar)
void cocktail_shaker_sort(Deportista *arreglo, int cantidad, int campo) {
    int intercambiado = 1;
    int inicio = 0;
    int fin = cantidad - 1;
    while (intercambiado) {
        intercambiado = 0;
        for (int i = inicio; i < fin; ++i) { // De izquierda a derecha
            if (comparar_deportistas(arreglo[i], arreglo[i + 1], campo) > 0) {
                Deportista temp = arreglo[i];
                arreglo[i] = arreglo[i + 1];
                arreglo[i + 1] = temp;
                intercambiado = 1;
            }
        }
        if (!intercambiado) break;
        intercambiado = 0;
        fin--;
        for (int i = fin - 1; i >= inicio; --i) { // De derecha a izquierda
            if (comparar_deportistas(arreglo[i], arreglo[i + 1], campo) > 0) {
                Deportista temp = arreglo[i];
                arreglo[i] = arreglo[i + 1];
                arreglo[i + 1] = temp;
                intercambiado = 1;
            }
        }
        inicio++;
    }
}