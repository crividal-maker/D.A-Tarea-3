#ifndef MEDICION_H
#define MEDICION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "deportista.h"


// Prototipos
void guardar_tiempo(const char *,int,double );
void medicion_general();

void medicion_experimento(FILE *,int);
void leer_deportistas_medicion(Deportista *arreglo, int, const char *);
#endif