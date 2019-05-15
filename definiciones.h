#ifndef P1COMPILADORES_DEFINICIONES_H
#define P1COMPILADORES_DEFINICIONES_H

/* Como el metodo empleado en el sistema de entrada es el del centinela, la memoria intermedia se divide en dos mitades
 * de n + 1 bytes (siendo el +1 el centinela), teniendo que ser n un multiplo de la unidad de asignacion. Es por esto
 * que el tam atribuido a cada buffer sea de 4096 bytes. */
#define TAM_MAX_BUFFER 4096

/* Palabras reservadas */
#define IMPORT 200
#define AS 201
#define DEF 202
#define FOR 203
#define IN 204
#define IF 205
#define ELIF 206
#define ELSE 207
#define RETURN 208
#define NOT 209
#define PRINT 210

/* Componentes lexicos */
#define IDENTIFICADOR 250
#define INTEGER 251
#define FLOAT 252
#define HEXADECIMAL 253
#define STRING 254
#define OP_IGUAL 255
#define OP_MENOS 256
#define OP_MAS 257
#define OP_PRODUCTO 258
#define OP_DIVISION 259
#define OP_MENOR 260
#define OP_MASIGUAL 261
#define OP_MAYOR 262
#define OP_POTENCIA 263
#define OP_MENORIGUAL 264
#define OP_IGUALIGUAL 265
#define COMENTARIO_MULTILINEA 266

// Arrays utilizados para inicializar la tabla de simbolos:
static char *palabrasReservadas[] = {"import", "as", "def", "for", "in", "if", "elif", "else", "return", "not",
                                     "print"};
static int componentesLexicos[] = {IMPORT, AS, DEF, FOR, IN, IF, ELIF, ELSE, RETURN, NOT, PRINT, -1};

// Arrays utilizados para identificar simbolos
static char simbolos[9] = {".{},()[]:"};
static int idSimbolos[9] = {'.', '{', '}', ',', '(', ')', '[', ']', ':'};
static char operadores[7] = {"=-+*/<>"};
static int idOperadores[7] = {OP_IGUAL, OP_MENOS, OP_MAS, OP_PRODUCTO, OP_DIVISION, OP_MENOR, OP_MAYOR};

#endif //P1COMPILADORES_DEFINICIONES_H