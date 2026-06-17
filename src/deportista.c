#include "deportista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Arreglo global con las sílabas o pares de letras más comunes
const char *silabas[] = {
    "fe", "ma", "pe", "pa", "so", "fi", "lu", "ca", 
    "ro", "di", "na", "to", "le", "mi", "sa", "va", 
    "da", "re", "ti", "no", "ja", "co", "ba", "se"
};

#define NUM_SILABAS (sizeof(silabas) / sizeof(silabas[0]))

// Arreglo global con equipos de la liga brasileña
const char *equipos_brasil[] = {
    "Flamengo", "Palmeiras", "Sao Paulo", "Corinthians", 
    "Fluminense", "Gremio", "Internacional", "Atletico Mineiro", 
    "Cruzeiro", "Botafogo", "Vasco da Gama", "Santos"
};

#define NUM_EQUIPOS (sizeof(equipos_brasil) / sizeof(equipos_brasil[0]))

// Función para generar un nombre aleatorio a partir de sílabas
void generar_nombre_aleatorio(char *destino, int longitud_maxima) {
    destino[0] = '\0';
    int longitud_actual = 0;
    int cantidad_silabas_nombre = (rand() % 3) + 2; 

    for (int i = 0; i < cantidad_silabas_nombre; i++) {
        int indice_aleatorio = rand() % NUM_SILABAS;
        const char *silaba = silabas[indice_aleatorio];
        int len_silaba = strlen(silaba);

        if (longitud_actual + len_silaba < longitud_maxima) {
            strcat(destino, silaba);
            longitud_actual += len_silaba;
        } else {
            break;
        }
    }

    if (longitud_actual > 0) {
        destino[0] = toupper(destino[0]);
    }
}

// Variable global (privada para este archivo) para mantener el autoincremental
static int contador_id = 1;

// Permite reiniciar el ID para que vuelva a 1 y reemplace los anteriores sin excederse
void reiniciar_generador_id() {
    contador_id = 1;
}

// Función que crea un deportista, le asigna su ID y su nombre aleatorio
Deportista generar_deportista() {
    Deportista nuevo_deportista;
    
    nuevo_deportista.id = contador_id++;
    generar_nombre_aleatorio(nuevo_deportista.nombre, MAX_LONGITUD_NOMBRE);
    
    // Asignar un equipo de forma aleatoria
    int indice_equipo = rand() % NUM_EQUIPOS;
    strncpy(nuevo_deportista.equipo, equipos_brasil[indice_equipo], MAX_LONGITUD_EQUIPO);
    nuevo_deportista.equipo[MAX_LONGITUD_EQUIPO - 1] = '\0'; // Asegurar que el string termine correctamente

    // Generar puntaje aleatorio (ejemplo: entre 50.0 y 100.0 con 1 decimal)
    nuevo_deportista.puntaje = 50.0 + ((float)(rand() % 501) / 10.0);
    
    // Generar cantidad de competencias jugadas (ejemplo: entre 1 y 40)
    nuevo_deportista.competencias = (rand() % 40) + 1;

    return nuevo_deportista;
}

// Función para generar un deportista con un ID específico
Deportista generar_deportista_con_id_especifico(int id) {
    Deportista nuevo_deportista;
    
    nuevo_deportista.id = id;
    generar_nombre_aleatorio(nuevo_deportista.nombre, MAX_LONGITUD_NOMBRE);
    
    // Asignar un equipo de forma aleatoria
    int indice_equipo = rand() % NUM_EQUIPOS;
    strncpy(nuevo_deportista.equipo, equipos_brasil[indice_equipo], MAX_LONGITUD_EQUIPO);
    nuevo_deportista.equipo[MAX_LONGITUD_EQUIPO - 1] = '\0'; // Asegurar que el string termine correctamente

    // Generar puntaje aleatorio (ejemplo: entre 50.0 y 100.0 con 1 decimal)
    nuevo_deportista.puntaje = 50.0 + ((float)(rand() % 501) / 10.0);
    
    // Generar cantidad de competencias jugadas (ejemplo: entre 1 y 40)
    nuevo_deportista.competencias = (rand() % 40) + 1;

    return nuevo_deportista;
}

// Genera un arreglo de deportistas ordenado por ID (mejor caso para muchos sorts)
void generar_datos_ordenados(Deportista *arreglo, int cantidad) {
    for (int i = 0; i < cantidad; i++) {
        arreglo[i] = generar_deportista_con_id_especifico(i + 1); // IDs de 1 a cantidad
    }
}

// Genera un arreglo de deportistas inversamente ordenado por ID (peor caso para muchos sorts)
void generar_datos_inversos(Deportista *arreglo, int cantidad) {
    for (int i = 0; i < cantidad; i++) {
        arreglo[i] = generar_deportista_con_id_especifico(cantidad - i); // IDs de cantidad a 1
    }
}

// Algoritmo de Fisher-Yates para mezclar aleatoriamente el arreglo
void mezclar_deportistas(Deportista *arreglo, int cantidad) {
    for (int i = cantidad - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        
        // Intercambiar arreglo[i] y arreglo[j]
        Deportista temporal = arreglo[i];
        arreglo[i] = arreglo[j];
        arreglo[j] = temporal;
    }
}

// Función para guardar el arreglo de deportistas en un archivo CSV
void guardar_deportistas_csv(Deportista *arreglo, int cantidad, const char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s para escribir.\n", nombre_archivo);
        return;
    }
    
    // Escribir cabecera
    fprintf(archivo, "ID,Nombre,Equipo,Puntaje,Competencias\n");
    
    // Escribir los registros
    for (int i = 0; i < cantidad; i++) {
        fprintf(archivo, "%d,%s,%s,%.1f,%d\n", arreglo[i].id, arreglo[i].nombre, arreglo[i].equipo, arreglo[i].puntaje, arreglo[i].competencias);
    }
    
    fclose(archivo);
}

// Función para leer los registros desde el CSV a un arreglo en memoria
int leer_deportistas_csv(Deportista *arreglo, int cantidad_maxima, const char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s para leer.\n", nombre_archivo);
        return 0;
    }
    
    char linea[256];
    int contador = 0;
    
    // Leer la primera línea (cabecera) y descartarla
    if (fgets(linea, sizeof(linea), archivo) == NULL) {
        fclose(archivo);
        return 0; // Archivo vacío
    }
    
    // Leer línea por línea hasta terminar el archivo o llegar al límite
    while (fgets(linea, sizeof(linea), archivo) != NULL && contador < cantidad_maxima) {
        sscanf(linea, "%d,%[^,],%[^,],%f,%d", &arreglo[contador].id, arreglo[contador].nombre, arreglo[contador].equipo, &arreglo[contador].puntaje, &arreglo[contador].competencias);
        contador++;
    }
    
    fclose(archivo);
    return contador; // Retornamos cuántos registros leímos exitosamente
}

void imprimir_deportista(Deportista d) {
    printf("ID: %-3d | Nombre: %-12s | Equipo: %-18s | Puntaje: %6.1f | Competencias: %d\n", 
           d.id, d.nombre, d.equipo, d.puntaje, d.competencias);
}
