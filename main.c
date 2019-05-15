#include <stdio.h>
#include "stdlib.h"
#include "definiciones.h"
#include "analizadores/sintactico.h"
#include "entrada/sistemaEntrada.h"
#include "simbolos/tablaSimbolos.h"
#include "errores/errores.h"

int main(int argc, char **argv) {
    char *rutaFichero;
    if (argc == 2) {
        rutaFichero = argv[1];
    } else if (argc > 2) {
        imprimirErrorFichero(ERROR_FICHERO, "se han recibido mas argumentos de los esperados.\n");
    } else {
        imprimirErrorFichero(ERROR_FICHERO, "se espera un argumento.\n");
    }
    /* Inicializacion */
    abrirFichero(rutaFichero);
    //abrirFichero("../wilcoxon.py");
    inicializarSistemaEntrada();
    cargarBuffer();
    inicializarTablaSimbolos();

    /* Analisis */
    imprimirTablaSimbolos();
    sig_comp();
    imprimirTablaSimbolos();

    /* Finalizacion del programa */
    cerrarFichero();
    liberarTablaSimbolos();
    liberarSistemaEntrada();
    return 0;
}