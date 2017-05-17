#include "pieza.h"
#include "globales.h"

//Clase Torre Herreda de la clase Pieza
class Torre : public Pieza{
    public:
	//Pasar Parametros del contructor de la clase Rey al constructor de la clase pieza
        Torre(const char* modelFile, const char* textureFile,int textureNum,char col, int row) : Pieza(modelFile, textureFile, textureNum,col,row){
            printf("Torre Cargado\n");
            value = 5;
        };
        ~Torre();
        void ListaMovimientos(void);
};

void Torre::ListaMovimientos(void){
    limpiarListaMovimientos();
    CreacionListaMovimientos(1,0,0);
    CreacionListaMovimientos(-1,0,7);
    CreacionListaMovimientos(0,1,14);
    CreacionListaMovimientos(0,-1,21);
}
