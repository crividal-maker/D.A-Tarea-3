#ifndef ORDENAMIENTO_H
#define ORDENAMIENTO_H

#include "deportista.h" // Necesita la estructura Deportista

// Función auxiliar para comparar deportistas según el campo seleccionado
int comparar_deportistas(Deportista a, Deportista b, int campo);

// Algoritmos de ordenamiento
void ordenar_ranking_descendente(Deportista *arreglo, int cantidad);
void bubble_sort(Deportista *arreglo, int cantidad, int campo);
void insertion_sort(Deportista *arreglo, int cantidad, int campo);
void selection_sort(Deportista *arreglo, int cantidad, int campo);
void cocktail_shaker_sort(Deportista *arreglo, int cantidad, int campo);

#endif // ORDENAMIENTO_H