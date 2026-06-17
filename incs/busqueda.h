#ifndef BUSQUEDA_H
#define BUSQUEDA_H

#include "deportista.h" // Necesita la estructura Deportista

// Búsqueda Secuencial 
int busqueda_secuencial(Deportista *arreglo, int cantidad, int id_buscar);

// Búsqueda Binaria Iterativa por ID 
int busqueda_binaria_iterativa(Deportista *arreglo, int cantidad, int id_buscar);

// Búsqueda Binaria Recursiva por ID 
int busqueda_binaria_recursiva(Deportista *arreglo, int izquierda, int derecha, int id_buscar);

/*
 * Búsqueda Binaria para encontrar todos los elementos dentro de un rango de IDs
 * Devuelve la posición de la primera y última ocurrencia de los elementos dentro del rango
 * Si no se encuentran elementos dentro del rango, devuelve -1 en ambas posiciones
 * Nota: El arreglo debe estar ordenado por ID para que esta función funcione correctamente
 */
void busqueda_binaria_rangos(Deportista *arreglo, int cantidad, int id_min, int id_max, int *primera_pos, int *ultima_pos);

/*
 * Búsqueda Exponencial por ID (para arreglos grandes, encuentra un rango donde el ID podría estar y luego hace una búsqueda binaria dentro de ese rango)
 * Devuelve el índice del elemento encontrado o -1 si no se encuentra
 */
int busqueda_exponencial(Deportista *arreglo, int cantidad, int id_buscar);

/*
    * Búsqueda por Interpolación por ID (similar a la búsqueda binaria pero calcula la posición de manera más inteligente asumiendo una distribución uniforme de los IDs)
    * Devuelve el índice del elemento encontrado o -1 si no se encuentra
*/
int busqueda_interpolacion(Deportista *arreglo, int cantidad, int id_buscar);

#endif // BUSQUEDA_H