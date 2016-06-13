#include "disenobdinicial.h"
#include "disenomenu.h"
#include "mantenimientobd.h"
#include <iostream>
#include <fstream>
#include <math.h>

disenobdinicial varDBI;
mantenimientobd varMBD;
disenomenu varDM;

int main(){
    varDBI.crearBaseDatos();
    varDBI.formatearBaseDatos();
    //varDM.menuPrincipal();

    varMBD.crearTabla("Kevin1              ",10);
    varMBD.crearTabla("Kevin2              ",10);
    varMBD.crearTabla("Kevin3              ",10);
    varMBD.crearTabla("Kevin4              ",10);
    varMBD.crearTabla("Kevin5              ",10);
    varMBD.crearTabla("Kevin6              ",10);
    varMBD.crearTabla("Kevin7              ",10);
    varMBD.crearTabla("Kevin8              ",10);
    varMBD.crearTabla("Kevin9              ",10);
    varMBD.crearTabla("Kevin10             ",10);
    varMBD.crearTabla("Kevin11             ",10);
    varMBD.crearTabla("Kevin12             ",10);
    varMBD.crearTabla("Kevin13             ",10);
    varMBD.crearTabla("Kevin14             ",10);
    varMBD.crearTabla("Kevin15             ",10);
    varMBD.crearTabla("Kevin16             ",10);

    varMBD.crearCampo("Hi1                  ","STR","Kevin20             ");
    varMBD.crearCampo("Hi2                  ","STR","Kevin15             ");
    varMBD.crearCampo("Hi3                  ","STR","Kevin15             ");
    varMBD.crearCampo("Hi4                  ","STR","Kevin15             ");
    varMBD.crearCampo("Hi5                  ","STR","Kevin15             ");
    varMBD.crearCampo("Hi6                  ","STR","Kevin15             ");
    varMBD.crearCampo("Hi7                  ","STR","Kevin15             ");
    varMBD.crearCampo("Hi8                  ","STR","Kevin15             ");
    varMBD.crearCampo("Hi9                  ","STR","Kevin15             ");
    varMBD.crearCampo("Hi10                 ","STR","Kevin15             ");
    varMBD.crearCampo("Hi11                 ","STR","Kevin15             ");


    return 0;
}
