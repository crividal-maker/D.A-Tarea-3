#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "deportista.h"

// Definición de los tipos de pivote solicitados en la pauta
typedef enum {
    PIVOTE_ULTIMO = 1,
    PIVOTE_PRIMERO,
    PIVOTE_ALEATORIO,
    PIVOTE_MEDIANA
} TipoPivote;

// Prototipo para Quick Sort
void quick_sort(Deportista *arreglo, int cantidad, int campo, TipoPivote tipo);

// Prototipo para Quick Select (Para buscar el k-ésimo mejor deportista)
Deportista quick_select(Deportista *arreglo, int inicio, int fin, int k);

#endif