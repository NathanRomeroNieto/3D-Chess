//Clase Torre Herreda de la clase Pieza
class Rook : public Piece{
    public:
	//Pasar Parametros del contructor de la clase Rey al constructor de la clase pieza
        Rook(const char* modelFile, const char* textureFile,int textureNum,char col, int row) : Piece(modelFile, textureFile, textureNum,col,row){
            printf("Rook loaded\n");
            value = 5;
        };
        ~Rook();
        void listMoves(void);
};
