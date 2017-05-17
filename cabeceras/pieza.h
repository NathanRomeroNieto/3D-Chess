#include "globales.h"

//Clase pieza es una clase abstracta
class Pieza{
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
        bool picked = false, en_passant = false, has_moved = false;
    	   //Fin Atributos

      	//explicit evita las conversiones de los tipos
      	//Constructor Clases pieza
        explicit Pieza(const char* modelFile, const char* textureFile,int textureNum,char col, int row);

        ~Pieza(){free(vertexArray);free(normalArray);free(uvArray);};

      	//Metodos virtuales
        virtual void Dibujar(bool side_Pieza = false, float x = 0.0f, float y = 0.0f);
	      virtual void ListaMovimientos(void);
        virtual void movimiento(unsigned int col, unsigned int row);
        virtual void movimiento(char col, unsigned int row);
      	//Fin de Metodos virtuales

      	//Metodos
      	void pick(void);
        void unpick(void);
        void CreacionListaMovimientos(int col_inc, int row_inc, int min_array);
        void render(void);
};

//Implementacion De la Clase

Pieza::Pieza(const char* modelFile, const char* textureFile,int textureNum,char col,int row){
    angle = 0.0f;
    loadOBJ(modelFile,vertexArray,normalArray,uvArray,numVerts);
    textureNumber = textureNum;
    loadGLTexture(textureFile,textureNumber,texture);
    c_Row = row;
    for(unsigned int i = 0; i <= sizeof(column)/sizeof(char); i++){
        if(tolower(column[i]) == tolower(col)){
            c_Col = i+1; //letter to int
        }
    }
    c_Column = col;
    if(strstr(textureFile,"black")){
        color = BLACK;
        grid_Piezas[c_Row-1][c_Col-1] = color;
    }else if(strstr(textureFile,"white")){
        color = WHITE;
        grid_Piezas[c_Row-1][c_Col-1] = color;
    }else{
        color = -1;
    }
}

void Pieza::render(void){
    glBindTexture(GL_TEXTURE_2D,texture[textureNumber]);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3,GL_FLOAT,0,vertexArray);
    glNormalPointer(GL_FLOAT,0,normalArray);

    glTexCoordPointer(2,GL_FLOAT,0,uvArray);

    glDrawArrays(GL_TRIANGLES,0,numVerts);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Pieza::Dibujar(bool side_Pieza, float pX, float pZ){
    float x = pX, z = pZ;
    glPushMatrix();

    if(!side_Pieza){
        x = -11.3+(2.5*c_Col);
        z = 11.3-(2.5*c_Row);
    }
    if(c_Column != 'z'){ //normal Piezas
        glColor3f(1,1,1);
        glScalef(0.8,0.8,0.8);
        glTranslatef(x,-1.25,z);
    }else if(c_Column == 'z'){ //board
        glColor4ub(238,221,187,255);
        glTranslatef(0,-2,0);
    }
    render();

    glPopMatrix();
}

void Pieza::pick(void){
    picked = true;
    ListaMovimientos();
    grid_row = c_Row;
    grid_col = c_Col;
    grid_column = c_Column;
}

void Pieza::unpick(void){
    picked = false;
}

void Pieza::CreacionListaMovimientos(int col_inc, int row_inc, int min_array){ //column increment,row increment
    int r = c_Row, c = c_Col;
    for(int i = 0; i < 7; i++){
        c = c_Col-(col_inc*(i+1));
        r = c_Row-(row_inc*(i+1));
        if(checkSquare(c,r)){ //if the square has a Pieza in it
            if(!((grid_Piezas[r-1][c-1] == BLACK && gamestate == BLACK_TURN) || (grid_Piezas[r-1][c-1] == WHITE && gamestate == WHITE_TURN))){
                highlight_tile(c,r,min_array+i,true);
            }
            break;
        }
        highlight_tile(c,r,min_array+i);
    }
}

void Pieza::ListaMovimientos(void){
    limpiarListaMovimientos();
}

void Pieza::movimiento(unsigned int col, unsigned int row){
    grid_Piezas[c_Row-1][c_Col-1] = 0;
    c_Col = col;
    c_Row = row;
    c_Column = column[col-1];
    grid_Piezas[c_Row-1][c_Col-1] = color;
    has_moved = true;
}

void Pieza::movimiento(char col, unsigned int row){
    grid_Piezas[c_Row-1][c_Col-1] = 0;
    for(unsigned int i = 0; i <= sizeof(column)/sizeof(char); i++){
        if(tolower(column[i]) == tolower(col)){
            c_Col = i+1;
        }
    }
    c_Row = row;
    c_Column = col;
    grid_Piezas[c_Row-1][c_Col-1] = color;
    for(int i = 0; i <= Piezas.size()-1; i ++){
        Piezas.at(i)->en_passant = false;
    }
    has_moved = true;
}
