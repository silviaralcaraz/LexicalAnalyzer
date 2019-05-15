#include <stdio.h>
#include <stdlib.h>
#include "sistemaEntrada.h"
#include "../definiciones.h"
#include <errno.h>
#include <memory.h>
#include "../errores/errores.h"
#include "stdbool.h"

FILE *archivo = NULL;
char *buffer_A, *buffer_B, *inicio, *delantero;
int centinela = EOF;
bool isBufferA = false; // true-> el ultimo buffer usado fue el A, false-> el ultimo fue el B (como se empieza en el A, se inicia a false)
bool avanzar = false; // booleano que indica cuando debe adelantarse el puntero de inicio
bool overflowTamMax = false; // booleano que indica cuando el lexema hasta ese punto leido sobrepasa el tam max del buffer

void abrirFichero(char *ruta) {
    archivo = fopen(ruta, "r");
    if (archivo == NULL) {
        int errnum = errno;
        imprimirErrorFichero(ERROR_FICHERO, strerror(errnum));
    }
}

int cerrarFichero() {
    fclose(archivo);
    return 0;
}

void inicializarSistemaEntrada() {
    // Se reserva memoria para tantos char como sea el tam max del buffer y para el centinela
    buffer_A = (char *) malloc((TAM_MAX_BUFFER + 1) * sizeof(char));
    buffer_B = (char *) malloc((TAM_MAX_BUFFER + 1) * sizeof(char));
    // Se coloca el centinela en la ultima posicion de los buffers
    buffer_A[TAM_MAX_BUFFER] = centinela;
    buffer_B[TAM_MAX_BUFFER] = centinela;
    // Se coloca inicio en el buffer A
    inicio = buffer_A;
}

void cargarBuffer() {
    if (isBufferA) {   // CARGAR BUFFER B
        int numCaracteres = fread(buffer_B, sizeof(char), TAM_MAX_BUFFER, archivo);
        if (numCaracteres < (TAM_MAX_BUFFER)) { // En caso de que el num de caracteres no llene el buffer
            buffer_B[numCaracteres] = EOF; // muevo el centinela a la ultima posicion
        }
        delantero = buffer_B; // delantero se coloca en la primera posicion del buffer correspondiente
        isBufferA = false; // Se indica que se ha usado el buffer B
    } else {      // CARGAR BUFFER A
        int numCaracteres = fread(buffer_A, sizeof(char), TAM_MAX_BUFFER, archivo);
        if (numCaracteres < (TAM_MAX_BUFFER)) {
            buffer_A[numCaracteres] = EOF;
        }
        delantero = buffer_A;
        isBufferA = true; // Se indica que se ha usado el buffer A
    }
}

bool encontrarPuntero(char *puntero) {
    if (puntero <= (buffer_A + TAM_MAX_BUFFER) && puntero >= buffer_A) { // Esta en buffer A
        return true;
    } else if (puntero <= (buffer_B + TAM_MAX_BUFFER) && puntero >= buffer_B) { // Esta en buffer B
        return false;
    }
}

char *formarLexema() {
    char *punteroAux = inicio, *nuevoLexema;
    int tamLexema = 0;
    if (encontrarPuntero(inicio) == encontrarPuntero(delantero)) { // Inicio y delantero estan en el mismo buffer
        tamLexema = delantero - inicio; // Se calcula el tam del lexema
        nuevoLexema = (char *) malloc((tamLexema) * sizeof(char)); // Se reserva memoria para la cadena
        for (int i = 0; i < tamLexema; i++) { // Se introducen caracteres hasta llegar al tam del lexema adecuado
            nuevoLexema[i] = *punteroAux;
            punteroAux++; // Se va avanzando el auxiliar que hace de inicio
        }
    } else { // Inicio y delantero estan en diferentes buffers -> puede ser que se haya excedido el tam max o no
        // Se reserva el tam maximo para el nuevo lexema considerando el peor de los casos
        nuevoLexema = (char *) malloc((TAM_MAX_BUFFER) * sizeof(char));
        if (overflowTamMax) { // Si se ha superado el tam maximo se atrasa el puntero delantero
            moverDelantero();
        }
        while (punteroAux != delantero && tamLexema <= (TAM_MAX_BUFFER)) {
            // Si el auxiliar no llega a delantero pero si a EOF, hay que cambiar de buffer
            if (*punteroAux != centinela) {
                nuevoLexema[tamLexema] = *punteroAux;
                punteroAux++;
                tamLexema++;
            } else {
                if (encontrarPuntero(inicio)) { // Si inicio estaba en el buffer A
                    punteroAux = buffer_B; // se carga el buffer B
                } else {  // Si inicio estaba en B
                    punteroAux = buffer_A;  // se carga A
                }
            }
        }
    }
    nuevoLexema = realloc(nuevoLexema, tamLexema + 1); // se aumenta en 1 el tam para colocar el fin de cadena
    nuevoLexema[tamLexema] = '\0';
    avanzar = true; // se indica a inicio que debe avanzar para leer un nuevo componente
    overflowTamMax = false;
    return nuevoLexema;
}

char sigCaracter(int numCaracteres) {
    // Se comprueba si el analizador lexico ya ha leido mas caracteres de los permitidos
    if (numCaracteres > TAM_MAX_BUFFER) {
        overflowTamMax = true;
        return OVERFLOW_MAXTAMLEX; // En caso de que si, le envia una constante para indicarlo
    }
    char caracter;
    if (isBufferA) {   // Esta cargado el buffer A
        if (*delantero == centinela) {  // Cuando delantero apunta al centinela
            if (delantero == (buffer_A + TAM_MAX_BUFFER)) {   // Cuando delantero esta en la ultima posicion del buffer
                // Se debe cambiar de buffer
                cargarBuffer();
            } else {    // Si delantero=EOF pero no esta en la ultima posicion del buffer es que se llego al fin de fichero
                return centinela; // Se devuelve EOF (End Of File) al Analizador Lexico
            }
        }
    } else {  // Esta cargado el buffer B
        if (*delantero == centinela) {
            if (delantero == (buffer_B + TAM_MAX_BUFFER)) {
                cargarBuffer();
            } else {
                return centinela;
            }
        }
    }
    caracter = *delantero;
    if (avanzar) {  // Si inicio debe avanzar, se avanza
        avanzarInicio();
        avanzar = false;
    }
    delantero++; // Se avanza una posicion a delantero
    return caracter;
}

void moverDelantero() {
    if (delantero != buffer_A && delantero != buffer_B) {
        delantero--; // Si el puntero no esta en la primera posicion de un buffer, simplemente se retrasa una posicion
    } else if (isBufferA) { // De lo contrario, estara en una primera posicion y si esta cargado el buffer A se mueve a la ultima del B
        delantero = buffer_B + TAM_MAX_BUFFER;
    } else if (!isBufferA) { // En caso de que este cargado el buffer B, se mueve a la ultima del A
        delantero = buffer_A + TAM_MAX_BUFFER;
    }
    avanzar = true; // se indica a inicio que debe avanzar
}

void avanzarInicio() {
    inicio = delantero;
}

void notificarAvanzarInicio() {
    avanzar = true;
}

void reiniciarLecturaComponente() {
    delantero = inicio;
}

void liberarSistemaEntrada() {
    free(buffer_A);
    free(buffer_B);
}