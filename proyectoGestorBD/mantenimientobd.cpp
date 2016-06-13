#include "mantenimientobd.h"
#include "disenobdinicial.h"

disenobdinicial varDBIM;
FILE *fM;
struct superBlock bMSPB;
struct bloqHTB bMHTB;
struct bloqITB bMITB;
struct bloqHCM bMHCM;
struct bloqICM bMICM;
struct bloqHHT bMHHT;
struct bloqIHT bMIHT;
struct bloqHRG bMHRG;
struct bloqIRG bMIRG;

int tBloqueM=512,
    nBloqueM=8192;

mantenimientobd::mantenimientobd(){

}

int mantenimientobd::bloqDisponibles(){
    int bloq=0;
    rewind(fM);
    fscanf(fM,"%i",&bloq);
    return nBloqueM - (bloq - 1);
}

void mantenimientobd::abrirArchivo(){
    fM=fopen("BDFile.dat","rwb+");
    if(fM==NULL){
        cout << "Error al abrir el archivo" << endl;
    }
}

int mantenimientobd::getBloqCampos(string tabla){
    int bloqTB, numCM, pos;
    abrirArchivo();
    bloqTB=getBloqTB(tabla);
    pos = 20+(bloqTB*tBloqueM);
    fseek(fM,pos,SEEK_SET);
    fread(&bMHTB,sizeof(bloqHTB),1,fM);
    for(int k=0;k<bMHTB.numTB;k++){
        fread(&bMITB,sizeof(bloqITB),1,fM);
        if((memcmp(bMITB.nombTabla,tabla.c_str(),20))==0){
            numCM=bMITB.pBloqCampos;
        }
    }
    cerrarArchivo();
    return numCM;
}

void mantenimientobd::imprimirCampos(int bloqCM, string campos[], string tipoCM[]){
    int pos, numCM=0;
    abrirArchivo();
    pos = 20+(bloqCM*tBloqueM);
    fseek(fM,pos,SEEK_SET);
    fread(&bMHCM,sizeof(bloqHCM),1,fM);
    for (int k=0;k<bMHCM.numCM;k++){
        fread(&bMICM,sizeof(bloqICM),1,fM);
        campos[numCM] = bMICM.nombCampo;
        tipoCM[numCM] = bMICM.tipoCampo;
        numCM++;
    }
    cerrarArchivo();
}

int mantenimientobd::numTablas(){
    int pBloqTB=0, pos, bloque, numTB=0;
    abrirArchivo();
    pos = 20+(pBloqTB*tBloqueM);
    fseek(fM,pos,SEEK_SET);
    fread(&bMHTB,sizeof(bloqHTB),1,fM);
    bloque=pBloqTB;
    if(bMHTB.bloqSigTB==-1){
        cerrarArchivo();
        return bMHTB.numTB;
    }
    else{
        while(bloque!=-1){
            pos = 20+(bloque*tBloqueM);
            fseek(fM,pos,SEEK_SET);
            fread(&bMHTB,sizeof(bloqHTB),1,fM);
            numTB+=bMHTB.numTB;
            bloque=bMHTB.bloqSigTB;
        }
        cerrarArchivo();
        return numTB;
    }

}

void mantenimientobd::imprimirTablas(string tablas[]){
    int pBloqTB=0, pos, bloque, numTB=0;
    abrirArchivo();
    pos = 20+(pBloqTB*tBloqueM);
    fseek(fM,pos,SEEK_SET);
    fread(&bMHTB,sizeof(bloqHTB),1,fM);
    bloque=pBloqTB;
    if(bMHTB.bloqSigTB==-1){
        for(int k=0;k<bMHTB.numTB;k++){
            fread(&bMITB,sizeof(bloqITB),1,fM);
            tablas[k]=bMITB.nombTabla;
        }
    }
    else{
        while(bloque!=-1){
            pos = 20+(bloque*tBloqueM);
            fseek(fM,pos,SEEK_SET);
            fread(&bMHTB,sizeof(bloqHTB),1,fM);
            for(int k=0;k<bMHTB.numTB;k++){
                fread(&bMITB,sizeof(bloqITB),1,fM);
                tablas[numTB]=bMITB.nombTabla;
                numTB++;
            }
            bloque=bMHTB.bloqSigTB;
        }
    }
}

