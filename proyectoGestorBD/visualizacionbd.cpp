#include "visualizacionbd.h"
#include "disenobdinicial.h"

FILE *fV[4]; // 0-BDFile 1-BDFile_Tablas 2-BDFile_Campos 3-BDFile_Registros
struct bloqHTB bVHTB;
struct bloqITB bVITB;
struct bloqHCM bVHCM;
struct bloqICM bVICM;

visualizacionbd::visualizacionbd()
{

}

void visualizacionbd::leerTablas(){
    fV[0]=fopen("BDFile.dat","rw+"); //Creo el archivo maestro de las BD.
    if(fV[0]==NULL){

    }
    fseek(fV[0],20,SEEK_SET);
    fread(&bVHTB,1,sizeof(bloqHTB),fV[0]);
    string tabDis[bVHTB.numTB];
    for(int k=0;k<bVHTB.numTB;k++){
        fread(&bVITB,1,sizeof(bloqITB),fV[0]);
        tabDis[k]=bVITB.nombTabla;
        cout << k <<". " << tabDis[k] << "\n";
    }
}
