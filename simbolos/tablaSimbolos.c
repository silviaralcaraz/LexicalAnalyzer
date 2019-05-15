#include <memory.h>
#include <ctype.h>
#include "stdlib.h"
#include "stdio.h"
#include "../util/abb.h"
#include "tablaSimbolos.h"
#include "../definiciones.h"
#include "stdbool.h"

abb *tablaSimbolos;

void insertarComponente(Nodo *nodo, char *lexema, int id) {
    *nodo = malloc(sizeof(struct Elemento));
    (*nodo)->identificador = id;
    (*nodo)->lexema = lexema;
    insertarNodo(tablaSimbolos, *nodo);
}

void imprimirTablaSimbolos() {
    printf("\n-----------------------------"
           "\n-----TABLA DE SIMBOLOS-------\n"
           "-----------------------------\n"
           "(LEXEMA -> ID):\n");
    imprimirArbol(tablaSimbolos);
    printf("-----------------------------\n");
}

void inicializarTablaSimbolos() {
    tablaSimbolos = malloc(sizeof(abb *));
    int i = 0;
    Nodo nodo;
    while (componentesLexicos[i] != -1) { // se introducen las palabras reservadas en la tabla
        insertarComponente(&nodo, palabrasReservadas[i], componentesLexicos[i]);
        i++;
    }
}

bool existeElemento(char *lexema){
    if(existeNodo(*tablaSimbolos, lexema)==0){
        return true;
    }else if(existeNodo(*tablaSimbolos, lexema)==1){
        return false;
    }else{
        return false;
    }
}

int buscarElemento(char *lexema) {
    return buscarNodo(*tablaSimbolos, lexema);
}

void liberarTablaSimbolos() {
    destruirArbol(tablaSimbolos);
}