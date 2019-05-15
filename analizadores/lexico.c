#include <ctype.h>
#include <memory.h>
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "../definiciones.h"
#include "../entrada/sistemaEntrada.h"
#include "../simbolos/tablaSimbolos.h"
#include "../errores/errores.h"

bool devolverComponente = false;
char nuevoCaracter, primerCaracter;
int numLinea = 1, numColumna = 0;
Nodo nuevoComponente;
char letrasHexadecimales[6] = {"abcdef"};

void crearLexema(Nodo *nodo, char *lexema, int id) {
    *nodo = malloc(sizeof(struct Elemento));
    (*nodo)->identificador = id;
    (*nodo)->lexema = lexema;
    devolverComponente = true;
    numColumna = 0;
}

char recibirCaracter() {
    numColumna++;
    return sigCaracter(numColumna);
}

void retrasarDelantero() {
    numColumna--;
    if (nuevoCaracter != EOF && nuevoCaracter != OVERFLOW_MAXTAMLEX) {
        moverDelantero();
    }
}

/* Metodo privado para saber si un caracter esta contenido en un array */
bool contenerCaracter(char caracter, char *array) {
    bool contener = false;
    for (int i = 0; i < sizeof(array); i++) {
        if (caracter == array[i] || caracter == toupper(array[i])) { // se incluyen tanto minusculas como mayusculas
            contener = true;
            break;
        }
    }
    return contener;
}

bool esSimbolo() {
    if (strchr(simbolos, nuevoCaracter) != NULL) {  // Compruebo que el simbolo esta en la lista de simbolos
        return true;
    } else {
        return false;
    }
}

bool esOperador() {
    if (strchr(operadores, nuevoCaracter) != NULL) {  // Compruebo que el operador esta en la lista de operadores
        return true;
    } else {
        return false;
    }
}

void tipoNumero(int identificador) {
    nuevoCaracter = recibirCaracter();
    int numDigitos = 0;
    switch (identificador) {
        case FLOAT:
            if (nuevoCaracter == '-' || isdigit(nuevoCaracter)) { // Se incluye el caso de que pueda ser negativo
                nuevoCaracter = recibirCaracter();
                while (isdigit(nuevoCaracter)) {
                    nuevoCaracter = recibirCaracter();
                }
            }
            if (nuevoCaracter == 'e') { // float con notacion cientifica
                nuevoCaracter = recibirCaracter();
                if (nuevoCaracter == '-' || isdigit(nuevoCaracter)) { // Se incluye el caso de que pueda ser negativo
                    char primeroAux = nuevoCaracter; // almaceno si era guion o numero para comprobar un posible error
                    nuevoCaracter = recibirCaracter();
                    if (isdigit(nuevoCaracter)) {
                        while (isdigit(nuevoCaracter)) {
                            nuevoCaracter = recibirCaracter();
                        }
                        if(isalpha(nuevoCaracter)){
                            imprimirError(FLOAT_MALFORMADO, numLinea, numColumna);
                            crearLexema(&nuevoComponente, formarLexema(), identificador);
                            return;
                        }
                    } else if (!isdigit(nuevoCaracter) && primeroAux == '-') {
                        imprimirError(FLOAT_MALFORMADO, numLinea, numColumna);
                        crearLexema(&nuevoComponente, formarLexema(), identificador);
                        return;
                    } else if(isalpha(nuevoCaracter)){
                        imprimirError(FLOAT_MALFORMADO, numLinea, numColumna);
                        crearLexema(&nuevoComponente, formarLexema(), identificador);
                        return;
                    }
                } else { // Si en este punto llega algun caracter que no es un digito, seria un error
                    imprimirError(FLOAT_MALFORMADO, numLinea, numColumna);
                    crearLexema(&nuevoComponente, formarLexema(), identificador);
                    return;
                }
            } else if (nuevoCaracter == '.') {  // float con .
                nuevoCaracter = recibirCaracter();
                while (isdigit(nuevoCaracter)) {
                    nuevoCaracter = recibirCaracter();
                }
                if(isalpha(nuevoCaracter)){
                    imprimirError(FLOAT_MALFORMADO, numLinea, numColumna);
                    crearLexema(&nuevoComponente, formarLexema(), identificador);
                    return;
                }
            } else { // Si en este punto llega algun caracter que no es un digito, seria un error
                imprimirError(FLOAT_MALFORMADO, numLinea, numColumna);
                break;
            }
            break;
        case HEXADECIMAL:
            if (nuevoCaracter == '0') {
                nuevoCaracter = recibirCaracter();
                if (nuevoCaracter == 'x') {
                    nuevoCaracter = recibirCaracter();
                    while (isdigit(nuevoCaracter) || contenerCaracter(nuevoCaracter, letrasHexadecimales)) {
                        nuevoCaracter = recibirCaracter();
                        numDigitos++;
                        // Si hay mas de 8 digitos (o letras permitidas) despues de la 'x', se trata de un error
                        if (numDigitos > 8) {
                            imprimirError(HEXADECIMAL_MALFORMADO, numLinea, numColumna);
                            break;
                        }
                    }
                    if(isalpha(nuevoCaracter)){
                        imprimirError(HEXADECIMAL_MALFORMADO, numLinea, numColumna);
                        crearLexema(&nuevoComponente, formarLexema(), identificador);
                        return;
                    }
                } else { // Si despues del 0 no va una x, es un hexadecimal mal formado
                    imprimirError(HEXADECIMAL_MALFORMADO, numLinea, numColumna);
                    break;
                }
            }
    }
    retrasarDelantero();
    crearLexema(&nuevoComponente, formarLexema(), identificador);
}

