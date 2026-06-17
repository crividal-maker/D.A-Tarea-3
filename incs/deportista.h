#ifndef DEPORTISTA_H
#define DEPORTISTA_H

#define MAX_LONGITUD_NOMBRE 30
#define MAX_LONGITUD_EQUIPO 30

// Estructura base para el deportista
typedef struct {
    int id;
    char nombre[MAX_LONGITUD_NOMBRE];
    char equipo[MAX_LONGITUD_EQUIPO];
    float puntaje;
    int competencias;
    int costo;
} Deportista;

// Prototipos de funciones
void generar_nombre_aleatorio(char *destino, int longitud_maxima);
Deportista generar_deportista();
void reiniciar_generador_id();
void mezclar_deportistas(Deportista *arreglo, int cantidad);
void guardar_deportistas_csv(Deportista *arreglo, int cantidad, const char *nombre_archivo);
int leer_deportistas_csv(Deportista *arreglo, int cantidad_maxima, const char *nombre_archivo);
void imprimir_deportista(Deportista d);

// Nuevas funciones para generar casos específicos
Deportista generar_deportista_con_id_especifico(int id); // Función auxiliar
void generar_datos_ordenados(Deportista *arreglo, int cantidad);
void generar_datos_inversos(Deportista *arreglo, int cantidad);

#endif // DEPORTISTA_H