void mantenimientobd::cerrarArchivo(){
    fclose(fM);
}

// - - - Creacion de un registro de Tabla - - - //
int mantenimientobd::createBloqCM(int bloqTB){
    int bloqL, pos;
    bloqL = varDBIM.getBloqueLibre();
    pos = 20+(bloqL*tBloqueM);
    fseek(fM,pos,SEEK_SET);
    bMHCM.bloqAntCM=-1;
    bMHCM.bloqSigCM=-1;
    bMHCM.bloqTB=bloqTB;
    bMHCM.numCM=0;
    fwrite(&bMHCM,sizeof(bloqHCM),1,fM);
    return bloqL;
}

int mantenimientobd::createBloqHV(int bloqTB){
    int bloqL, pos, bloqHV;
    bloqL = varDBIM.getBloqueLibre();
    pos = 20+(bloqL*tBloqueM);
    fseek(fM,pos,SEEK_SET);
    bMHHT.bloqTB=bloqTB;
    bMHHT.numHV=10;
    fwrite(&bMHHT,sizeof(bloqHHT),1,fM);
    bloqHV=bloqL;
    for(int k=0;k<10;k++){
        bloqL = varDBIM.getBloqueLibre();
        pos = 20+(bloqHV*tBloqueM)+8+(k*12);
        fseek(fM,pos,SEEK_SET);
        bMIHT.hashValue=k;
        bMIHT.pBloqRegistros=bloqL;
        bMIHT.uBloqRegistros=bloqL;
        fwrite(&bMIHT,sizeof(bloqIHT),1,fM);
        // Crear Bloques de Registros
        pos = 20+(bloqL*tBloqueM);
        fseek(fM,pos,SEEK_SET);
        bMHRG.bloqTB=bloqTB;
        bMHRG.bloqAntRG=-1;
        bMHRG.bloqSigRG=-1;
        bMHRG.numRG=0;
        fwrite(&bMHRG,sizeof(bloqHRG),1,fM);
    }
    return bloqL;
}

int mantenimientobd::getUBloqTabla(){
    rewind(fM);
    fread(&bMSPB,sizeof(superBlock),1,fM);
    return bMSPB.uBloqTablas;
}

int mantenimientobd::getPBloqTabla(){
    rewind(fM);
    fread(&bMSPB,sizeof(superBlock),1,fM);
    return bMSPB.pBloqTablas;
}

int mantenimientobd::valEspBloqTab(int uBloqTB){
    int numTB, pos, heaTB=12, infTB=36, espSobrante;
    pos=20+(uBloqTB*tBloqueM);
    fseek(fM,pos,SEEK_SET);
    fread(&bMHTB,sizeof(bloqHTB),1,fM);
    numTB=bMHTB.numTB;
    espSobrante=tBloqueM-(heaTB+(numTB*infTB));
    if(espSobrante>=infTB){
        return 0;  // Espacio Suficiente para continuar escribiendo en el bloque.
    }
    else{
        return -1; // Espacio Insuficiente debera de crear un nuevo bloque.
    }
}

