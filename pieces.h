#include <GL/glew.h>
#include <GL/freeglut.h>
#include "textfile.h"
#include "loaders.h"
#include <vector>
#include <algorithm>

//Definiciones de los valores de las columnas del tablero 
#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define F 5
#define G 6
#define H 7
//Definiciones del valor del turno
#define BLACK 1337
#define WHITE 6969
//Declaracion del estado del juego
enum GAMESTATE : int{
    MENU,WHITE_TURN,BLACK_TURN,WHITE_WIN,BLACK_WIN,TIE
};

//Variables Globales
	 
extern int grid_pieces[8][8], grid_row, grid_col, highlighted_tiles[56][2]; //No se porque es 56*2
extern char column[9], grid_column;
extern GAMESTATE gamestate; //enum which stores current state, like player turns and menu
extern GLuint p,f,v; //program, frag shader, vert shader

//Funciones Globales
extern void setShaders(void);
extern void drawGrid(void);
extern void initDLs(void);
extern void clearMovesList(void);
extern std::vector<Piece*> pieces,side_pieces;
extern Piece* piece_at(int col, int row);
extern void remove_piece(int col, int row);


//Clase pieza es una clase abstracta 

class Piece{
    public:
	//Atributos
		//c_ = Current 
		int c_Row, c_Col, color, textureNumber, value, numVerts; 
		char c_Column;

		//Para entender como funcionan:
		//http://www.songho.ca/opengl/gl_vertexarray.html#
		//uvArray = http://www.c-jump.com/bcc/common/Talk3/OpenGL/Wk07_texture/const_images/texturemapping.png
		GLfloat *vertexArray,*normalArray,*uvArray;
		float var_swag;
	//Fin Atributos
	
	bool picked = false, en_passant = false, has_moved = false;
	//explicit evita las conversiones de los tipos
	//Constructor Clases pieza
        explicit Piece(const char* modelFile, const char* textureFile,int textureNum,char col, int row);
	
        ~Piece(){free(vertexArray);free(normalArray);free(uvArray);};
	
	//Metodos virtuales 
        virtual void draw(bool side_piece = false, float x = 0.0f, float y = 0.0f);
	virtual void listMoves(void);
        virtual void move(unsigned int col, unsigned int row);
        virtual void move(char col, unsigned int row);
	//Fin de Metodos virtuales
	
	//Metodos 
	void pick(void);
        void unpick(void);
        void createMoveList(int col_inc, int row_inc, int min_array);
        void render(void);
};

//Clase Rey Herreda de la clase Pieza
class King : public Piece{
    public:
	//Pasar Parametros del contructor de la clase Rey al constructor de la clase pieza
        King(const char* modelFile, const char* textureFile,int textureNum,char col, int row) : Piece(modelFile, textureFile, textureNum,col,row){
            printf("King loaded\n");
            value = 9999;
        };
        ~King();
	
	//Metodos
        void listMoves(void);
        void move(unsigned int col, unsigned int row);
};

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
