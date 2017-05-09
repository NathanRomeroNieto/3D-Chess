//Clase Caballo Herreda de la clase Pieza
class Knight : public Piece{
    public:
	//Pasar Parametros del contructor de la clase Rey al constructor de la clase pieza
        Knight(const char* modelFile, const char* textureFile,int textureNum,char col, int row) : Piece(modelFile, textureFile, textureNum,col,row){
            printf("Knight loaded\n");
            value = 3;
            var_swag = 0.0f;
        };
        ~Knight();
        void draw(bool side_piece = false, float x = 0.0f, float y = 0.0f);
        void listMoves(void);
};
