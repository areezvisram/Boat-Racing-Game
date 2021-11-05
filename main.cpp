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
#include <mesh/mesh.h>
#include <object/upgrade.h>

// Global variables
float GLOBAL_WIDTH = 500;
float GLOBAL_HEIGHT = 500;

Mesh mesh;
Upgrade upgrade;

float lightPos[4] = {0, 10, 0, 1};
float lightAmb[4] = {0.2, 0.2, 0.2, 1};
float lightDiff[4] = {1, 1, 1, 1};
float lightSpec[4] = {1, 1, 1, 1};

// Display function
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);

    glPushMatrix();
    glRotated(upgrade.rot.x, 1,0,0);
    glRotated(upgrade.rot.y, 0,1,0);
    glRotated(upgrade.rot.z, 0,0,1);
    glColor3f(0.7, 0.1, 0);
    upgrade.draw();
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
    gluPerspective(45, (float)w / h, 1, 100);

    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
}

void timer(int value)
{
    upgrade.update();
    glutPostRedisplay();    
    glutTimerFunc(17, timer, 0);
}

void initGlut()
{
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);    
    glutInitWindowSize(GLOBAL_WIDTH, GLOBAL_HEIGHT);
    glutInitWindowPosition(2000, 50);    
    glutCreateWindow("Final Project");   
    glutReshapeFunc(reshape);
    glutKeyboardFunc(Event::keyboard);
    glutDisplayFunc(display);
    glutTimerFunc(17, timer, 0);    
}

void init()
{
    initGlut();    
    glClearColor(0.3, 0.3, 0.3, 0.3);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
}

// main function - entry of the program
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    init();

     upgrade = Upgrade(Point3D(), Mesh::createFromOBJ("obj/upgrade.obj"), Vec3D(0,1,0));
    //mesh = Mesh::createFromOBJ("obj/boat.obj");

    glutMainLoop();

    return (0);
}