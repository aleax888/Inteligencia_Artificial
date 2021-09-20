#include <windows.h>
#include <iostream>
#include <math.h>
#include "glut.h"
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <utility>
#include <algorithm>
#include <queue>
#include <stack>
#include <list>
#include "button.h"
#include "dfs.h"
#include "time.h"
#define KEY_ESC 27

using namespace std;

vector<point> generate_points(int num) { //Generar puntos
    float windowWidth = 0;
    float windowHeight = 0;
    int a = 600 / 25;
    glPointSize(5.0f);
    glColor3f(0.658824, 0.658824, 0.658824);
    vector<point> result(num * num);
    int j = 0;
    for (int h = a; h < (num * a + 1); h = h + a) {
        for (int i = a; i < (num * a + 1); i = i + a) {
            points[j].x = windowWidth - a / 2 + i;
            points[j].y = (windowHeight)-a / 2 + h;
            points[j].st = 1;
            glBegin(GL_POINTS);
            glVertex2d((windowWidth)-a / 2 + i, (windowHeight)-a / 2 + h);
            glEnd();
            j++;
        }
    }
    return result;
}
void generate_edges() { //Generar aristas/lines
    glColor3f(0.658824, 0.658824, 0.658824);
    int a = 600 / 25;
    for (int i = 0; i < (25 * 25); i++) {
        if (points[i].st) {
            if (points[i].y < 600 - a && points[i].x < 600 - a) {
                glLineWidth(1);
                glBegin(GL_LINES);
                if (points[i + 1].st) {
                    glVertex2f(points[i].x, points[i].y);
                    glVertex2f(points[i + 1].x, points[i + 1].y); //para un lado

                    graph[i].push_back(i + 1);
                    graph[i + 1].push_back(i);
                }
                if (points[i + 25].st) {
                    glVertex2f(points[i].x, points[i].y);
                    glVertex2f(points[i + 25].x, points[i + 25].y); //para arriba

                    graph[i].push_back(i + 25);
                    graph[i + 25].push_back(i);
                }
                if (points[i + 26].st) {
                    glVertex2f(points[i].x, points[i].y);// diagonal
                    glVertex2f(points[i + 26].x, points[i + 26].y);

                    graph[i].push_back(i + 26);
                    graph[i + 26].push_back(i);
                }
                if (points[i + 25].st && points[i + 1].st) {
                    glVertex2f(points[i + 25].x, points[i + 25].y);
                    glVertex2f(points[i + 1].x, points[i + 1].y);

                    graph[i + 25].push_back(i + 1);
                    graph[i + 1].push_back(i + 25);
                }
                glEnd();
            }
            else {
                if (points[i].y > 600 - a && points[i].y < 500 + a && points[i].x < 600 + a) {
                    glBegin(GL_LINES);
                    glVertex2f(points[i].x, points[i].y);
                    glVertex2f(points[i + 1].x, points[i + 1].y); //para un lado
                    glEnd();
                    graph[i].push_back(i + 1);
                    graph[i + 1].push_back(i);
                }
                if (points[i].x > 600 - a && points[i].y < 500 + a && points[i].x < 600 + a) {
                    glBegin(GL_LINES);
                    glVertex2f(points[i].x, points[i].y);
                    glVertex2f(points[i + 25].x, points[i + 25].y); //para arriba
                    graph[i].push_back(i + 25);
                    graph[i + 25].push_back(i);
                    glEnd();
                }
            }
        }
    }
}
void printDFS() {
    vector<int> prev, dist;
    if (!DFS(prev, dist)) { cout << "No path"; return; }
    vector<int> path;
    int temp = finish;
    path.push_back(temp);
    while (prev[temp] != -1) {
        path.push_back(prev[temp]);
        temp = prev[temp];
    }
    for (int i = path.size() - 2; i >= 0; i--) {
        glLineWidth(3);
        glBegin(GL_LINES);
        glColor3f(0.0, 0.3, 1.0);
        glVertex2f(points[path[i + 1]].x, points[path[i + 1]].y);
        glVertex2f(points[path[i]].x, points[path[i]].y);
        glEnd();
    }
}
void eliminar() { //Eliminar ruta, para crear nueva
    for (int i = 0; i < ptsDel.size(); i++) {
        points[ptsDel[i]].st = 0;
        glPointSize(20.0f);
        glColor3f(0, 0, 0);
        glDisable(GL_POINT_SMOOTH);
        glBegin(GL_POINTS);
        glVertex2f(points[ptsDel[i]].x, points[ptsDel[i]].y);
        glEnd();
    }
    generate_edges();
}
void displayGizmo() {
    generate_points(25);
    eliminar();
    generate_edges();
    printDFS();
    glColor3f(0.5, 1.0, 0.5);
    ///To draw points since start to finish
    glPointSize(15.0f);
    glDisable(GL_POINT_SMOOTH);
    glBegin(GL_POINTS);
    glVertex2f(points[start].x, points[start].y);
    glColor3f(1.0, 1.0, 0.0);
    glVertex2d(points[finish].x, points[finish].y);
    glEnd();

    glColor3f(0, 0, 0);
    Font(GLUT_BITMAP_8_BY_13, (char*)"S", points[start].x - 2, points[start].y + 5); //Inicio del camino
    Font(GLUT_BITMAP_8_BY_13, (char*)"E", points[finish].x - 2, points[finish].y + 5); //Final del camino
}
int click_point(int x, int y) {
    for (int i = 0; i < points.size(); i++) {
        if (x < points[i].x + 5 && x > points[i].x - 5 && y < points[i].y + 5 && y > points[i].y - 5)
            return i;
    }
    return 0;
}
int pt;
void MouseButton(int button, int state, int x, int y) {
    TheMouse.x = x;
    TheMouse.y = y;

    if (state == GLUT_DOWN) {
        TheMouse.xpress = x;
        TheMouse.ypress = y;
        switch (button) {
        case GLUT_LEFT_BUTTON:
            TheMouse.lmb = 1;
            pt = click_point(x, y);
            if (pt != 0) start = pt;
            ButtonPress(x, y);
            break;
        case GLUT_MIDDLE_BUTTON:
            TheMouse.mmb = 1;
            pt = click_point(x, y);
            if (pt != 0) ptsDel.push_back(pt);
            break;
        case GLUT_RIGHT_BUTTON:
            TheMouse.rmb = 1;
            pt = click_point(x, y);
            if (pt != 0) finish = pt;
            break;
        }
    }
    else {
        switch (button) {
        case GLUT_LEFT_BUTTON:
            TheMouse.lmb = 0;
            ButtonRelease(x, y);
            break;
        case GLUT_MIDDLE_BUTTON:
            TheMouse.mmb = 0;
            break;
        case GLUT_RIGHT_BUTTON:
            TheMouse.rmb = 0;
            break;
        }
    }
    glutPostRedisplay();
}
void MouseMotion(int x, int y){
    int dx = x - TheMouse.x;
    int dy = y - TheMouse.y;

    TheMouse.x = x;
    TheMouse.y = y;
    ButtonPassive(x, y);
    glutPostRedisplay();
}
void MousePassiveMotion(int x, int y) {
    int dx = x - TheMouse.x;
    int dy = y - TheMouse.y;

    TheMouse.x = x;
    TheMouse.y = y;
    ButtonPassive(x, y);
}
void init_GL(void) {
    glClearColor(5.0f, 5.0f, 5.0f, 5.0f); //(R, G, B, transparencia) en este caso un fondo negro
}
GLvoid window_redraw(GLsizei width, GLsizei height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 600, 600, 0, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
}
GLvoid window_key(unsigned char key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        TheMouse.mmb = 1;
        pt = click_point(x, y);
        if (pt != 0) ptsDel.push_back(pt);
        break;
    case KEY_ESC:
        exit(0);
        break;

    default:
        break;
    }
}
void glPaint(void) {
    glClear(GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    displayGizmo();
    glutSwapBuffers();
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600); //tamaño de la ventana
    glutInitWindowPosition(100, 100); //posicion de la ventana
    glutCreateWindow("DFS-Blind Search"); //titulo de la ventana
    init_GL(); //funcion de inicializacion de OpenGL

    glutDisplayFunc(glPaint);
    glutReshapeFunc(&window_redraw);
    // Callback del teclado
    glutKeyboardFunc(&window_key);
    glutMouseFunc(MouseButton);
    glutMotionFunc(MouseMotion);
    glutPassiveMotionFunc(MousePassiveMotion);

    glutMainLoop(); //bucle de rendering
    init_GL();
    return 0;
}
