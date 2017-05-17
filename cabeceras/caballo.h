#include "pieza.h"
#include "globales.h"

//Clase Caballo Herreda de la clase Pieza
class Caballo : public Pieza{
    public:
	//Pasar Parametros del contructor de la clase Rey al constructor de la clase pieza
        Caballo(const char* modelFile, const char* textureFile,int textureNum,char col, int row) : Pieza(modelFile, textureFile, textureNum,col,row){
            printf("Caballo Cargado\n");
            value = 3;
            var_swag = 0.0f;
        };
        ~Caballo();
        void dibujar(bool side_Pieza = false, float x = 0.0f, float y = 0.0f);
        void ListaMovimientos(void);
};

void Caballo::dibujar(bool side_piece, float pX, float pZ){
    float x = pX, z = pZ;
    glPushMatrix();

    if(!side_piece){
        x = -11.3+(2.5*c_Col);
        z = 11.3-(2.5*c_Row);
    }
    glColor3f(1,1,1);
    glScalef(0.8,0.8,0.8);
    glTranslatef(-7.3+x,-2.25,-0.8+z);
    render();

    glPopMatrix();
}

void Caballo::ListaMovimientos(void){
    limpiarListaMovimientos();
    highlight_tile(c_Col-1,c_Row+2,0);
    highlight_tile(c_Col+1,c_Row+2,1);
    highlight_tile(c_Col-1,c_Row-2,2);
    highlight_tile(c_Col+1,c_Row-2,3);
    highlight_tile(c_Col-2,c_Row+1,4);
    highlight_tile(c_Col-2,c_Row-1,5);
    highlight_tile(c_Col+2,c_Row+1,6);
    highlight_tile(c_Col+2,c_Row-1,7);
}
