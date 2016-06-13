#ifndef VISUALIZACIONBD_H
#define VISUALIZACIONBD_H
#include "disenobdinicial.h"

class visualizacionbd
{
public:
    visualizacionbd();

    void leerTablas();
    int leerCampos(string nomTabla);
    int leerRegistros(string nomTabla);

};

#endif // VISUALIZACIONBD_H
