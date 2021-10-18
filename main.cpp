#ifdef __APPLE__
#  define GL_SILENCE_DEPRECATION
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <iostream>
#include <random>

// Global variables
float GLOBAL_WIDTH = 800;
float GLOBAL_HEIGHT = 800;

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

// main function - entry of the program
int main(int argc, char** argv)
{                
    glutInit(&argc, argv);
    glutInitWindowSize(GLOBAL_WIDTH, GLOBAL_HEIGHT);
    glutCreateWindow("Final Project"); 
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);   
    glutMainLoop();

    return (0);
}