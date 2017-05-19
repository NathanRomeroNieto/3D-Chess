#include "piezas.h"

float lpos[4] = {1,0.5,1,0};
int screen_width = 850, screen_height = 850;

//Some piece variables
Piece* board;
Piece* selected_piece = NULL;

//helper function to print grid 2d array variable
void print_grid_pieces(void){
    for(int row = 0; row <= 7; row++){
        for(int col = 0; col <= 7; col++){
            int element = piezas_tablero[row][col];
            if(element > 0){
                printf("[%i]",element);
            }else if(element == 0){
                printf("[0000]");
            }
        }
        printf("\n");
    }
    printf("\n");
}

void loadPieces(){
    piezas.push_back(new King("data/models/king.dae","data/textures/white_king.jpg",0,'e',1));
    piezas.push_back(new King("data/models/king.dae","data/textures/black_king.jpg",1,'e',8));
    piezas.push_back(new Queen("data/models/queen.dae","data/textures/white_queen.jpg",2,'d',1));
    piezas.push_back(new Queen("data/models/queen.dae","data/textures/black_queen.jpg",3,'d',8));

    for(int i = 0; i <= 1; i++){
        piezas.push_back(new Rook("data/models/rook.dae","data/textures/white_rook.jpg",4,columna[i*7],1));           //white rooks
        piezas.push_back(new Rook("data/models/rook.dae","data/textures/black_rook.jpg",5,columna[i*7],8));           //black rooks
        piezas.push_back(new Bishop("data/models/bishop.dae","data/textures/white_bishop.jpg",6,columna[(i*3)+2],1)); //white bishops
        piezas.push_back(new Bishop("data/models/bishop.dae","data/textures/black_bishop.jpg",7,columna[(i*3)+2],8)); //black bishops
        piezas.push_back(new Knight("data/models/knight.dae","data/textures/white_knight.jpg",8,columna[(i*5)+1],1)); //white knights
        piezas.push_back(new Knight("data/models/knight.dae","data/textures/black_knight.jpg",9,columna[(i*5)+1],8)); //black knights
    }
    for(int i = 0; i <= 7; i++){
        piezas.push_back(new Pawn("data/models/pawn.dae","data/textures/white_pawn.jpg",10,columna[i],2));            //white pawns
        piezas.push_back(new Pawn("data/models/pawn.dae","data/textures/black_pawn.jpg",11,columna[i],7));            //black pawns
    }
    board = new Piece("data/models/board.dae","data/textures/board.jpg",12,'z',0);                                   //chess board
    printf("loaded starting piezas\n");
}

void drawBoard(){
    board->draw();
}

void drawPieces(){
    for(int i = 0; i <= piezas.size()-1; i++){ //draw piezas on the board
        glLoadName(1+i);
        piezas.at(i)->draw();
    }
    int white_counter = 0, black_counter = 0;
    for(int j = 0; j < piezas_comidas.size(); j++){ //draw dead piezas on the side
        Piece* temp = piezas_comidas.at(j);
        if(temp->color == BLACK){
            if(black_counter <= 7){	//front row
                temp->draw(true, -11.3, 8-(2*black_counter));
            }else{			//back row
                temp->draw(true, -13.3, 8-(2*(black_counter-8)));
            }
            black_counter++;
        }else if(temp->color == WHITE){
            if(white_counter <= 7){	//front row
                temp->draw(true, 11.2, 8-(2*white_counter));
            }else{			//back row
                temp->draw(true, 13.2, 8-(2*(white_counter-8)));
            }
            white_counter++;
        }
    }
}

void draw(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0,25,15,
    0,0,0,
    0,1,0);
    glLightfv(GL_LIGHT0,GL_POSITION,lpos);

    glLoadName(99);
    drawBoard();
    drawGrid();
    drawPieces();

    glutSwapBuffers();
}

void resize(int w, int h){
    if(h == 0)
        h = 1;
    float ratio = w*1.0f/h;
    screen_width = w;
    screen_height = h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0,0,w,h);
    gluPerspective(45,ratio,1,1000);
    glMatrixMode(GL_MODELVIEW);
}

