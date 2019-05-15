#ifndef P1COMPILADORES_TABLASIMBOLOS_H
#define P1COMPILADORES_TABLASIMBOLOS_H

#include "../util/abb.h"
#include "stdbool.h"

/* Funcion que inicializa la tabla de simbolos */
void inicializarTablaSimbolos();
/* Funcion que busca un componente y devuelve su identificador*/
int buscarElemento(char *lexema);
/* Funcion que comprueba si la tabla tiene o no un elemento */
bool existeElemento(char *lexema);
/* Funcion que inserta un componente en la tabla de simbolos */
void insertarComponente(Nodo *nodo, char *lexema, int id);
/* Funcion que imprime el contenido de la tabla de simbolos */
void imprimirTablaSimbolos();
/* Funcion que libera la tabla de simbolos */
void liberarTablaSimbolos();

#endif //P1COMPILADORES_TABLASIMBOLOS_H