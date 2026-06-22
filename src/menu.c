#include "menu.h"
#include <stdio.h>

void mostrar_menu() {
    printf("\n\033[0;32m========== MENU PRINCIPAL ==========\033[0m\n");
    printf("1. Generar, mezclar y guardar datos\n");
    printf("2. Cargar datos desde archivo\n");
    printf("3. Mostrar todos los registros\n");
    printf("4. Ordenar registros (Submenu)\n");
    printf("5. Buscar deportista por ID\n");
    printf("6. Mostrar ranking (Mejores N deportistas)\n");
    printf("7. Medicion de tiempos de Ordenamientos\n");
    printf("8. Buscar k-esimo mejor deportista (SQuickselect)\n");
    printf("9. Conformar equipo optimo (Programacion Dinamica - Memoizacion)\n");
    printf("10. Conformar equipo (Greedy - Menor costo primero)\n");
    printf("0. Salir\n");
    printf("\n\033[0;32m====================================\033[0m\n");
    printf("Seleccione una opcion: ");
}

void menu_ordenamiento() {
    printf("\n\033[0;32m--- ORDENAR POR ---\033[0m\n");
    printf("1. ID\n");
    printf("2. Nombre\n");
    printf("3. Equipo\n");
    printf("4. Puntaje\n");
    printf("5. Competencias\n");
    printf("Seleccione campo a ordenar: ");
}

// Nuevo menú para la generación de datos
void menu_generacion_datos() {
    printf("\n\033[0;32m--- GENERACION DE DATOS ---\033[0m\n");
    printf("1. Generar datos aleatorios (mezclados)\n");
    printf("2. Generar datos ordenados por ID (Mejor Caso)\n");
    printf("3. Generar datos inversamente ordenados por ID (Peor Caso)\n");
    printf("Seleccione tipo de generacion: ");
}