void keyEvents(unsigned char key, int x, int y){
    switch(key){
        case 27: //escape key
            glDetachShader(p,v);
            glDetachShader(p,f);
            glDeleteShader(v);
            glDeleteShader(f);
            glDeleteProgram(p);
            for(int i = 0; i < piezas.size(); i++){
                free(piezas.at(i));
            }
            printf("shaders freed\n");
            exit(0);
            break;
    }

}

void specialKeys(int key, int x, int y){
    //no events to handle here yet...
}

void initGL(void){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(10);
}

//calls pick function on the piece at the position specified
void pickPiece(int col, int row){
    Piece* piece = pieza_en(col,row);
    if(piece != NULL){
        if((piece->color == BLACK && estadodeljuego == TURNO_NEGRAS) || (piece->color == WHITE && estadodeljuego == TURNO_BLANCAS)){
            for(int j = 0; j <= piezas.size()-1; j++){
                piezas.at(j)->unpick();
            }
            piece->pick();
            selected_piece = piece;
        }
    }
}

//swaps turns from white to black or black to white
void swap_turns(void){
    if(estadodeljuego == TURNO_BLANCAS){
        estadodeljuego = TURNO_NEGRAS;
    }else if(estadodeljuego == TURNO_NEGRAS){
        estadodeljuego = TURNO_BLANCAS;
    }
}

//processes which piece is clicked and which casilla is clicked
void list_hits(GLint hits, GLuint *names){
    for(int i = 0; i <= piezas.size()-1; i++){
        piezas.at(i)->unpick();
    }
    bool move_pressed = false;
    for (int i = 0; i < hits; i++){
        int name = (GLubyte)names[i*4+3];
        if(name == 165){
            move_pressed = true;
            break;
        }
        if(name >= 101){
            name -= 100;
            fila_tablero = 9-(name%8);
            if(fila_tablero == 9)
                fila_tablero = 1;
            columna_tablero = ceil((float)name/8.0f);
            columna_tablero_c = columna[columna_tablero-1];
            printf("row = %i col = %i name = %i\n",fila_tablero,columna_tablero,name);
        }
    }
    if(move_pressed){
        if((piezas_tablero[fila_tablero-1][columna_tablero-1] == BLACK && estadodeljuego == TURNO_BLANCAS) || (piezas_tablero[fila_tablero-1][columna_tablero-1] == WHITE && estadodeljuego == TURNO_NEGRAS)) //if the possible move is a capture move
            remover_pieza(columna_tablero,fila_tablero); //... then remove the piece that's eaten
        selected_piece->move((unsigned int)columna_tablero,(unsigned int)fila_tablero);
        swap_turns();
        clearMovesList();
        selected_piece->unpick();
    }
    pickPiece(columna_tablero,fila_tablero);
}

//sets up gl select mode
void gl_select(int x, int y){
    GLuint buff[64] = {0};
    GLint hits, view[4];

    glSelectBuffer(64, buff);

    glGetIntegerv(GL_VIEWPORT, view);

    glRenderMode(GL_SELECT);

    glInitNames();

    glPushName(0);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPickMatrix(x,y,1.0,1.0,view);
    gluPerspective(45,1.0,0.0001,1000.0);

    glMatrixMode(GL_MODELVIEW);

    glutSwapBuffers();
    draw();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    hits = glRenderMode(GL_RENDER);

    list_hits(hits,buff);

    glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y){
    if(state == GLUT_DOWN){
        gl_select(x,screen_height-y);
    }
}

int main(int argc, char **argv){

    glutInit(&argc,argv);
    glutInitWindowPosition(-1,-1); //starts at the default position
    glutInitWindowSize(screen_width,screen_height);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitContextVersion(2, 1);
    glutCreateWindow("Chess 3D");

    glewExperimental = GL_TRUE;
    glewInit();
    initGL();
    initDLs();
    loadPieces();
    estadodeljuego = TURNO_BLANCAS; //white starts first

    glClearColor(0.8, 0.8, 0.8, 1);
    glutDisplayFunc(draw);
    glutIdleFunc(draw);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyEvents);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);

    glutMainLoop();

    return 1;
}
