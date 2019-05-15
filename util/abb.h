#ifndef P1COMPILADORES_ABB_H
#define P1COMPILADORES_ABB_H

//////////////////////////////////////////////
/*  Estructuras y tipos de datos    */
typedef struct Elemento{
    char *lexema;
    int identificador;
}Elemento;

struct Arbol {
    struct Elemento * nodo;
    struct Arbol *izq, *der;
};

typedef struct Elemento* Nodo;
typedef struct Arbol* abb;
/////////////////////////////////////////////
/*  Metodos     */
void crearArbol(abb *A);
void destruirArbol(abb *A);
void insertarNodo(abb *A, Nodo n);
int buscarNodo(abb arbol, char* clave);
int existeNodo(abb arbol, char* clave);
void imprimirArbol(abb* A);
abb izq(abb A);
abb der(abb A);
unsigned esMiembro(abb A, Nodo n);
unsigned esVacio(abb A);
//////////////////////////////////////////////

#endif //P1COMPILADORES_ABB_H