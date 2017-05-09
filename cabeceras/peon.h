//Clase Peon Herreda de la clase Pieza
class Pawn : public Piece{
    public:
        bool firstMove = true;
	//Pasar Parametros del contructor de la clase Rey al constructor de la clase pieza
        Pawn(const char* modelFile, const char* textureFile,int textureNum,char col, int row) : Piece(modelFile, textureFile, textureNum,col,row){
            printf("Pawn loaded\n");
            value = 1;
        };
        ~Pawn();
        void listMoves(void);
        void move(unsigned int col, unsigned int row);
        void check_promotion(void);
};
