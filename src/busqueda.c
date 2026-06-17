#include "busqueda.h"
//Todas las funciones de búsqueda asumen que el arreglo de Deportistas está ordenado por ID de forma ascendente, excepto la búsqueda secuencial que no tiene esta restricción.
//--------------------------------------------------------------------------------------------------------

// Búsqueda Secuencial Estándar por ID
int busqueda_secuencial(Deportista *arreglo, int cantidad, int id_buscar) {
    for (int i = 0; i < cantidad; i++) {
        if (arreglo[i].id == id_buscar) {
            return i; // Encontrado
        }
    }
    return -1; // No encontrado
}

//--------------------------------------------------------------------------------------------------------

// Búsqueda Binaria Iterativa por ID
int busqueda_binaria_iterativa(Deportista *arreglo, int cantidad, int id_buscar) {
    int izquierda = 0;
    int derecha = cantidad - 1;
    while (izquierda <= derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;
        if (arreglo[medio].id == id_buscar) {
            return medio; // Encontrado
        }
        if (arreglo[medio].id < id_buscar) {
            izquierda = medio + 1;
        } else {
            derecha = medio - 1;
        }
    }
    return -1; // No encontrado
}

//--------------------------------------------------------------------------------------------------------

// Búsqueda Binaria Recursiva por ID
int busqueda_binaria_recursiva(Deportista *arreglo, int izquierda, int derecha, int id_buscar) {
    // No encontrado si el rango es inválido
    if (izquierda > derecha) return -1;

    int medio = izquierda + (derecha - izquierda) / 2;

    if (arreglo[medio].id == id_buscar) return medio;

    // Si el ID buscado es mayor, descartar mitad izquierda
    if (arreglo[medio].id < id_buscar)
        return busqueda_binaria_recursiva(arreglo, medio + 1, derecha, id_buscar);

    // Si el ID buscado es menor, descartar mitad derecha
    return busqueda_binaria_recursiva(arreglo, izquierda, medio - 1, id_buscar);
}

// Función auxiliar: encuentra la PRIMERA posición donde aparece id_min
static int buscar_primera_posicion(Deportista *arreglo, int cantidad, int id_min) {
    int izquierda = 0, derecha = cantidad - 1, resultado = -1;
    while (izquierda <= derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;
        if (arreglo[medio].id >= id_min) {
            if (arreglo[medio].id == id_min) resultado = medio;
            derecha = medio - 1; // Seguir buscando hacia la izquierda
        } else {
            izquierda = medio + 1;
        }
    }
    return resultado;
}

// Función auxiliar: encuentra la ÚLTIMA posición donde aparece id_max
static int buscar_ultima_posicion(Deportista *arreglo, int cantidad, int id_max) {
    int izquierda = 0, derecha = cantidad - 1, resultado = -1;
    while (izquierda <= derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;
        if (arreglo[medio].id <= id_max) {
            if (arreglo[medio].id == id_max) resultado = medio;
            izquierda = medio + 1; // Seguir buscando hacia la derecha
        } else {
            derecha = medio - 1;
        }
    }
    return resultado;
}

//--------------------------------------------------------------------------------------------------------

// Búsqueda Binaria para Rangos por ID
void busqueda_binaria_rangos(Deportista *arreglo, int cantidad, int id_min, int id_max,
                              int *primera_pos, int *ultima_pos) {
    *primera_pos = -1;
    *ultima_pos  = -1;

    if (id_min > id_max) return; // Rango inválido

    *primera_pos = buscar_primera_posicion(arreglo, cantidad, id_min);
    *ultima_pos  = buscar_ultima_posicion(arreglo, cantidad, id_max);

    // Si no se encontró ningún extremo, el rango no existe
    if (*primera_pos == -1 || *ultima_pos == -1) {
        *primera_pos = -1;
        *ultima_pos  = -1;
    }
}

//--------------------------------------------------------------------------------------------------------

// Búsqueda Exponencial por ID
int busqueda_exponencial(Deportista *arreglo, int cantidad, int id_buscar) {
    // Caso base: el primer elemento es el buscado
    if (arreglo[0].id == id_buscar) return 0;

    // Primero encontrar el rango duplicando el índice hasta que el ID sea mayor que el buscado o se alcance el final del arreglo
    int i = 1;
    while (i < cantidad && arreglo[i].id <= id_buscar) {
        i *= 2;
    }

    // Despues búsqueda binaria recursiva dentro del rango encontrado 
    int izquierda = i / 2;
    int derecha = (i < cantidad) ? i : cantidad - 1;

    return busqueda_binaria_recursiva(arreglo, izquierda, derecha, id_buscar);
}

//--------------------------------------------------------------------------------------------------------

// Búsqueda por Interpolación por ID
int busqueda_interpolacion(Deportista *arreglo, int cantidad, int id_buscar) {
    int izquierda = 0;
    int derecha = cantidad - 1;

    while (izquierda <= derecha &&
           id_buscar >= arreglo[izquierda].id &&
           id_buscar <= arreglo[derecha].id) {

        // Evitar division por cero
        if (arreglo[derecha].id == arreglo[izquierda].id) {
            if (arreglo[izquierda].id == id_buscar) return izquierda;
            return -1;
        }

        // Estimar la posición por interpolación
        int pos = izquierda + ((id_buscar - arreglo[izquierda].id) *
                  (derecha - izquierda)) /
                  (arreglo[derecha].id - arreglo[izquierda].id);

        if (arreglo[pos].id == id_buscar) return pos;

        if (arreglo[pos].id < id_buscar)
            izquierda = pos + 1;
        else
            derecha = pos - 1;
    }
    return -1;
}