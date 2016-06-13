#include "disenomenu.h"
#include "mantenimientobd.h"
#include "disenobdinicial.h"

mantenimientobd vDM_MBD; // Variable en Diseno de Menus de Mantenimiento de Base de Datos
disenobdinicial vDM_DBI; // Variable en Diseno de Menus de Diseno de Base de Datos Inicial
struct bloqPosTB bDMPosTB;

disenomenu::disenomenu()
{

}

void disenomenu::menuPrincipal(){
    int opcPri;
    string opcion="OK";
    cout<<" * * * * * * * * * * * * * * * * * * * * * * \n";
    cout<<" * Menu de Mantenimiento de Base de Datos  * \n";
    cout<<" * * * * * * * * * * * * * * * * * * * * * * \n\n";
    cout<<"1. Ver Base de datos disponibles. \n";
    cout<<"2. Mantenimiento de Base de Datos. \n";
    cout<<"3. Salir del Sistema \n\n";
    while(opcion=="OK"){
        cout<<"Ingrese un numero: ";
        cin>>opcPri;
        if(opcPri>=1 && opcPri<=3){
            opcion="NOOK";
            switch(opcPri){
                case 1:
                    menuVisualizacionBD();
                break;
                case 2:
                    menumantenimientoBD();
                break;
                case 3:
                exit(1);
            }
        }
        if(cin.fail()){
            cin.clear();
            cin.ignore(100, '\n');
            cout << "Recuerde que debe de ingresar valores numericos.\n";
        }
    }
}

void disenomenu::menuVisualizacionBD(){
    int opcPri;
    string opcion="OK";
    cout<<" * * * * * * * * * * * * * * * * * * * * * * \n";
    cout<<" * Menu de Visualizacion de Base de Datos  * \n";
    cout<<" * * * * * * * * * * * * * * * * * * * * * * \n\n";
    cout<<"1. BDKevin \n";
    cout<<"2. Regresar al Menu Principal \n";
    cout<<"3. Salir del Sistema \n\n";
    while(opcion=="OK"){
        cout<<"Ingrese un numero: ";
        cin>>opcPri;
        if(opcPri>=1 && opcPri<=3){
            opcion="NOOK";
            switch(opcPri){
                case 1:
                    visualizarTablas();
                break;
                case 2:
                    menuPrincipal();
                break;
                case 3:
                exit(1);
            }
        }
        if(cin.fail()){
            cin.clear();
            cin.ignore(100, '\n');
            cout << "Recuerde que debe de ingresar valores numericos.\n";
        }
    }
}

void disenomenu::menumantenimientoBD(){
    int opcPri;
    string opcion="OK";
    cout<<" * * * * * * * * * * * * * * * * * * * * * * \n";
    cout<<" * Menu de Mantenimiento de Base de Datos  * \n";
    cout<<" * * * * * * * * * * * * * * * * * * * * * * \n\n";
    cout<<"1. Crear una Tabla Nueva \n";
    cout<<"2. Agregar un Campo en una Tabla \n";
    cout<<"3. Crear un Registro en una Tabla \n";
    cout<<"4. Regresar al Menu Principal \n";
    cout<<"5. Salir del Sistema \n\n";
    while(opcion=="OK"){
        cout<<"Ingrese un numero: ";
        cin>>opcPri;
        if(opcPri>=1 && opcPri<=5){
            opcion="NOOK";
            switch(opcPri){
                case 1:
                    formularioTablas();
                    break;
                case 2:
                    formularioCampos();
                    break;
                case 3:
                    formularioRegistros();
                    break;
                case 4:
                    menuPrincipal();
                    break;
                case 5:
                    exit(1);
            }
        }
        if(cin.fail()){
            cin.clear();
            cin.ignore(100, '\n');
            cout << "Recuerde que debe de ingresar valores numericos.\n";
        }
    }
}

void disenomenu::visualizarTablas(){
    int numTB, valTB, numCM, bloqCM;
    cout<<" \n";
    cout<<" * Tablas Disponibles - BDKevin  * \n";
    cout<<" ------------------------------------- \n\n";
    numTB=vDM_MBD.numTablas();
    string tablas[numTB];
    vDM_MBD.imprimirTablas(tablas);
    for (int k=0;k<numTB;k++){
        cout << k+1 << ". " << tablas[k] << endl;
    }
    cout<<"\nIngrese el numero de Tabla: ";
    cin >> valTB;
    numCM=vDM_MBD.getnNumCM(tablas[valTB-1]);
    bloqCM=vDM_MBD.getBloqCampos(tablas[valTB-1]);
    string nomCM[numCM],tipoCM[numCM];
    vDM_MBD.imprimirCampos(bloqCM,nomCM,tipoCM);
    cout<<" * Campos Disponibles - BDKevin  * \n";
    cout<<" ------------------------------------- \n\n";
    cout<<"   Nombre del Campo  - Tipo Campo \n";
    for (int k=0;k<numCM;k++){
        cout << k+1 << ". " << nomCM[k] << endl;
    }
    menuPrincipal();
}

