#ifndef DISENOBDINICIAL_H
#define DISENOBDINICIAL_H

#include <cstring>
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;

struct superBlock{   // Struct para leer el Super Block.
    int bloqueLibre;   // Indicador del siguiente bloque libre.
    int tamBloques;    // Indicador del tamano de los bloques.
    int numBloques;    // Indicador del numero de bloques.
    int pBloqTablas;   // Primer bloque con las tablas de la BD
    int uBloqTablas;   // Ultimo bloque con las tablas de la BD
};

struct bloqHTB{    // Struct para leer la metadata de los bloques de tipo Tabla
    int numTB;       // Numero de Tablas en el Bloque
    int bloqSigTB;   // Siguiente Bloque con informacion de Tablas
    int bloqAntTB;   // Bloque Anterior con informacion de Tablas
};

struct bloqITB{         // Struct para leer los registros de tipo Tabla
    int numCampos;        // Numero de Campos dentro de la Tabla
    int pBloqCampos;      // Primer Bloque con los campos de la Tabla
    int uBloqCampos;      // Ultimo Bloque con los campos de la Tabla
    int bloqHashValues;   // Bloque donde se guarda el registro del primer bloque de cada Hash value.
    char nombTabla[20];   // Nombre de la Tabla
};

struct bloqHHT{   // Struct para leer la metadata de los bloques de tipo Hash
    int numHV;    // Numero de Hash Values en el bloque
    int bloqTB;   // Numero del bloque de TB que pertenecen los Valores Hash
};

struct bloqIHT{         // Struct para leer los registros de tipo Hash
    int hashValue;        // Numero de Campos dentro de la Tabla
    int pBloqRegistros;   // Primer Bloque con los Registros de la Tabla
    int uBloqRegistros;   // Ultimo Bloque con los Registros de la Tabla
};

struct bloqHCM{         // Struct para leer la metadata de los bloques de tipo Campo
    int numCM;          // Numero de Campos en el Bloque
    int bloqTB;           // Numero del bloque de TB que pertenecen los Campos
    int bloqSigCM;        // Siguiente Bloque con informacion de Campos
    int bloqAntCM;        // Bloque Anterior con informacion de Campos
};

struct bloqICM{         // Struct para leer los registros de tipo Campo
    char nombCampo[20];   // Nombre del Campo
    char tipoCampo[3];    // Tipo de Campo
};

struct bloqHRG{    // Struct para leer la metadata de los bloques de tipo BD
    int numRG;       // Numero de Registros en el bloque
    int bloqTB;      // Numero del Bloque de Campos que pertenecen los Registros
    int bloqSigRG;   // Bloque Siguiente con Registros
    int bloqAntRG;   // Bloque Anterior con Registros
};

struct bloqIRG{   // Struct para leer la metadata de los bloques de tipo BD
    char tipoReg[3];     // Tamano del Registros
    char *registro;   // Registros de una Tabla Separados por '-'
};

class disenobdinicial
{
public:
    disenobdinicial();
    int crearBaseDatos();
    int formatearBaseDatos();
    int getBloqueLibre();
    int validateBloqueLibre();
    int funcionHash(char codigo[]);

};

#endif // DISENOBDINICIAL_H
