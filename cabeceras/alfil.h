//Clase Alfil Herreda de la clase Pieza
class Bishop : public Piece{
    public:
	//Pasar Parametros del contructor de la clase Rey al constructor de la clase pieza
        Bishop(const char* modelFile, const char* textureFile,int textureNum,char col, int row) : Piece(modelFile, textureFile, textureNum,col,row){
            printf("Bishop loaded\n");
            value = 3;
        };
        ~Bishop();
        void listMoves(void);
};
