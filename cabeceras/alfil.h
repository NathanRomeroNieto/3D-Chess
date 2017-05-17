#include "pieza.h"
#include "globales.h"

//Clase Alfil Herreda de la clase Pieza
class Alfil : public Pieza{
    public:
	//Pasar Parametros del contructor de la clase Rey al constructor de la clase pieza
        Alfil(const char* modelFile, const char* textureFile,int textureNum,char col, int row) : Pieza(modelFile, textureFile, textureNum,col,row){
            printf("Alfil Cargado\n");
            value = 3;
        };
        ~Alfil();
        void ListaMovimientos(void);
};

void Alfil::ListaMovimientos(void){
    limpiarListaMovimientos();
    CreacionListaMovimientos(1,1,0);
    CreacionListaMovimientos(-1,1,7);
    CreacionListaMovimientos(-1,-1,14);
    CreacionListaMovimientos(1,-1,21);
}
