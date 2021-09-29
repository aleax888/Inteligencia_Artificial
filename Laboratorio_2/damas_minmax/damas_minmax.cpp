#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
//#include <unistd.h>
#include <iostream>
#include <math.h>
#include "glut.h"
#include "Minimax.h"
//#include "button_funs.h"
#define KEY_ESC 27
#define KEY_NEXT 9
using namespace std;
Minimax* AI;
bool player = true;

checkerboard tableroD(8);
int level = 3;
void token_upload(bool d) {
    tokens.clear();
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (tableroD[i][j] != 0) {
                if (tableroD[i][j] == '1') { ///rojo
                    //if(i == 7) { fichas.push_back(new Fichita(j,i,0,1));}
                    tokens.push_back(new token(j, i, 0));
                }
                else if (tableroD[i][j] == '2') { ///negro
                  //  if(i == 0) { fichas.push_back(new Fichita(j,i,1,1));}
                    tokens.push_back(new token(j, i, 1));
                }
            }
        }
    }
}

void draw_token() {
    for (size_t i = 0; i < tokens.size(); ++i) {
        glPushMatrix();
        glPointSize(150.0f);
        glEnable(GL_POINT_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        if (tokens[i]->id && tokens[i]->dama == 0)
            glColor3f(0.4f, 0.4f, 0.4f);
        else if (tokens[i]->id && tokens[i]->dama == 1)
            glColor3f(0.0f, 0.0f, 0.0f);
        else if (tokens[i]->id == 0 && tokens[i]->dama == 1)
            glColor3f(1.0f, 0.0f, 0.0f);
        else
            glColor3f(0.7f, 0.0f, 0.0f);

        glBegin(GL_POINTS);
        glVertex2i(((tokens[i]->x_position) * 75) - 300 + 37, ((tokens[i]->y_position) * 75) - 300 + 37);
        glEnd();
        glPopMatrix();
    }
}

int l = 75;
void draw_checkerboard() {
    int x = -300, y = 300, X = 300, Y = -300;
    for (int f = 0; f < 8; ++f) {
        for (int c = 0; c < 8; ++c) {
            if ((f + c + 1) % 2 == 0) {
                glColor3f(0.4f, 0.25f, 0.04f);
                glBegin(GL_QUADS);
                glVertex2i(x, y);
                glVertex2i(x, y - l);
                glVertex2i(x + l, y - l);
                glVertex2i(x + l, y);
                glEnd();
            }
            else {
                glColor3f(1, 1, 1);
                glBegin(GL_QUADS);
                glVertex2i(x, y);
                glVertex2i(x, y - l);
                glVertex2i(x + l, y - l);
                glVertex2i(x + l, y);
                glEnd();
            }
            x += l;
        }
        y -= l;
        x = -300;
    }
}

void init_checkerboard()
{
    for (int i = 0; i < 8; ++i) {
        tableroD[i].assign(8, '0');
    }
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 3; ++j) {
            if ((i + j) % 2) {
                tableroD[j][i] = '1';
            }
        }
    }
    for (int i = 0; i < 8; ++i) {
        for (int j = 5; j < 8; ++j) {
            if ((i + j) % 2) {
                tableroD[j][i] = '2';
            }
        }
    }
    token_upload(0);
}

void play_AI()
{
    AI = new Minimax(tableroD, 0, level, 1);
    tableroD = AI->play();

    token_upload(0);
    if (tokens.size() == 0) cout << "GANO IA" << endl;
    turn = 0;
}
void glPaint(void) {
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glPushMatrix();
    glColor3f(0.3f, 0.15f, 0.03f);
    glLineWidth(18.0f);
    glBegin(GL_LINES);
    glVertex2i(-300, 309); glVertex2i(-300, -309);
    glVertex2i(-300, -300); glVertex2i(300, -300);
    glVertex2i(300, -309); glVertex2i(300, 309);
    glVertex2i(300, 300); glVertex2i(-300, 300);
    glEnd();
    glPopMatrix();
    draw_checkerboard();
    draw_token();


    game_over();
    glutSwapBuffers();
    glutPostRedisplay();
}

void adjust_coord(int& x, int& y) {
    if (x >= 400) {
        x = x - 400;
    }
    else {
        x = (400 - x) * (-1);
    }
    if (y <= 400) {
        y = 400 - y;
    }
    else {
        y = (y - 400) * (-1);
    }
}

float euclidean_distance(int x, int y, int a, int b) {
    return sqrt(pow((x - a), 2) + pow((y - b), 2));
}

