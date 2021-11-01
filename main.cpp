#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#endif

#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <fstream>
#include <mathLib3D.h>
#include <event.h>
#include <mesh.h>

// Global variables
float GLOBAL_WIDTH = 500;
float GLOBAL_HEIGHT = 500;

// Display function
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);

    glPushMatrix();
    glColor3f(0.7, 0.1, 0);
    glutSolidCube(2);
    glPopMatrix();

    glutSwapBuffers();
}

// Reshape function
void reshape(int w, int h)
{
    GLOBAL_WIDTH = w;
    GLOBAL_HEIGHT = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);

    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
}

void timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(17, timer, 0);
}

void initGlut()
{
    glutInitWindowSize(GLOBAL_WIDTH, GLOBAL_HEIGHT);
    glutInitWindowPosition(50, 50);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Final Project");
    // glutReshapeFunc(reshape);
    glutKeyboardFunc(Event::keyboard);
    glutDisplayFunc(display);
    glutTimerFunc(17, timer, 0);

    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 100);
    glClearColor(0.3, 0.3, 0.3, 0.3);
}

void init()
{
    initGlut();
}

// main function - entry of the program
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    init();

    // Mesh::createFromOBJ("cube.obj");

    glutMainLoop();

    return (0);
}