void mantenimientobd::writeBloqTB(string nomTB, int numCM, int uBloqTB){
    int pos, heaTB=12, infTB=36;
    pos=20+(uBloqTB*tBloqueM);
    fseek(fM,pos,SEEK_SET);
    fread(&bMHTB,sizeof(bloqHTB),1,fM);
    bMHTB.bloqAntTB=bMHTB.bloqAntTB;
    bMHTB.bloqSigTB=bMHTB.bloqSigTB;
    bMHTB.numTB+=1;
    fseek(fM,pos,SEEK_SET);
    fwrite(&bMHTB,sizeof(bloqHTB),1,fM);
    bMITB.pBloqCampos=createBloqCM(uBloqTB);
    bMITB.bloqHashValues=createBloqHV(uBloqTB);
    bMITB.uBloqCampos=-1;
    strcpy(bMITB.nombTabla,nomTB.c_str());
    bMITB.numCampos=numCM;
    pos+= heaTB+((bMHTB.numTB-1)*infTB);
    fseek(fM,pos,SEEK_SET);
    fwrite(&bMITB,sizeof(bloqITB),1,fM);
    cerrarArchivo();
}

void mantenimientobd::writeNBloqTB(string nomTB, int numCM, int uBloqTB){
    int pos, bloqL;
    bloqL = varDBIM.getBloqueLibre();
    // Actualizar Ultimo Bloque de Tablas del SuperBlock
    rewind(fM);
    fread(&bMSPB,sizeof(superBlock),1,fM);
    bMSPB.bloqueLibre=bMSPB.bloqueLibre;
    bMSPB.numBloques=bMSPB.numBloques;
    bMSPB.pBloqTablas=bMSPB.pBloqTablas;
    bMSPB.tamBloques=bMSPB.tamBloques;
    bMSPB.uBloqTablas=bloqL;
    rewind(fM);
    fwrite(&bMSPB,sizeof(superBlock),1,fM);
    // Actualizar el campo sigBloqTB del bloque.
    pos=20+(uBloqTB*tBloqueM);
    fseek(fM,pos,SEEK_SET);
    fread(&bMHTB,sizeof(bloqHTB),1,fM);
    bMHTB.bloqAntTB = bMHTB.bloqAntTB;
    bMHTB.bloqSigTB = bloqL;
    bMHTB.numTB = bMHTB.numTB;
    fseek(fM,pos,SEEK_SET);
    fwrite(&bMHTB,sizeof(bloqHTB),1,fM);
    // Creacion del Bloque Nuevo
    pos=20+(bloqL*tBloqueM);
    fseek(fM,pos,SEEK_SET);
    bMHTB.bloqAntTB=uBloqTB;
    bMHTB.bloqSigTB=-1;
    bMHTB.numTB=1;
    fwrite(&bMHTB,sizeof(bloqHTB),1,fM);
    bMITB.pBloqCampos=createBloqCM(uBloqTB);
    bMITB.bloqHashValues=createBloqHV(uBloqTB);
    bMITB.uBloqCampos=-1;
    strcpy(bMITB.nombTabla,nomTB.c_str());
    bMITB.numCampos=numCM;
    pos=20+(bloqL*tBloqueM)+12;
    fseek(fM,pos,SEEK_SET);
    fwrite(&bMITB,sizeof(bloqITB),1,fM);
    cerrarArchivo();
}

int mantenimientobd::crearTabla(string nomTabla, int numCampos){
    int uBloqTabla, espBloq, bloqDis;
    abrirArchivo();
    bloqDis=bloqDisponibles();
    if(bloqDis > 13){ // 13 = 1 Bloque Tablas + 1 Bloque Campos + 1 Bloque Hash Table + 10 Bloque Registros
        uBloqTabla=getUBloqTabla();
        espBloq=valEspBloqTab(uBloqTabla);
        if(espBloq==0){
            writeBloqTB(nomTabla, numCampos, uBloqTabla);
            return 0;
        }
        else{
            writeNBloqTB(nomTabla, numCampos, uBloqTabla);
            return 0;
        }
     }
     else{
         return -1; // Espacio insuficiente para crear una Tabla.
     }
}

