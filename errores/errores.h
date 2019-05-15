#ifndef P1COMPILADORES_ERRORES_H
#define P1COMPILADORES_ERRORES_H

#define OVERFLOW_MAXTAMLEX 20
#define ID_MALFORMADO 21
#define STRING_MALFORMADO 22
#define COMENTARIO_MULTILINEA_MALFORMADO 23
#define COMPONENTE_NO_RECONOCIDO 24
#define NUMERO_MALFORMADO 25
#define FLOAT_MALFORMADO 26
#define HEXADECIMAL_MALFORMADO 27
#define ERROR_FICHERO 28

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/* Funcion que recibe un tipo de error e imprime sus datos por pantalla */
void imprimirError(int tipo, int linea, int columna);
/* Funcion que imprime un error causado por el fichero de entrada */
void imprimirErrorFichero(int tipo, char* mensaje);

#endif //P1COMPILADORES_ERRORES_H