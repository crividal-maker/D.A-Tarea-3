#include "quicksort.h"
#include "ordenamiento.h"
#include <stdlib.h>

// Función auxiliar para intercambiar dos elementos
static void intercambiar(Deportista *a, Deportista *b) {
    Deportista temp = *a;
    *a = *b;
    *b = temp;
}

// =====================================================================
// FUNCIONES PARA QUICK SORT
// =====================================================================

// Función para encontrar la mediana de tres (Inicio, Medio, Fin)
static int mediana_de_tres(Deportista *arreglo, int inicio, int fin, int campo) {
    int medio = inicio + (fin - inicio) / 2;
    if (comparar_deportistas(arreglo[inicio], arreglo[medio], campo) > 0) intercambiar(&arreglo[inicio], &arreglo[medio]);
    if (comparar_deportistas(arreglo[inicio], arreglo[fin], campo) > 0) intercambiar(&arreglo[inicio], &arreglo[fin]);
    if (comparar_deportistas(arreglo[medio], arreglo[fin], campo) > 0) intercambiar(&arreglo[medio], &arreglo[fin]);
    return medio;
}

static int particion_lomuto(Deportista *arreglo, int inicio, int fin, int campo, TipoPivote tipo) {
    int indice_pivote = fin; // Por defecto el último

    // Selección de pivote según las variantes del PDF
    if (tipo == PIVOTE_PRIMERO) {
        indice_pivote = inicio;
    } else if (tipo == PIVOTE_ALEATORIO) {
        indice_pivote = inicio + rand() % (fin - inicio + 1);
    } else if (tipo == PIVOTE_MEDIANA) {
        indice_pivote = mediana_de_tres(arreglo, inicio, fin, campo);
    }

    // Movemos el pivote elegido al final para aplicar Lomuto estándar
    intercambiar(&arreglo[indice_pivote], &arreglo[fin]);
    
    Deportista pivote = arreglo[fin];
    int i = (inicio - 1);

    for (int j = inicio; j <= fin - 1; j++) {
        if (comparar_deportistas(arreglo[j], pivote, campo) <= 0) {
            i++;
            intercambiar(&arreglo[i], &arreglo[j]);
        }
    }
    intercambiar(&arreglo[i + 1], &arreglo[fin]);
    return (i + 1);
}

static void quick_sort_recursivo(Deportista *arreglo, int inicio, int fin, int campo, TipoPivote tipo) {
    if (inicio < fin) {
        int pi = particion_lomuto(arreglo, inicio, fin, campo, tipo);
        quick_sort_recursivo(arreglo, inicio, pi - 1, campo, tipo);
        quick_sort_recursivo(arreglo, pi + 1, fin, campo, tipo);
    }
}

void quick_sort(Deportista *arreglo, int cantidad, int campo, TipoPivote tipo) {
    if (cantidad > 1) {
        quick_sort_recursivo(arreglo, 0, cantidad - 1, campo, tipo);
    }
}

// =====================================================================
// FUNCIONES PARA QUICK SELECT (K-ésimo mejor deportista)
// =====================================================================

// Función auxiliar para Quick Select (Partición de Lomuto Descendente por Puntaje)
static int particion_lomuto_descendente(Deportista *arreglo, int inicio, int fin) {
    Deportista pivote = arreglo[fin];
    int i = inicio - 1;

    for (int j = inicio; j <= fin - 1; j++) {
        // Compara por puntaje descendente (queremos los mayores a la izquierda)
        if (arreglo[j].puntaje >= pivote.puntaje) {
            i++;
            intercambiar(&arreglo[i], &arreglo[j]);
        }
    }
    
    // Colocar el pivote en su posición final
    intercambiar(&arreglo[i + 1], &arreglo[fin]);
    
    return i + 1;
}

// Función principal Quick Select
Deportista quick_select(Deportista *arreglo, int inicio, int fin, int k) {
    // Si el arreglo tiene un solo elemento, retornarlo
    if (inicio == fin) {
        return arreglo[inicio];
    }

    // pi es el índice de partición
    int pi = particion_lomuto_descendente(arreglo, inicio, fin);

    // Cantidad de elementos en la parte izquierda del pivote (incluyendo el pivote)
    int cantidad_izquierda = pi - inicio + 1;

    // Si k coincide con el tamaño del lado izquierdo, el pivote es el k-ésimo mejor
    if (k == cantidad_izquierda) {
        return arreglo[pi];
    } 
    // Si k es menor, el elemento está en la mitad izquierda
    else if (k < cantidad_izquierda) {
        return quick_select(arreglo, inicio, pi - 1, k);
    } 
    // Si k es mayor, el elemento está en la mitad derecha.
    // Ajustamos k restándole los elementos que ya dejamos atrás.
    else {
        return quick_select(arreglo, pi + 1, fin, k - cantidad_izquierda);
    }
}