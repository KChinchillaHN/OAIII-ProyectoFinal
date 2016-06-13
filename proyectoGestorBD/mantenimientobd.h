#ifndef MANTENIMIENTOBD_H
#define MANTENIMIENTOBD_H
#include "disenobdinicial.h"

struct bloqPosTB{
    int numBloq;
    int posBloq;
};

class mantenimientobd
{
public:
    mantenimientobd();
    int bloqDisponibles();
    void abrirArchivo();
    void cerrarArchivo();
    int numTablas();
    int getBloqCampos(string tabla);

    int createBloqHV(int bloqTB);
    int createBloqCM(int bloqTB);
    int getUBloqTabla();
    int getPBloqTabla();
    int valEspBloqTab(int uBloqTB);
    void writeBloqTB(string nomTB, int numCM, int uBloqTB);
    void writeNBloqTB(string nomTB, int numCM, int uBloqTB);
    int crearTabla(string nomTabla, int numCampos);

    int getBloqTB(string nomTabla);
    int valNumCM(string nomTabla);
    int getnNumCM(string nomTabla);
    int crearCampo(string nomCampo, string tipoCampo, string nomTabla);
    int valEspBloqCM(int numBloq);
    int writeBloqCM (string nomCampo, string tipoCampo, int bloqCM);
    int writeBloqNCM(string nomCampo, string tipoCampo, int bloqCM, int bloqTB, int nBloqTB);
    void imprimirTablas(string tablas[]);

    int espTotal(string tipoReg[], int numReg);
    int crearRegistro(string nomTabla, string tipoReg[], string registro[], int numReg);
    int valEspBloqReg(int numBloq, int espTot);
    void writeBloqRG (string tipoReg[], string registro[], int numReg, int bloqReg, int espTot);
    void writeBloqNRG(string tipoReg[], string registro[], int numReg, int bloqReg, int espTot);
    void imprimirCampos(int bloqCM, string campos[], string tipoCM[]);

};

#endif // MANTENIMIENTOBD_H