void detectarIdentificador() {
    while (isalpha(nuevoCaracter) || isdigit(nuevoCaracter) || nuevoCaracter == '_') {
        nuevoCaracter = recibirCaracter();
        if (nuevoCaracter == OVERFLOW_MAXTAMLEX) {
            imprimirError(OVERFLOW_MAXTAMLEX, numLinea, numColumna);
            break;
        }
    }
    retrasarDelantero();
    crearLexema(&nuevoComponente, formarLexema(), IDENTIFICADOR);
}

void detectarNumeros() {
    primerCaracter = nuevoCaracter;
    if (primerCaracter == '-' || isdigit(primerCaracter)) { // se contempla que pueda ser un numero negativo
        while (isdigit(nuevoCaracter)) {
            nuevoCaracter = recibirCaracter();
        }
    }
    if (nuevoCaracter == '.' || nuevoCaracter == 'e') {     // Reiniciar analisis como float
        reiniciarLecturaComponente();
        numColumna = 0;
        tipoNumero(FLOAT);
        return;
    } else if (nuevoCaracter == 'x' && primerCaracter == '0') { // Reiniciar analisis como hexadecimal
        reiniciarLecturaComponente();
        numColumna = 0;
        tipoNumero(HEXADECIMAL);
        return;
    } else if (isalpha(nuevoCaracter)) {
        imprimirError(NUMERO_MALFORMADO, numLinea, numColumna);
        crearLexema(&nuevoComponente, formarLexema(), INTEGER);
        return;
    }
    retrasarDelantero();
    crearLexema(&nuevoComponente, formarLexema(), INTEGER);
}

void detectarSimbolo() {
    bool encontrar = false;
    int id;
    for (int i = 0; i < sizeof(simbolos); i++) {    // Se busca el caracter en la lista de simbolos
        if (nuevoCaracter == simbolos[i]) {
            encontrar = true;
            id = idSimbolos[i];
            break;
        }
    }
    if (encontrar) { // Una vez encontrado, se analizan los posibles casos
        if (nuevoCaracter == '.') {
            nuevoCaracter = recibirCaracter();
            if (isdigit(nuevoCaracter)) {   // Se comprueba la posibilidad de ser un float
                reiniciarLecturaComponente();
                numColumna = 0;
                tipoNumero(FLOAT);
                return;
            } else {
                retrasarDelantero();
            }
        }
        crearLexema(&nuevoComponente, formarLexema(), id);
    } else {
        return;
    }
}