// - - - Creacion de un registro de Campo - - - //
int mantenimientobd::getBloqTB(string nomTabla){
    int pBloqTB=0, pos, bloque;
    pos = 20+(pBloqTB*tBloqueM);
    fseek(fM,pos,SEEK_SET);
    fread(&bMHTB,sizeof(bloqHTB),1,fM);
    bloque=pBloqTB;
    if(bMHTB.bloqSigTB==-1){
        return pBloqTB;
    }
    else{
        while(bloque!=-1){
            pos = 20+(bloque*tBloqueM);
            fseek(fM,pos,SEEK_SET);
            fread(&bMHTB,sizeof(bloqHTB),1,fM);
            for(int k=0; k<bMHTB.numTB; k++){
                fread(&bMITB,sizeof(bloqITB),1,fM);
                if((memcmp(bMITB.nombTabla,nomTabla.c_str(),20))==0){
                    return bloque;
                }
            }
            bloque=bMHTB.bloqSigTB;
        }
    }
    return -1;
}

int mantenimientobd::valEspBloqCM(int numBloq){
    int numCM, pos, heaCM=16, infCM=23, espSobrante;
    pos=20+(numBloq*tBloqueM);
    fseek(fM,pos,SEEK_SET);
    fread(&bMHCM,sizeof(bloqHCM),1,fM);
    numCM=bMHCM.numCM;
    espSobrante=tBloqueM-(heaCM+(numCM*infCM));
    if(espSobrante>=infCM){
        return 0;  // Espacio Suficiente para continuar escribiendo en el bloque.
    }
    else{
        return -1; // Espacio Insuficiente debera de crear un nuevo bloque.
    }
}

int mantenimientobd::writeBloqCM(string nomCampo, string tipoCampo, int bloqCM){
    int pos, heaCM=16, infCM=23;
    pos=20+(bloqCM*tBloqueM);
    fseek(fM,pos,SEEK_SET);
    fread(&bMHCM,sizeof(bloqHCM),1,fM);
    bMHCM.bloqAntCM=bMHCM.bloqAntCM;
    bMHCM.bloqSigCM=bMHCM.bloqSigCM;
    bMHCM.bloqTB=bMHCM.bloqTB;
    bMHCM.numCM+=1;
    pos=20+(bloqCM*tBloqueM);
    fseek(fM,pos,SEEK_SET);
    fwrite(&bMHCM,sizeof(bloqHCM),1,fM);
    strcpy(bMICM.nombCampo,nomCampo.c_str());
    strcpy(bMICM.tipoCampo,tipoCampo.c_str());
    pos+= heaCM+((bMHCM.numCM-1)*infCM);
    fseek(fM,pos,SEEK_SET);
    fwrite(&bMICM,sizeof(bloqICM),1,fM);
    cerrarArchivo();
}

int mantenimientobd::writeBloqNCM(string nomCampo, string tipoCampo, int bloqCM, int bloqTB, int nBloqTB){
    int pos, bloqL,heaTB=12, infTB=36;
    bloqL = varDBIM.getBloqueLibre();
    // Actualizar el campo uBloqCampos del bloque.
    pos=20+(bloqTB*tBloqueM)+heaTB+(infTB*nBloqTB);
    fseek(fM,pos,SEEK_SET);
    fread(&bMITB,sizeof(bloqITB),1,fM);
    bMITB.bloqHashValues=bMITB.bloqHashValues;
    strcpy(bMITB.nombTabla,bMITB.nombTabla);
    bMITB.numCampos=bMITB.numCampos;
    bMITB.pBloqCampos=bMITB.pBloqCampos;
    bMITB.uBloqCampos=bloqL;
    fseek(fM,pos,SEEK_SET);
    fwrite(&bMITB,sizeof(bloqITB),1,fM);
    // Actualizar el campo sigBloqCM del bloque.
    pos=20+(bloqCM*tBloqueM);
    fseek(fM,pos,SEEK_SET);
    fread(&bMHCM,sizeof(bloqHCM),1,fM);
    bMHCM.bloqAntCM=bMHCM.bloqAntCM;
    bMHCM.bloqSigCM=bloqL;
    bMHCM.bloqTB=bMHCM.bloqTB;
    bMHCM.numCM=bMHCM.numCM;
    fseek(fM,pos,SEEK_SET);
    fwrite(&bMHCM,sizeof(bloqHCM),1,fM);
    // Creacion del Bloque Nuevo
    pos=20+(bloqL*tBloqueM);
    fseek(fM,pos,SEEK_SET);
    bMHCM.bloqAntCM=bloqCM;
    bMHCM.bloqSigCM=-1;
    bMHCM.bloqTB;
    bMHCM.numCM=1;
    fwrite(&bMHCM,sizeof(bloqHCM),1,fM);
    strcpy(bMICM.nombCampo,nomCampo.c_str());
    strcpy(bMICM.tipoCampo,tipoCampo.c_str());
    fwrite(&bMICM,sizeof(bloqICM),1,fM);
    cerrarArchivo();
}