void disenomenu::formularioTablas(){
    string nomTabla, nomTablaOK;
    int numCM, returnVal;
    cout<<" \n";
    cout<<" * Formulario de Creacion de Tablas  * \n";
    cout<<" ------------------------------------- \n\n";
    cout<<"Ingrese el nombre de la Tabla: ";
    cin >> nomTabla;
    cout<<"Ingrese el numero de Campos: ";
    cin >> numCM;
    if(nomTabla.length()<20){
        nomTabla.append(20-(nomTabla.length()),' ');
    }
    if(nomTabla.length()>=20){
        nomTablaOK.append(nomTabla,0,20);
    }
    returnVal = vDM_MBD.crearTabla(nomTablaOK,numCM);
    if(returnVal==0){
        cout << "\n - - - La Tabla se ha creado Exitosamente. - - -\n";
    }
    else{
        cout << "\n - - - Error: Espacio insuficiente de almacenamiento disponible. - - -\n";
    }
    cout<<" \n";
    menumantenimientoBD();
}

void disenomenu::formularioCampos(){
    int numTB, retVal;
    string opcion="OK", nomCampo, nomCampoOK, tipoCM;
    cout<<" \n";
    cout<<" * Formulario de Creacion de Campos - Seleccion de Tabla * \n";
    cout<<" ------------------------------------- \n\n";
    numTB=vDM_MBD.numTablas();
    string tablas[numTB];
    vDM_MBD.imprimirTablas(tablas);
    for (int k=0;k<numTB;k++){
        cout << k+1 << ". " << tablas[k] << endl;
    }
    cout<<"\nIngrese el numero de Tabla: ";
    cin >> numTB;
    cout<<" * Formulario de Creacion de Campos - Creacion del Campo * \n";
    cout<<" ------------------------------------- \n\n";
    cout<<"Ingrese el nombre del campo(MAX 20): ";
    cin >> nomCampo;
    if(nomCampo.length()<20){
        nomCampo.append(20-(nomCampo.length()),' ');
    }
    if(nomCampo.length()>=20){
        nomCampoOK.append(nomCampo,0,20);
    }
    while(opcion=="OK"){
        cout<<"Ingrese el Tipo de Campo (STR CHR INT KEY): ";
        cin >> tipoCM;
        if(tipoCM.compare("STR")==0||tipoCM.compare("CHR")==0||tipoCM.compare("INT")==0||tipoCM.compare("KEY")==0){
            opcion="NOOK";
        }
    }
    retVal=vDM_MBD.crearCampo(nomCampoOK,tipoCM,tablas[numTB-1]);
    if(retVal==0){
        cout << "\n - - - El Campo se ha agregado Exitosamente. - - -\n" << endl;
    }
    else if(retVal==-1){
        cout << "\n Error: La tabla llego al maximo de Campos permitidos. \n" << endl;
    }
    else{
        cout << "\n Error: Nombre de Tabla Incorrecto. \n" << endl;
    }
    menumantenimientoBD();
}

void disenomenu::formularioRegistros(){
    int numTB, numCM, bloqCM, retVal, valTB;
    cout<<" \n";
    cout<<" * Formulario de Creacion de Registros - Seleccion de Tabla * \n";
    cout<<" ------------------------------------- \n\n";
    numTB=vDM_MBD.numTablas();
    string tablas[numTB];
    vDM_MBD.imprimirTablas(tablas);
    for (int k=0;k<numTB;k++){
        cout << k+1 << ". " << tablas[k] << endl;
    }
    cout<<"\nIngrese el numero de Tabla: ";
    cin >> valTB;
    cout<<" * Formulario de Creacion de Registros - Creacion del Registro * \n";
    cout<<" ------------------------------------- \n\n";
    numCM=vDM_MBD.getnNumCM(tablas[valTB-1]);
    bloqCM=vDM_MBD.getBloqCampos(tablas[valTB-1]);
    string nomCM[numCM],tipoCM[numCM],registro[numCM],registroOK[numCM];
    vDM_MBD.imprimirCampos(bloqCM,nomCM,tipoCM);
    for (int k=0;k<numCM;k++){
        cout << "Nombre del Campo: " << nomCM[k] << endl;
        cout << "Tipo de Campo: " << tipoCM[k] << endl;
        cout << "Ingrese el registro: ";
        cin >> registro[k];
        cout << "\n";
        if(registro[k].length()<20){
            registro[k].append(20-(registro[k].length()),' ');
        }
        if(registro[k].length()>=20){
            registroOK[k].append(registro[k],0,20);
        }
    }
    vDM_MBD.crearRegistro(tablas[valTB-1],tipoCM,registroOK,numCM);
    cout << "\n - - - El Registro se ha agregado Exitosamente. - - -\n" << endl;
    menumantenimientoBD();
}