void detectarOperador() {
    bool encontrado = false;
    int id = 0;
    for (int i = 0; i < sizeof(operadores); i++) { // Se busca el caracter en la lista de operadores
        if (nuevoCaracter == operadores[i]) {
            encontrado = true;
            id = idOperadores[i];
            break;
        }
    }
    if (encontrado) { // Una vez encontrado, se analizan los posibles casos
        if (primerCaracter == '=' || primerCaracter == '<' || primerCaracter == '+') {
            nuevoCaracter = recibirCaracter();
            if (nuevoCaracter == '=') {         // Caso: ==
                if (primerCaracter == '=') {
                    id = OP_IGUALIGUAL;
                } else if (primerCaracter == '<') {
                    id = OP_MENORIGUAL;
                } else if (primerCaracter == '+') {
                    id = OP_MASIGUAL;
                }
            } else {
                retrasarDelantero();
            }
        } else if (primerCaracter == '*') {
            nuevoCaracter = recibirCaracter();
            if (nuevoCaracter == '*') {         // Caso: **
                id = OP_POTENCIA;
            } else {
                retrasarDelantero();
            }
        } else if (primerCaracter == '-') {    // Caso: num negativo
            nuevoCaracter = recibirCaracter();
            if (isdigit(nuevoCaracter)) {
                detectarNumeros();
                return;
            } else {
                retrasarDelantero();
            }
        }
        crearLexema(&nuevoComponente, formarLexema(), id);
    } else {
        return; // no se corresponde con un operador
    }
}

void detectarComentarioMultilinea() {
    int numComillasDobles = 0, error = COMENTARIO_MULTILINEA_MALFORMADO;
    nuevoCaracter = recibirCaracter();
    while (nuevoCaracter == '\"') { // Se buscan las tres comillas dobles iniciales
        numComillasDobles++;
        if (numComillasDobles != 3) {
            nuevoCaracter = recibirCaracter();
        } else {
            break;
        }
    }
    nuevoCaracter = recibirCaracter();
    while (nuevoCaracter != '\"') { // Se busca el cuerpo del comentario en caso de haberlo
        if (nuevoCaracter == OVERFLOW_MAXTAMLEX) {
            // Si llega el id de overflow antes de detectar el cierre -> es un comentario malformado
            error = OVERFLOW_MAXTAMLEX;
            break;
        } else if (nuevoCaracter == EOF) {
            // Si llega EOF antes de detectar el cierre -> es un comentario malformado
            error = COMENTARIO_MULTILINEA_MALFORMADO;
            break;
        } else if (nuevoCaracter == '\n') {
            // Si se encuentran saltos de carro se aumenta el num de linea
            numLinea++;
        }
        nuevoCaracter = recibirCaracter();
    }
    if (nuevoCaracter == '\"') {    // Se busca el cierre del comentario: tres comillas dobles seguidas
        while (nuevoCaracter == '\"') {
            numComillasDobles++;
            if (numComillasDobles != 6) {
                nuevoCaracter = recibirCaracter();
            } else {
                break;
            }
        }
    }
    if (numComillasDobles != 6) {   // En caso de que no se hayan registrado 6 comillas dobles -> hubo un error
        imprimirError(error, numColumna, numLinea);
        retrasarDelantero();
        crearLexema(&nuevoComponente, formarLexema(), COMENTARIO_MULTILINEA);
    } else {
        crearLexema(&nuevoComponente, formarLexema(), COMENTARIO_MULTILINEA);
    }
}