int find_token(int x, int y) {
    int cx;
    int cy;
    for (int i = 0; i < tokens.size(); ++i) {
        int cx = tokens[i]->x_position * 75 - 300 + 40;
        int cy = tokens[i]->y_position * 75 - 300 + 40;
        if (euclidean_distance(x, y, cx, cy) < 42) {
            return i;
        }
    }
    return -1;
}

void capture(int cx, int cy, int& x, int& y, bool d) {
    int mx = (cx + x) / 2, my = (cy + y) / 2;
    int mx_d = (cx - x) / 2, my_d = (cy + y) / 2;
    if (tableroD[my][mx] == '2') {
        tableroD[my][mx] = '0';
        tableroD[cy][cx] = '0';
        tableroD[y][x] = '1';
    }
}


int position_token = -1;
void OnMouseClick(int button, int state, int x, int y) {
    adjust_coord(x, y);
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

        int pt = find_token(x, y);
        if (pt != -1) {
            position_token = pt;
        }
        else
        {
            x += 300; x /= 75;
            y += 300; y /= 75;
            int cx = tokens[position_token]->x_position;
            int cy = tokens[position_token]->y_position;
            bool d = tokens[position_token]->dama;
            if (d == 1)
            {

                if (x != cx && y != cy && y + 1 == cy && (x + 1 == cx || x - 1 == cx)) {
                    tableroD[cy][cx] = '0';
                    tableroD[y][x] = '1';
                    tokens[position_token]->x_position = x;
                    tokens[position_token]->y_position = y;
                    token_upload(tokens[position_token]->dama);
                    turn = 1;
                }
                if (x != cx && y != cy && y - 1 == cy && (x + 1 == cx || x - 1 == cx)) {
                    tableroD[cy][cx] = '0';
                    tableroD[y][x] = '1';
                    tokens[position_token]->x_position = x;
                    tokens[position_token]->y_position = y;
                    token_upload(tokens[position_token]->dama);
                    turn = 1;
                }
                if (x != cx && y != cy && y - 2 == cy && (x - 2 == cx || x + 2 == cx)) {
                    capture(cx, cy, x, y, tokens[position_token]->dama);
                    player1++;
                    tokens[position_token]->x_position = x;
                    tokens[position_token]->y_position = y;
                    token_upload(tokens[position_token]->dama);
                    turn = 1;
                }
            }
            else
            {
                if (x != cx && y != cy && y - 1 == cy && (x - 1 == cx || x + 1 == cx)) {
                    tableroD[cy][cx] = '0';
                    tableroD[y][x] = '1';
                    
                    tokens[position_token]->x_position = x;
                    tokens[position_token]->y_position = y;
                    token_upload(tokens[position_token]->dama);
                    turn = 1;
                }
                if (x != cx && y != cy && y - 2 == cy && (x - 2 == cx || x + 2 == cx)) {
                    capture(cx, cy, x, y, tokens[position_token]->dama);
                    player1++;
                    tokens[position_token]->x_position = x;
                    tokens[position_token]->y_position = y;
                    token_upload(tokens[position_token]->dama);
                    turn = 1;
                }
                if (x != cx && y != cy && y + 2 == cy && (x - 2 == cx || x + 2 == cx)) {
                    capture(cx, cy, x, y, d);
                    player1++;
                    tokens[position_token]->x_position = x;
                    tokens[position_token]->y_position = y;
                    token_upload(tokens[position_token]->dama);
                    turn = 1;
                }
            }

        }
        if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {

        }
    }
}

void init_GL(void) 
{
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glOrtho(-400.0f, 400.0f, -400.0f, 400.0f, -1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}
GLvoid window_redraw(GLsizei width, GLsizei height) 
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

GLvoid window_key(unsigned char key, int x, int y) 
{
    switch (key) {
    case KEY_ESC:
        exit(0);
        break;
    default: {
        if (turn == 1)
        {
            //Sleep(1000);
            play_AI();
            turn = 0;
        }
        break;
    }
    }
}

void print_window(GLdouble x, GLdouble y, const std::string& string)
{
    glColor3d(0.0, 1.0, 0.0);
    //GLUT_BITMAP_TIMES_ROMAN_24
    glRasterPos2d(x, y);
    for (int n = 0; n < string.size(); ++n) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[n]);
    }
}

int main(int argc, char** argv) {


    init_checkerboard();
    //Inicializacion de la GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800); //tamaño de la ventana
    glutInitWindowPosition(10, 10); //posicion de la ventana
    glutCreateWindow("DAMAS"); //titulo de la ventana


    init_GL(); //funcion de inicializacion de OpenGL

    glutDisplayFunc(glPaint);
    glutReshapeFunc(&window_redraw);
    // Callback del teclado
    glutKeyboardFunc(&window_key);
    glutMouseFunc(&OnMouseClick);

    glutMainLoop(); //bucle de rendering
    return 0;
}