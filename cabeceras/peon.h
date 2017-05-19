#include "pieza.h"
#include "globales.h"
//Clase Peon Herreda de la clase Pieza
class Peon : public Pieza{
    public:
        bool firstMove = true;
	//Pasar Parametros del contructor de la clase Rey al constructor de la clase pieza
        Peon(const char* modelFile, const char* textureFile,int textureNum,char col, int row) : Pieza(modelFile, textureFile, textureNum,col,row){
            printf("Peon cargado\n");
            value = 1;
        };
        ~Peon();
        void ListaMovimientos(void);
        void movimiento(unsigned int col, unsigned int row);
        void check_promotion(void);
};

void Peon::movimiento(unsigned int col, unsigned int row){
    Pieza::movimiento(col,row);
    if(color == WHITE){
        if(piezas_tablero[row-2][col-1] == BLACK && pieza_en(col,row-1)->en_passant){
            remover_pieza(col,row-1);
        }
    }else if(color == BLACK){
        if(piezas_tablero[row][col-1] == WHITE && pieza_en(col,row+1)->en_passant){
            remover_pieza(col,row+1);
        }
    }
    for(int i = 0; i <= piezas.size()-1; i ++){
        piezas.at(i)->en_passant = false;
    }
    if(firstMove){
        firstMove = false;
        en_passant = true;
        printf("1st movimiento\n");
    }
    has_moved = true;
    check_promotion();
}

void Peon::check_promotion(void){
    Queen* temp = NULL;
    int index = get_index(c_Col,c_Row);
    if(index < 0)
        return;
    if(color == BLACK && c_Row == 1){
        temp = new Queen("data/models/queen.dae","data/textures/black_queen.jpg",3,c_Column,c_Row);
        piezas.at(index) = temp;
    }else if(color == WHITE && c_Row == 8){
        temp = new Queen("data/models/queen.dae","data/textures/white_queen.jpg",2,c_Column,c_Row);
        piezas.at(index) = temp;
    }
}

void Peon::ListaMovimientos(void){
    limpiarListaMovimientos();
    if(color == BLACK){
        if(!checkSquare(c_Col, c_Row-1)){ //if no Pieza is in front than space is free to movimiento
            highlight_tile(c_Col, c_Row-1, 0);
            if(firstMove && !checkSquare(c_Col, c_Row-2)) //first movimiento is the only time Peon can movimiento two squares
                highlight_tile(c_Col, c_Row-2, 1);
        }
        //diagonal capture moves
        if(piezas_tablero[c_Row-2][c_Col-2] == WHITE){
            highlight_tile(c_Col-1, c_Row-1, 2, true);
        }
        if(piezas_tablero[c_Row-2][c_Col] == WHITE){
            highlight_tile(c_Col+1, c_Row-1, 3, true);
        }
        //en passant
        if(piezas_tablero[c_Row-1][c_Col-2] == WHITE){
            if(pieza_en(c_Col-1,c_Row)->value == 1 && pieza_en(c_Col-1,c_Row)->en_passant){ //if it's a Peon after first movimiento
                highlight_tile(c_Col-1, c_Row-1, 4, true);
            }
        }
        if(piezas_tablero[c_Row-1][c_Col] == WHITE){
            if(pieza_en(c_Col+1,c_Row)->value == 1 && pieza_en(c_Col+1,c_Row)->en_passant){
                highlight_tile(c_Col+1, c_Row-1, 4, true);
            }
        }
    }else if(color == WHITE){
        if(!checkSquare(c_Col, c_Row+1)){
            highlight_tile(c_Col, c_Row+1, 0);
            if(firstMove && !checkSquare(c_Col, c_Row+2))
                highlight_tile(c_Col, c_Row+2, 1);
        }
        if(piezas_tablero[c_Row][c_Col-2] == BLACK){
            highlight_tile(c_Col-1, c_Row+1, 2, true);
        }
        if(piezas_tablero[c_Row][c_Col] == BLACK){
            highlight_tile(c_Col+1, c_Row+1, 3, true);
        }
        //en passant
        if(piezas_tablero[c_Row-1][c_Col-2] == BLACK){
            if(pieza_en(c_Col-1, c_Row)->value == 1 && pieza_en(c_Col-1,c_Row)->en_passant){
                highlight_tile(c_Col-1, c_Row+1, 4, true);
            }
        }
        if(piezas_tablero[c_Row-1][c_Col] == BLACK){
            if(pieza_en(c_Col+1, c_Row)->value == 1 && pieza_en(c_Col+1,c_Row)->en_passant){
                highlight_tile(c_Col+1, c_Row+1, 4, true);
            }
        }
    }
}
