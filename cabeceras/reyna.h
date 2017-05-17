#include "pieza.h"
#include "globales.h"

//Clase Reina Herreda de la clase Pieza
class Reina : public Pieza{
    public:
	     //Pasar Parametros del contructor de la clase Rey al constructor de la clase pieza
        Reina(const char* modelFile, const char* textureFile,int textureNum,char col, int row) : Pieza(modelFile, textureFile, textureNum,col,row){
            printf("Reina Cargada\n");
            value = 9;
        };
        ~Reina();
        void ListaMovimientos(void);
};

void Reina::ListaMovimientos(void){
    limpiarListaMovimientos();
    CreacionListaMovimientos(1,0,0);
    CreacionListaMovimientos(-1,0,7);
    CreacionListaMovimientos(0,1,14);
    CreacionListaMovimientos(0,-1,21);
    CreacionListaMovimientos(1,1,28);
    CreacionListaMovimientos(-1,1,35);
    CreacionListaMovimientos(-1,-1,42);
    CreacionListaMovimientos(1,-1,48);
}
