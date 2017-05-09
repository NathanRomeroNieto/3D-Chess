//Clase Reina Herreda de la clase Pieza
class Queen : public Piece{
    public:
	//Pasar Parametros del contructor de la clase Rey al constructor de la clase pieza
        Queen(const char* modelFile, const char* textureFile,int textureNum,char col, int row) : Piece(modelFile, textureFile, textureNum,col,row){
            printf("Queen loaded\n");
            value = 9;
        };
        ~Queen();
        void listMoves(void);
};
