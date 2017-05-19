#include "globales.h"
#include "pieza.h"


//Clase Rey Herreda de la clase Pieza
class Rey : public Pieza{
    public:
	//Pasar Parametros del contructor de la clase Rey al constructor de la clase pieza
        Rey(const char* modelFile, const char* textureFile,int textureNum,char col, int row) : Pieza(modelFile, textureFile, textureNum,col,row){
            printf("Rey loaded\n");
            value = 9999;
        };
        ~Rey();

	//Metodos
        void ListaMovimientos(void);
        void movimiento(unsigned int col, unsigned int row);
};

void Rey::ListaMovimientos(void){
    limpiarListaMovimientos();
    highlight_tile_k(c_Col+1,c_Row,0);
    highlight_tile_k(c_Col-1,c_Row,1);
    highlight_tile_k(c_Col,c_Row+1,2);
    highlight_tile_k(c_Col,c_Row-1,3);
    highlight_tile_k(c_Col+1,c_Row+1,4);
    highlight_tile_k(c_Col+1,c_Row-1,5);
    highlight_tile_k(c_Col-1,c_Row+1,6);
    highlight_tile_k(c_Col-1,c_Row-1,7);
    if(!has_moved){
        if(!checkSquare(c_Col-3,c_Row) && !checkSquare(c_Col-2,c_Row) && !checkSquare(c_Col-1,c_Row)){
            highlight_tile(c_Col-2,c_Row,8);
        }
        if(!checkSquare(c_Col+2,c_Row) && !checkSquare(c_Col+1,c_Row)){
            highlight_tile(c_Col+2,c_Row,9);
        }
    }
}

void Rey::movimiento(unsigned int col, unsigned int row){
    piezas_tablero[c_Row-1][c_Col-1] = 0;
    if(c_Col-col == 2){
        if(color == WHITE){
            pieza_en(1,1)->movimiento((unsigned int)4,(unsigned int)1);
        }else if(color == BLACK){
            pieza_en(1,8)->movimiento((unsigned int)4,(unsigned int)8);
        }
    }else if(c_Col-col == -2){
        if(color == WHITE){
            pieza_en(8,1)->movimiento((unsigned int)6,(unsigned int)1);
        }else if(color == BLACK){
            pieza_en(8,8)->movimiento((unsigned int)6,(unsigned int)8);
        }
    }
    c_Col = col;
    c_Row = row;
    c_Column = columna[col-1];
    piezas_tablero[c_Row-1][c_Col-1] = color;
    has_moved = true;
}
