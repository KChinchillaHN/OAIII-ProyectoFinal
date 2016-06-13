#ifndef DISENOMENU_H
#define DISENOMENU_H
#include "mantenimientobd.h"
#include "visualizacionbd.h"

class disenomenu
{
public:
    disenomenu();

    void menuPrincipal(); // Imprime el menu principal.
    void menuVisualizacionBD(); // Imprime el menu de Visualizacion de Base de Datos.
    void menumantenimientoBD(); // Imprime el menu de Mantenimiento de Base de Datos.
    void menuTablas(); // Imprime el menu de Tablas.

    void formularioTablas();
    void formularioCampos();
    void formularioRegistros();

    void visualizarTablas();

};

#endif // DISENOMENU_H
