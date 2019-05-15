#ifndef P1COMPILADORES_SISTEMAENTRADA_H
#define P1COMPILADORES_SISTEMAENTRADA_H

#include "stdbool.h"

/* Funcion para abrir el fichero con el codigo a analizar */
void abrirFichero(char* ruta);
/* Funcion para cerrar el fichero con el codigo a analizar */
int cerrarFichero();
/* Funcion donde se inicializa el doble buffer */
void inicializarSistemaEntrada();
/* Funcion que carga el buffer correspondiente */
void cargarBuffer();
/* Funcion que proporciona los caracteres del codigo a analizar uno a uno */
char sigCaracter(int numCaracteres);
/* Funcion que indica en que buffer esta el puntero indicado (inicio o delantero) */
bool encontrarPuntero(char *puntero); // true-> bufferA false -> bufferB
/* Funcion que construye lexemas con la referencia de los punteros inicio y delantero */
char* formarLexema();
/* Funcion que mueve el puntero delantero una posicion hacia atras. Se emplea cuando un automata llega a un estado de aceptacion,
 * es decir, cuando se identifica un componente.*/
void moverDelantero();
/* Funcion que avanza el puntero de inicio a la posicion de delantero para iniciar la lectura de un nuevo componente */
void avanzarInicio();
/* Funcion que da valor "true" al booleano "avanzar" utilizado para avanzar el puntero de inicio a la posicion de delantero*/
void notificarAvanzarInicio();
/* Funcion que reinicia el analisis de un componente (se usa cuando se comenzo a analizar un lexema como un tipo de componente
 * cuando en realidad era de otro tipo. Ejemplo: analizar un lexema como entero cuando en realidad es un float). */
void reiniciarLecturaComponente();
/* Funcion que libera toda la memoria utilizada en el sistema de entrada */
void liberarSistemaEntrada();

#endif //P1COMPILADORES_SISTEMAENTRADA_H