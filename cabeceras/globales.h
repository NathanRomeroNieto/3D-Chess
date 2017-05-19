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
enum estadodeljuego : int{
    MENU,TURNO_BLANCAS,TURNO_NEGRAS,GANAN_LAS_BLANCAS,GANAN_LAS_NEGRAS,EMPATE
};

//Declaraciones de Variables
GLuint textura[32];
float angulo = 0.0f;
GLuint p,f,v,casilla;
int fila_tablero, columna_tablero;
char columna_tablero_c;
int casillas_resaltadas[56][2] = {0};
int piezas_tablero[8][8] = {0};
estadodeljuego estadodeljuego;
char columna[9] = {'a','b','c','d','e','f','g','h','z'};
vector<Pieza*> piezas,piezas_comidas;

//Declaracion de Funciones

/*!
   \brief "Funcion para retornar que pieza esta en
          la posision enviada por parametros"
   \param "int Col, int row"
   \return "Pieza* "
*/
Pieza* pieza_en(int col, int row){
    for(int i = 0; i <= piezas.size()-1; i++){
        if(piezas.at(i)->c_Row == row && piezas.at(i)->c_Col == col){ //busqueda si existe una pieza en esta posision
            return piezas.at(i); //retorno de un apuntador de la clase pieza
        }
    }
}

/*!
   \brief "Funcion para remover una pieza del tablero"
   \param "int col, int row"
   \return "Void"
*/
void remover_pieza(int col, int row){
    Pieza* temp = pieza_en(col, row); //retorno de la pieza
    auto esta_pieza = find(piezas.begin(), piezas.end(), temp); // Busca de principio a fin del vector piezas el valor de temp
    piezas_comidas.push_back(temp); //Piezas comidas
    if(esta_pieza != piezas.end()){
        piezas.erase(esta_pieza); //borrar elemento encontrado
        piezas_tablero[row-1][col-1] = 0; //borrar la pieza del tablero
    }
}

/*!
   \brief "Generacion del los casillas"
   \param "Void"
   \return "Void"
*/
void initDLs(void){
    printf("%i\n",A);
    casilla = glGenLists(1); //Genera una lista de visualizacion

    //Rellena la lista y la compila
    glNewList(casilla,GL_COMPILE);
      glBegin(GL_QUADS);
          glVertex3f(1,0,1);
          glVertex3f(-1,0,1);
          glVertex3f(-1,0,-1);
          glVertex3f(1,0,-1);
      glEnd();
    glEndList();
}

/*!
   \brief "Funcion para dibujar el tablero"
   \param "void"
   \return "void"
*/
void drawGrid(void){
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glColor4f(1,1,1,0);
    int counter = 0;
    //Dibuja cada celda
    for(int i = 0; i <= 7; i++){
        for(int j = 0; j <= 7; j++){
            counter++;
            glLoadName(100 + counter); //Cargar un nombre en el Stack de nombres
            glPushMatrix();
            glColor4f(1,1,1,0);
            glTranslatef( -7+(i*2) , -0.97 , (-7+(j*2)) );
            glCallList(casilla); //Ejecuta la lista celda
            glPopMatrix();
        }
    }

    //Dibuja las celdas marcadas donde se puede mover la pieza
    //En la inicializacion no es posible llamarla pues esta inicializada con 0
    for(int k = 0; k < sizeof(casillas_resaltadas)/sizeof(casillas_resaltadas[0]); k++){
        if(casillas_resaltadas[k] != 0){
            int row = casillas_resaltadas[k][0]-1;
            int col = casillas_resaltadas[k][1]-1;
            if(col >= 0 && col >= 0 && col <= 7 && col <= 7){
                glLoadName(165);
                glPushMatrix();
                glColor4f(1,0,0,0.5);
                glTranslatef(-7+(col*2), -0.96 , -(-7+(row*2)) );
                glCallList(casilla);
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

/*!
   \brief "Definir los shaders de las figuras, los cuales se encuentra el la carpeta data"
   \param "Void"
   \return "Void"
*/
void setShaders(void){
    char *vs = NULL,*fs = NULL;

    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);

    vs = textFileRead("shaders/swag.vert");
    fs = textFileRead("shaders/swag.frag");

    const char *vv = vs;
    const char *ff = fs;

    glShaderSource(v , 1 , &vv , NULL);
    glShaderSource(f , 1 , &ff , NULL);

    free(vs);free(fs);

    glCompileShader(v);
    glCompileShader(f);

    p = glCreateProgram();

    glAttachShader(p,f);
    glAttachShader(p,v);

    glLinkProgram(p);
    glUseProgram(p);
}

/*!
   \brief "Te retorna si existe la casilla que mandas a llamar"
   \param "int col, int row"
   \return "bool"
*/
bool checkSquare(int col, int row){
    if(piezas_tablero[row-1][col-1] >= 1){
        return true;
    }
    return false;
}

/*!
   \brief "Limpiar la lista de movimientos se encarga de reiniciar el array casillas_marcadas"
   \param "void"
   \return "void"
*/
void limpiarListaMovimientos(void){
    memset(casillas_resaltadas,0,sizeof(casillas_resaltadas[0][0])*(sizeof(casillas_resaltadas)/sizeof(casillas_resaltadas[0]))*2); //clear the array
}


/*!
   \brief "Funcion para retornar la distancia entre el inicio y la pieza"
   \param "int col, int row"
   \return "int"
*/
int get_index(int col, int row){
    auto esta_pieza = find(piezas.begin(), piezas.end(), pieza_en(col,row)); //verifica que exista un position una pieza
    if (esta_pieza == piezas.end()){
        return -1;
    }else{
        return std::distance(piezas.begin(), esta_pieza);
    }
}

/*!
   \brief "Funcion para remarcar las casillas con los posbles moviemientos y si el resultado de la operacion es correcta se almacena en las casillas_marcadas "
   \param "int col, int row, unsigned int casilla, bool captured_mode = false"
   \return "void"
*/
void highlight_tile(int col, int row, unsigned int casilla, bool captured_mode = false){
    if((col <= 8 && row <= 8 && col > 0 && row > 0) && ((estadodeljuego == TURNO_BLANCAS && piezas_tablero[row-1][col-1] != WHITE) || (estadodeljuego == TURNO_NEGRAS && piezas_tablero[row-1][col-1] != BLACK))){
        casillas_resaltadas[casilla][0] = row;
        casillas_resaltadas[casilla][1] = col;
    }
}

/*!
   \brief "Funcion de comunicacion con la funcion anterior"
   \param "int col, int row, unsigned int casilla, bool captured_mode = false"
   \return "void"
*/
void highlight_tile_k(int col, int row, unsigned int casilla, bool captured_mode = false){
    highlight_tile(col, row, casilla, captured_mode);
}
