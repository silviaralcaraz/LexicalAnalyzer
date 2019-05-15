#ifndef P1COMPILADORES_LEXICO_H
#define P1COMPILADORES_LEXICO_H

#include <stdbool.h>
#include "../util/abb.h"

/* Funcion que solicita el siguiente caracter al sistema de entrada */
char recibirCaracter();
/* Funcion que inicia el analisis lexico y envia componentes lexicos
 * al Analizador Sintactico */
Nodo analisisLexico();
/* Funcion que crea un componente con la estructura adecuada */
void crearLexema(Nodo *nodo, char* lexema, int id);

//////////////////////////////////////////////////

/* Funcion que comprueba si un caracter es un simbolo */
bool esSimbolo();
/* Funcion que identifica un simbolo determinado */
void detectarSimbolo();
/* Funcion que comprueba si un caracter es un operador */
bool esOperador();
/* Funcion que identifica un operador determinado */
void detectarOperador();
/* Funcion que detecta los comentarios multilinea */
void detectarComentarioMultilinea();
/* Funcion que detecta strings */
void detectarString();
/* Funcion que determina el tipo de un numero */
void tipoNumero(int identificador);
/* Funcion que detecta numeros */
void detectarNumeros();
/* Funcion que detecta identificadores */
void detectarIdentificador();
/* Funcion que solicita que se mueva el puntero delantero y
 * reduce en 1 el num de caracteres leidos */
void retrasarDelantero();

#endif //P1COMPILADORES_LEXICO_H