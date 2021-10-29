#ifdef __APPLE__
#  define GL_SILENCE_DEPRECATION
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <windows.h>
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <fstream>
#include "classes/mathLib3D.h"
#include "classes/event.h"

// Global variables
float GLOBAL_WIDTH = 500;
float GLOBAL_HEIGHT = 500;

// Display function
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_POLYGON);
	glColor3f(0, 1, 0);
	glVertex2f(-0.5, -0.5);
	glVertex2f(0.5, -0.5);
	glVertex2f(0.5, 0.5);
	glVertex2f(-0.5, 0.5);
	glEnd();

    glFlush();
}

// Reshape function
void reshape(int w, int h) {    
    GLOBAL_WIDTH = w;
    GLOBAL_HEIGHT = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);    

    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);    
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key){
        case 'q':
            exit(0);
            break;
    }
}

void timer(int value)
{   
    glutPostRedisplay();
    glutTimerFunc(17, timer, 0);
}

void initGlut()
{
    glutInitWindowSize(GLOBAL_WIDTH, GLOBAL_HEIGHT);
    glutCreateWindow("Final Project"); 
    // glutReshapeFunc(reshape);
    glutKeyboardFunc(Event::keyboard);
    glutDisplayFunc(display); 
    glutTimerFunc(17, timer, 0);  
}

void init()
{
    initGlut();
}

// main function - entry of the program
int main(int argc, char** argv)
{                
    glutInit(&argc, argv);
    init();
    glutMainLoop();

    return (0);
}