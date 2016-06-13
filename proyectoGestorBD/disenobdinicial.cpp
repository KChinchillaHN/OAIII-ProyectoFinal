#include "disenobdinicial.h"

FILE *f[4]; // 0-BDFile 1-BDFile_Tablas 2-BDFile_Campos 3-BDFile_Registros

int bloqLibre=0,
    block_size=512,
    block_count=8192,
    pos=0;

struct bloqHTB bHTB;
struct bloqITB bITB;
struct bloqHCM bHCM;
struct bloqICM bICM;
struct bloqHRG bHRG;
struct bloqIRG bIRG;
struct bloqHHT bHHT;
struct bloqIHT bIHT;

disenobdinicial::disenobdinicial(){

}

int disenobdinicial::funcionHash(char codigo[]){
    int sumHashValue=0, hashValue;
    for(int k=0;k<5;k++){
        sumHashValue+=(int) codigo[k];
    }
    hashValue=sumHashValue%10;
    return hashValue;
}

int disenobdinicial:: getBloqueLibre(){
    int returnBloque;
    rewind(f[0]);
    fscanf(f[0],"%i",&bloqLibre);
    if(bloqLibre>block_count){
        return -1; // Todos los bloques se encuentran Llenos
    }
    returnBloque = bloqLibre;
    bloqLibre+=1;
    rewind(f[0]);
    fprintf(f[0],"%i",bloqLibre);
    return returnBloque;
}

int disenobdinicial:: validateBloqueLibre(){
    return bloqLibre;
}

int disenobdinicial::crearBaseDatos(){
    int i, pBloqTB=0, uBloqTB=0;
    char *buffer=(char*)malloc(block_size);
    if(buffer==NULL){
        return -2; // No se ha podido reservar el espacio de memoria.
    }
    //Abro el archivo de Base de Datos
    f[0]=fopen("BDFile.dat","w"); //Creo el archivo maestro de las BD.
    if(f[0]==NULL){
        return -1; // Error al abrir el archivo.
    }
    //Ingreso la Metadata del Archivo (SuperBlock)
    fwrite(&bloqLibre,sizeof(int),1,f[0]);
    fwrite(&block_size,sizeof(int),1,f[0]);
    fwrite(&block_count,sizeof(int),1,f[0]);
    fwrite(&pBloqTB,sizeof(int),1,f[0]);
    fwrite(&uBloqTB,sizeof(int),1,f[0]);
    //Crear los bloques del Archivo
    memset(buffer,'*',block_size);
    for(i=0 ; i<block_count; i++){
        fwrite(buffer,(sizeof(char)),block_size,f[0]);
    }
    free(buffer);
    fclose(f[0]);
    return 0;
}