int mantenimientobd::getnNumCM(string nomTabla){
    int bloqTB, pos, numCM;
    abrirArchivo();
    bloqTB = getBloqTB(nomTabla);
    pos = 20+(bloqTB*tBloqueM);
    fseek(fM,pos,SEEK_SET);
    fread(&bMHTB,sizeof(bloqHTB),1,fM);
    for (int k=0; k<bMHTB.numTB;k++){
        fread(&bMITB,sizeof(bloqITB),1,fM);
        if((memcmp(bMITB.nombTabla,nomTabla.c_str(),20))==0){
            numCM=bMITB.numCampos;
        }
    }
    cerrarArchivo();
    return numCM;
}

int mantenimientobd::valNumCM(string nomTabla){
    int bloqTB, bloqCM, pos, numCM;
    bloqTB = getBloqTB(nomTabla);
    pos = 20+(bloqTB*tBloqueM);
    fseek(fM,pos,SEEK_SET);
    fread(&bMHTB,sizeof(bloqHTB),1,fM);
    for (int k=0; k<bMHTB.numTB;k++){
        fread(&bMITB,sizeof(bloqITB),1,fM);
        if((memcmp(bMITB.nombTabla,nomTabla.c_str(),20))==0){
            numCM=bMITB.numCampos;
            if(bMITB.uBloqCampos==-1){
                bloqCM=bMITB.pBloqCampos;
           }
            else{
                bloqCM=bMITB.uBloqCampos;
            }
        }
    }
    pos = 20+(bloqCM*tBloqueM);
    fseek(fM,pos,SEEK_SET);
    fread(&bMHCM,sizeof(bloqHCM),1,fM);
    if(numCM>bMHCM.numCM){
        return 0; // Campos disponibles
    }
    else{
        return -1; // No hay mas campos disponibles
    }
}

int mantenimientobd::crearCampo(string nomCampo, string tipoCampo, string nomTabla){
    int bloqTB, espBloq, bloqCM, pos, nBloqTB, numCMDisp;
    abrirArchivo();
    bloqTB = getBloqTB(nomTabla);
    if(bloqTB==-1){
        return -2;
    }
    numCMDisp=valNumCM(nomTabla);
    if(numCMDisp==0){
        bloqTB = getBloqTB(nomTabla);
        pos = 20+(bloqTB*tBloqueM);
        fseek(fM,pos,SEEK_SET);
        fread(&bMHTB,sizeof(bloqHTB),1,fM);
        for (int k=0; k<bMHTB.numTB;k++){
            fread(&bMITB,sizeof(bloqITB),1,fM);
            if((memcmp(bMITB.nombTabla,nomTabla.c_str(),20))==0){
                if(bMITB.uBloqCampos==-1){
                    bloqCM=bMITB.pBloqCampos;
                    nBloqTB=k;
                }
                else{
                    bloqCM=bMITB.uBloqCampos;
                }
             }
        }
        espBloq = valEspBloqCM(bloqCM);
        if(espBloq==0){
            writeBloqCM(nomCampo, tipoCampo, bloqCM);
            return 0;
        }
        else{
            writeBloqNCM(nomCampo, tipoCampo, bloqCM, bloqTB, nBloqTB);
            return 0;
        }
    }
    else{
        return -1;
    }
}

