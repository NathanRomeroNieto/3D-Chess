#include <GL/glew.h>
#include <GL/freeglut.h>
#include "lectorarchivos.h"
#include "cargador.h"
#include <vector>
#include <algorithm>

using namespace std;

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
//Definicion del estado del juego
enum GAMESTATE : int{
    MENU,WHITE_TURN,BLACK_TURN,WHITE_WIN,BLACK_WIN,TIE
};

//Declaraciones de Variables
GLuint texture[32];
float angle = 0.0f;
GLuint p,f,v,tile;
int grid_row, grid_col;
char grid_column;
int highlighted_tiles[56][2] = {0};
int grid_pieces[8][8] = {0};
GAMESTATE gamestate;
char column[9] = {'a','b','c','d','e','f','g','h','z'};
vector<Piece*> pieces,side_pieces;

//Declaracion de Funciones

Piece* piece_at(int col, int row){
    for(int i = 0; i <= pieces.size()-1; i++){
        if(pieces.at(i)->c_Row == row && pieces.at(i)->c_Col == col){
            return pieces.at(i);
        }
    }
}

void remove_piece(int col, int row){
    Piece* temp = piece_at(col, row);
    auto it = std::find(pieces.begin(), pieces.end(), temp);
    side_pieces.push_back(temp);
    if(it != pieces.end()){
        pieces.erase(it);
        grid_pieces[row-1][col-1] = 0;
    }
}

void initDLs(void){
    printf("%i\n",A);
    tile = glGenLists(1);
    glNewList(tile,GL_COMPILE);
    glBegin(GL_QUADS);
        glVertex3f(1,0,1);
        glVertex3f(-1,0,1);
        glVertex3f(-1,0,-1);
        glVertex3f(1,0,-1);
    glEnd();
    glEndList();
}

void drawGrid(void){
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glColor4f(1,1,1,0);
    int counter = 0;
    for(int i = 0; i <= 7; i++){
        for(int j = 0; j <= 7; j++){
            counter++;
            glLoadName(100+counter);
            glPushMatrix();
            glColor4f(1,1,1,0);
            glTranslatef(-7+(i*2),-0.97,(-7+(j*2)));
            glCallList(tile);
            glPopMatrix();
        }
    }
    for(int k = 0; k < sizeof(highlighted_tiles)/sizeof(highlighted_tiles[0]); k++){
        if(highlighted_tiles[k] != 0){
            int row = highlighted_tiles[k][0]-1;
            int col = highlighted_tiles[k][1]-1;
            if(col >= 0 && col >= 0 && col <= 7 && col <= 7){
                glLoadName(165);
                glPushMatrix();
                glColor4f(1,0,0,0.5);
                glTranslatef(-7+(col*2),-0.96,-(-7+(row*2)));
                glCallList(tile);
                glColor4f(1,0,0,1);
                glBegin(GL_LINES);
                glVertex3f(1,0,1);
                glVertex3f(0.4,0,1);
                glVertex3f(1,0,-0.9);
                glVertex3f(0.4,0,-0.9);
                glVertex3f(-1,0,1);
                glVertex3f(-0.4,0,1);
                glVertex3f(-1,0,-0.9);
                glVertex3f(-0.4,0,-0.9);
                glVertex3f(-0.9,0,1);
                glVertex3f(-0.9,0,0.4);
                glVertex3f(-0.9,0,-1);
                glVertex3f(-0.9,0,-0.4);
                glVertex3f(0.9,0,1);
                glVertex3f(0.9,0,0.4);
                glVertex3f(0.9,0,-1);
                glVertex3f(0.9,0,-0.4);
                glEnd();
                glPopMatrix();
            }
        }
    }
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

void setShaders(void){
    char *vs = NULL,*fs = NULL;

    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);

    vs = textFileRead("shaders/swag.vert");
    fs = textFileRead("shaders/swag.frag");

    const char *vv = vs;
    const char *ff = fs;

    glShaderSource(v,1,&vv,NULL);
    glShaderSource(f,1,&ff,NULL);

    free(vs);free(fs);

    glCompileShader(v);
    glCompileShader(f);

    p = glCreateProgram();

    glAttachShader(p,f);
    glAttachShader(p,v);

    glLinkProgram(p);
    glUseProgram(p);
}

bool checkSquare(int col, int row){
    if(grid_pieces[row-1][col-1] >= 1){
        return true;
    }
    return false;
}

void clearMovesList(void){
    memset(highlighted_tiles,0,sizeof(highlighted_tiles[0][0])*(sizeof(highlighted_tiles)/sizeof(highlighted_tiles[0]))*2); //clear the array
}

int get_index(int col, int row){
    auto it = std::find(pieces.begin(), pieces.end(), piece_at(col,row));
    if (it == pieces.end()){
        return -1;
    }else{
        return std::distance(pieces.begin(), it);
    }
}

void highlight_tile(int col, int row,unsigned int tile, bool captured_mode = false){
    if((col <= 8 && row <= 8 && col > 0 && row > 0) && ((gamestate == WHITE_TURN && grid_pieces[row-1][col-1] != WHITE) ||(gamestate == BLACK_TURN && grid_pieces[row-1][col-1] != BLACK))){
        highlighted_tiles[tile][0] = row;
        highlighted_tiles[tile][1] = col;
    }
}

void highlight_tile_k(int col, int row,unsigned int tile, bool captured_mode = false){
    highlight_tile(col, row, tile, captured_mode);
}