int disenobdinicial::formatearBaseDatos(){
    int getBloqueL, getBloqueLTB,numTBCM ,regINT, numTBRG, hashValue, numCM;
    char tipoReg[3], regSTR[20], regCHR[1], regKey[5];

    f[0]=fopen("BDFile.dat","rwb+");
    if(f[0]==NULL){
        return -10; // Error al abrir el archivo.
    }
    //Ingreso de las Tablas prediseñadas
    getBloqueL=getBloqueLibre();
    if(getBloqueL==-1){
        return -2; // Todos los bloques se encuentran llenos.
    }
    f[1]=fopen("BDFile_Tablas.txt","r");
    if(f[1]==NULL){
        return -11; // Error al abrir el archivo.
    }
    fscanf(f[1],"%i",&bHTB.numTB);
    bHTB.bloqAntTB=-1;
    bHTB.bloqSigTB=-1;
    pos=20+(getBloqueL*block_size);
    fseek(f[0],pos,SEEK_SET);
    fwrite(&bHTB,sizeof(bloqHTB),1,f[0]); // Ingreso de Bloque de Header
    for(int i=0;i<bHTB.numTB;i++){
        fgetc(f[1]);
        fscanf(f[1],"%i",&bITB.numCampos);
        fread(bITB.nombTabla,1,20,f[1]);
        bITB.pBloqCampos=i+1;                // Campo 1 2
        bITB.uBloqCampos=-1;
        bITB.bloqHashValues=i+bHTB.numTB+1;  // Hash Table 3 4
        fwrite(&bITB,sizeof(bloqITB),1,f[0]);
    }
    fclose(f[1]);
    //Ingreso de los Campos predisenados
    f[2]=fopen("BDFile_Campos.txt","r");
    if(f[2]==NULL){
        return -1; // Error al abrir el archivo.
    }
    fscanf(f[2],"%i",&numTBCM);
    for(int i=0; i<numTBCM;i++){
        fgetc(f[2]);
        getBloqueL=getBloqueLibre();
        if(getBloqueL==-1){
            return -2; // Todos los bloques se encuentran llenos.
        }
        pos=20+(getBloqueL*block_size);
        fseek(f[0],pos,SEEK_SET);
        fscanf(f[2],"%i",&bHCM.numCM);
        bHCM.bloqAntCM=-1;
        bHCM.bloqSigCM=-1;
        bHCM.bloqTB=0;
        fwrite(&bHCM,sizeof(bloqHCM),1,f[0]);
        for(int k=0;k<bHCM.numCM;k++){
            fread(bICM.nombCampo,1,20,f[2]);
            fread(bICM.tipoCampo,1,3,f[2]);
            fwrite(&bICM,sizeof(bloqICM),1,f[0]);
        }
    }
    fclose(f[2]);
    //Creacion de los bloques de Hash Table
    for(int i=0; i<bHTB.numTB;i++){
        getBloqueL=getBloqueLibre();
        if(getBloqueL==-1){
            return -2; // Todos los bloques se encuentran llenos.
        }
        pos=20+(getBloqueL*block_size);
        fseek(f[0],pos,SEEK_SET);
        bHHT.bloqTB=0;
        bHHT.numHV=10;
        fwrite(&bHHT,sizeof(bloqHHT),1,f[0]);
    }
    getBloqueLTB=getBloqueL;
    int hashTable[9][bHTB.numTB];
    for(int i=0; i<bHTB.numTB;i++){
        pos=20+(((getBloqueLTB-bHTB.numTB)+1+i)*block_size)+8;
        fseek(f[0],pos,SEEK_SET);
        for(int c=0;c<10;c++){
            getBloqueL=getBloqueLibre();
            pos=20+(((getBloqueLTB-bHTB.numTB)+1+i)*block_size)+8+(c*12);
            fseek(f[0],pos,SEEK_SET);
            bIHT.hashValue=c;
            bIHT.pBloqRegistros=getBloqueL;
            bIHT.uBloqRegistros=-1;
            fwrite(&bIHT,sizeof(bloqIHT),1,f[0]);
            hashTable[c][i]=getBloqueL; //cout << "Hash Table: " << "[" << c<< "]"<< " [" << i<< "]" << " = " << getBloqueL << "\n";
        }
    }
    //Creacion bloques de Registros
    for(int tab=0; tab<bHTB.numTB;tab++){
        for(int hv=0;hv<10;hv++){
            getBloqueL=hashTable[hv][tab];
            pos=20+(getBloqueL*block_size);
            fseek(f[0],pos,SEEK_SET);
            bHRG.bloqTB=0;
            bHRG.bloqAntRG=-1;
            bHRG.bloqSigRG=-1;
            bHRG.numRG=0;
            fwrite(&bHRG,sizeof(bloqHRG),1,f[0]);
        }
    }
    //Ingreso de los Registros predisenados
    f[3]=fopen("BDFile_Registros.txt","r");
    if(f[3]==NULL){
        return -1; // Error al abrir el archivo.
    }
    fscanf(f[3],"%i",&numTBRG);
    fgetc(f[3]);
    for(int i=0; i<numTBRG;i++){
        fscanf(f[3],"%i",&bHRG.numRG);
        fgetc(f[3]);
        fscanf(f[3],"%i",&numCM);
        fgetc(f[3]);
        for(int j=0; j<bHRG.numRG;j++){
            fread(tipoReg,1,3,f[3]);
            fgetc(f[3]);
            fread(regKey,1,5,f[3]);
            fgetc(f[3]);
            hashValue=funcionHash(regKey);
            getBloqueL = hashTable[hashValue][i];
            pos=20+(getBloqueL*512);
            fseek(f[0],pos,SEEK_SET);
            bHRG.bloqAntRG=-1;
            bHRG.bloqSigRG=-1;
            bHRG.bloqTB=0;
            fwrite(&bHRG,sizeof(bloqHRG),1,f[0]);
            fwrite("KEY_",1,4,f[0]);
            fwrite(regKey,1,5,f[0]);
            for(int k=0; k<(numCM-1);k++){
                fread(tipoReg,1,3,f[3]);
                fgetc(f[3]);
                if((memcmp(tipoReg,"STR",3))==0){
                    fwrite("STR_",1,4,f[0]);
                    fread(regSTR,1,20,f[3]);
                    fwrite(regSTR,1,20,f[0]);
                }
                else if((memcmp(tipoReg,"CHR",3))==0){
                    fwrite("CHR_",1,4,f[0]);
                    fread(regCHR,1,1,f[3]);
                    fwrite(regCHR,1,1,f[0]);
                }
                else if((memcmp(tipoReg,"INT",3))==0){
                    fwrite("INT_",1,4,f[0]);
                    fscanf(f[3],"%i",&regINT);
                    fwrite(&regINT,1,4,f[0]);
                }
                else{
                    return -1;
                }
                fgetc(f[3]);
            }
        }
    }
    fclose(f[3]);
    fclose(f[0]);
    return 0; // El proceso se llevo a cabo con exito.
}