// - - - Creacion de un registro en una Tabla - - - //
int mantenimientobd::espTotal(string tipoReg[], int numReg){
    int total=0;
    for (int k=0; k<numReg; k++){
        if (strcmp(tipoReg[k].c_str(),"STR")){
            total+=4+20; // 4 = STR_ + 20 = Tamano maximo de reg STR
        }
        else if (strcmp(tipoReg[k].c_str(),"CHR")){
            total+=4+1; // 4 = CHR_ + 1 = Tamano maximo de reg CHR
        }
        else if (strcmp(tipoReg[k].c_str(),"INT")){
            total+=4+4; // 4 = STR_ + 4 = Tamano maximo de reg INT
        }
        else{
            total+=4+5; // 4 = STR_ + 5 = Tamano maximo de reg KEY
        }
    }
    return total;
}

int mantenimientobd::valEspBloqReg(int numBloq, int espTot){
    int numRG, pos, heaRG=16, infRG=espTot, espSobrante;
    pos=20+(numBloq*tBloqueM);
    fseek(fM,pos,SEEK_SET);
    fread(&bMHRG,sizeof(bloqHRG),1,fM);
    numRG=bMHRG.numRG;
    espSobrante=tBloqueM-(heaRG+(numRG*infRG));
    if(espSobrante>=infRG){
        return 0;  // Espacio Suficiente para continuar escribiendo en el bloque.
    }
    else{
        return -1; // Espacio Insuficiente debera de crear un nuevo bloque.
    }
}

void mantenimientobd::writeBloqRG(string tipoReg[], string registro[], int numReg, int bloqReg, int espTot){
    int pos, heaRG=16, infRG=espTot;
    char regSTR[20];
    pos=20+(bloqReg*tBloqueM);
    fseek(fM,pos,SEEK_SET);
    fread(&bMHRG,sizeof(bloqHRG),1,fM);
    bMHRG.bloqAntRG=bMHRG.bloqAntRG;
    bMHRG.bloqSigRG=bMHRG.bloqSigRG;
    bMHRG.bloqTB=bMHRG.bloqTB;
    bMHRG.numRG+=1;
    fseek(fM,pos,SEEK_SET);
    fwrite(&bMHRG,sizeof(bloqHRG),1,fM);
    pos+= heaRG+((bMHRG.numRG-1)*infRG);
    fseek(fM,pos,SEEK_SET);
    for(int k=0;k<numReg;k++){
        if((memcmp(tipoReg[k].c_str(),"STR",3))==0){
            fwrite("STR_",4,1,fM);
            memcpy(regSTR,registro[k].c_str(),20);
            fwrite(regSTR,20,1,fM);
        }
        else if((memcmp(tipoReg[k].c_str(),"CHR",3))==0){
            fwrite("CHR_",4,1,fM);
            char CHRTemp[1];
            strcpy(CHRTemp,registro[k].c_str());
            fwrite(CHRTemp,1,1,fM);
        }
        else if((memcmp(tipoReg[k].c_str(),"INT",3))==0){
            fwrite("INT_",4,1,fM);
            int INTtemp;
            INTtemp=atoi(registro[k].c_str());
            fwrite(&INTtemp,4,1,fM);
        }
        else{
            fwrite("KEY_",4,1,fM);
            char KEYTemp[1];
            strcpy(KEYTemp,registro[k].c_str());
            fwrite(KEYTemp,5,1,fM);
        }
    }
    cerrarArchivo();
}