void detectarString() {
    nuevoCaracter = recibirCaracter();
    if (primerCaracter == '\'') {   // Strings con comillas simples
        while (nuevoCaracter != '\'') {
            if (nuevoCaracter == EOF || nuevoCaracter == OVERFLOW_MAXTAMLEX || nuevoCaracter == '\n') {
                if (nuevoCaracter == OVERFLOW_MAXTAMLEX) {
                    imprimirError(OVERFLOW_MAXTAMLEX, numLinea, numColumna);
                } else {
                    // error -> no se encontro el cierre del string
                    imprimirError(STRING_MALFORMADO, numLinea, numColumna);
                }
                retrasarDelantero();
                break;
            }
            nuevoCaracter = recibirCaracter();
        }
        crearLexema(&nuevoComponente, formarLexema(), STRING);
    } else if (primerCaracter == '\"') {    // Strings con comillas dobles
        if (nuevoCaracter == '\"') {
            nuevoCaracter = recibirCaracter();
            if (nuevoCaracter == '\"') {    // Se trata de un comentario multilinea
                reiniciarLecturaComponente();
                numColumna = 0;
                detectarComentarioMultilinea();
                return;
            } else {
                retrasarDelantero();
                crearLexema(&nuevoComponente, formarLexema(), STRING); // String vacio: ""
                return;
            }
        } else {
            while (nuevoCaracter != '\"') {
                nuevoCaracter = recibirCaracter();
                if (nuevoCaracter == EOF || nuevoCaracter == OVERFLOW_MAXTAMLEX || nuevoCaracter == '\n') {
                    if (nuevoCaracter == OVERFLOW_MAXTAMLEX) {
                        imprimirError(OVERFLOW_MAXTAMLEX, numLinea, numColumna);
                    } else { // error -> no se encontro el cierre del string
                        imprimirError(STRING_MALFORMADO, numLinea, numColumna);
                    }
                    retrasarDelantero();
                    break;
                }
            }
            crearLexema(&nuevoComponente, formarLexema(), STRING);
            return;
        }
    }
}

Nodo analisisLexico() {
    devolverComponente = false;
    while (!devolverComponente) {
        nuevoCaracter = recibirCaracter();
        primerCaracter = nuevoCaracter;
        if (nuevoCaracter == EOF) {     // End Of File
            crearLexema(&nuevoComponente, "EOF", EOF);
            return nuevoComponente;
        } else if (nuevoCaracter == '#') {  // Comentarios simples -> se omiten
            while (nuevoCaracter != '\n') {
                nuevoCaracter = recibirCaracter();
                numColumna = 0;
                if (nuevoCaracter == EOF || nuevoCaracter == OVERFLOW_MAXTAMLEX) {
                    break;
                }
            }
            retrasarDelantero();
        } else if (nuevoCaracter == ' ') {  // Espacios -> se omiten
            while (nuevoCaracter == ' ') {
                nuevoCaracter = recibirCaracter();
                numColumna = 0;
            }
            retrasarDelantero();
        } else if (nuevoCaracter == '\n') {     // Saltos de linea
            crearLexema(&nuevoComponente, "\\n\0", '\n');
            notificarAvanzarInicio();
            numLinea++;
            return nuevoComponente;
        } else if (esSimbolo(nuevoCaracter)) {  // Simbolos
            detectarSimbolo();
        } else if (esOperador(nuevoCaracter)) {    // Operadores
            detectarOperador();
        } else if (nuevoCaracter == '\'' || nuevoCaracter == '\"') { // Strings o comentarios multilinea
            detectarString();
        } else if (isdigit(nuevoCaracter)) {    // Numeros
            detectarNumeros();
        } else if ((isalpha(nuevoCaracter) || (nuevoCaracter == '_'))) {    // Identificadores
            detectarIdentificador();
        } else {
            imprimirError(COMPONENTE_NO_RECONOCIDO, numLinea, numColumna);
            avanzarInicio(); // Se avanza cuando no se reconoce el caracter leido con ningun automata
        }
    }
    if (nuevoComponente->identificador == IDENTIFICADOR) {  // Se almacena el id en la tabla de simbolos si no existe ya
        if (!existeElemento(nuevoComponente->lexema)) {
            insertarComponente(&nuevoComponente, nuevoComponente->lexema, nuevoComponente->identificador);
        }
    }
    numColumna = 0;
    return nuevoComponente; // se envia al analizador sintactico el componente reconocido
}