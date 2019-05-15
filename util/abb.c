#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abb.h"

//////////////////////////////////////////////
abb izq(abb A) {
    return A->izq;
}

abb der(abb A) {
    return A->der;
}

unsigned esVacio(abb A) {
    return A==NULL;
}

unsigned esMiembro(abb A, Nodo E) {
    if (esVacio(A))
        return 0;
    else if (strcmp(E->lexema, A->nodo->lexema)==0)
        return 1;
    else if (strcmp(E->lexema, A->nodo->lexema)>0)
        return esMiembro(A->der, E);
    else
        return esMiembro(A->izq, E);
}
//////////////////////////////////////////////

void crearArbol(abb *A) {
    *A = NULL;
}

void destruirArbol(abb *A) {
    if (*A != NULL) {
        destruirArbol(&(*A)->izq);
        destruirArbol(&(*A)->der);
        free(*A);
        *A = NULL;
    }
}

void insertarNodo(abb *A, Nodo E) {
    if (esVacio(*A)) {
        *A = (abb) malloc(sizeof(struct Elemento));
        (*A)->nodo = E;
        (*A)->izq = NULL;
        (*A)->der = NULL;
    }else if (strcmp(E->lexema, ((*A)->nodo)->lexema)<0)
        insertarNodo(&(*A)->izq, E);
    else
        insertarNodo(&(*A)->der, E);
}

void imprimirArbol(abb* A){
    if(!esVacio(*A)){
        imprimirArbol(&(*A)->izq);
        printf("%s -> %d\n", (*A)->nodo->lexema, (*A)->nodo->identificador);
        imprimirArbol(&(*A)->der);
    }
}

int existeNodo(abb A, char* clave){
    if (esVacio(A))
        return 1;   // no existe
    else if (strcmp(clave, A->nodo->lexema)==0)
        return 0;   // existe
    else if (strcmp(clave, A->nodo->lexema)<0)
        existeNodo(izq(A), clave);
    else
        existeNodo(der(A), clave);
}

int buscarNodo(abb A, char* clave) {
    if (esVacio(A))
        printf("Clave inexistente\n");
    else if (strcmp(clave, A->nodo->lexema)==0)
        return A->nodo->identificador;
    else if (strcmp(clave, A->nodo->lexema)<0)
        buscarNodo(izq(A), clave);
    else
        buscarNodo(der(A), clave);
}