void mantenimientobd::writeBloqNRG(string tipoReg[], string registro[], int numReg, int bloqReg, int espTot){
    int pos, heaRG=16, infRG=espTot, bloqL;
    char regSTR[20];
    bloqL=varDBIM.getBloqueLibre();
    // Actualizar Bloq Siguiente
    pos=20+(bloqReg*tBloqueM);
    fseek(fM,pos,SEEK_SET);
    fread(&bMHRG,sizeof(bloqHRG),1,fM);
    bMHRG.bloqAntRG=bMHRG.bloqAntRG;
    bMHRG.bloqSigRG=bloqL;
    bMHRG.bloqTB=bMHRG.bloqTB;
    bMHRG.numRG=bMHRG.numRG;
    fseek(fM,pos,SEEK_SET);
    fwrite(&bMHRG,sizeof(bloqHRG),1,fM);
    // Crear Nuevo Bloq Registro - Header
    pos=20+(bloqL*tBloqueM) ;
    fseek(fM,pos,SEEK_SET);
    bMHRG.bloqAntRG=bloqReg;
    bMHRG.bloqSigRG=-1;
    bMHRG.bloqTB=bMHRG.bloqTB;
    bMHRG.numRG=1;
    fwrite(&bMHRG,sizeof(bloqHRG),1,fM);
    // Crear Nuevo Bloq Registro - Informacion
    for(int k=0;k<numReg;k++){
        if((memcmp(tipoReg[k].c_str(),"STR",3))==0){
            fwrite("STR_",4,1,fM);
            memcpy(regSTR,registro[k].c_str(),20);
            fwrite(regSTR,20,1,fM);
        }
        else if((memcmp(tipoReg[k].c_str(),"CHR",3))==0){
            fwrite("CHR_",4,1,fM);
            char CHRTemp[1];
            strcpy(CHRTemp,registro[k].c_str());
            fwrite(CHRTemp,1,1,fM);
        }
        else if((memcmp(tipoReg[k].c_str(),"INT",3))==0){
            fwrite("INT_",4,1,fM);
            int INTtemp;
            INTtemp=atoi(registro[k].c_str());
            fwrite(&INTtemp,4,1,fM);
        }
        else{
            fwrite("KEY_",4,1,fM);
            char KEYTemp[1];
            strcpy(KEYTemp,registro[k].c_str());
            fwrite(KEYTemp,5,1,fM);
        }
    }
    cerrarArchivo();
}

int mantenimientobd::crearRegistro(string nomTabla, string tipoReg[], string registro[], int numReg){
    int pos, bloqTB, espTot, bloqHV, valHash, bloqReg, valEsp;
    char keyValue[5];
    strcpy(keyValue,tipoReg[1].c_str()); // Registro[1] = KEY Value
    abrirArchivo();
    bloqTB = getBloqTB(nomTabla);
    espTot = espTotal(tipoReg, numReg);
    pos = 20+(bloqTB*tBloqueM);
    fseek(fM,pos,SEEK_SET);
    fread(&bMHTB,sizeof(bloqHTB),1,fM);
    for (int k=0; k<bMHTB.numTB;k++){
        fread(&bMITB,sizeof(bloqITB),1,fM);
        if((memcmp(bMITB.nombTabla,nomTabla.c_str(),19))==0){
            bloqHV=bMITB.bloqHashValues;
        }
    }
    pos = 20+(bloqHV*tBloqueM);
    fseek(fM,pos,SEEK_SET);
    valHash=varDBIM.funcionHash(keyValue);
    pos = 20+(bloqHV*tBloqueM)+8+(12*valHash);
    fseek(fM,pos,SEEK_SET);
    fread(&bMIHT,sizeof(bloqIHT),1,fM);
    if(bMIHT.uBloqRegistros==-1){
        bloqReg=bMIHT.pBloqRegistros;
    }
    else{
        bloqReg=bMIHT.uBloqRegistros;
    }
    valEsp=valEspBloqReg(bloqReg, espTot);
    if(valEsp==0){
        writeBloqRG(tipoReg, registro, numReg, bloqReg, espTot);
    }
    else{
        writeBloqNRG(tipoReg, registro, numReg, bloqReg, espTot);
    }
    return 0;
}

