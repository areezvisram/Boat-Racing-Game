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
#include <object/boat.h>

// Global variables
float GLOBAL_WIDTH = 800;
float GLOBAL_HEIGHT = 800;

Mesh mesh;
Upgrade upgrade;
Boat boat;

float lightPos[4] = {0, 10, 0, 1};
float lightAmb[4] = {0.2, 0.2, 0.2, 1};
float lightDiff[4] = {1, 1, 1, 1};
float lightSpec[4] = {1, 1, 1, 1};

void drawAxis(){
    glPushMatrix();
    glLineWidth(2);
    glBegin(GL_LINES);

    glColor3f (1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(5.0, 0.0, 0.0);

    glColor3f (1.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 5.0, 0.0);

    glColor3f (0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 5.0);
    glEnd();
    glPopMatrix();
}

// Display function
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(10, 10, 0, 0, 0, 0, 0, 1, 0);    

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);

    drawAxis();

    glPushMatrix();
    glTranslatef(boat.pos.x, boat.pos.y, boat.pos.z);
    glRotated(boat.rot.x, 1,0,0);
    glRotated(boat.rot.y, 0,1,0);
    glRotated(boat.rot.z, 0,0,1);
    glColor3f(0.7, 0.1, 0);
    boat.draw();
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
    boat.update();
    //std::cout << boat.pos.x << ", " << boat.pos.y << ", " << boat.pos.z << "\n";
    glutPostRedisplay();    
    glutTimerFunc(17, timer, 0);
}

void special(int key, int x, int y)
{
    switch(key) {
        case GLUT_KEY_RIGHT:
            boat.pos.z -= 0.5;
            break;
        case GLUT_KEY_LEFT:
            boat.pos.z += 0.5;
            break;
        case GLUT_KEY_UP:
            boat.pos.x -= 0.5;
            break;
        case GLUT_KEY_DOWN:
            boat.pos.x += 0.5;
            break;            
    }
}

void initGlut()
{
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);    
    glutInitWindowSize(GLOBAL_WIDTH, GLOBAL_HEIGHT);
    glutInitWindowPosition(2000, 50);    
    glutCreateWindow("Final Project");   
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
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

    //upgrade = Upgrade(Point3D(), Mesh::createFromOBJ("obj/upgrade.obj"), Vec3D(0,1,0));
    boat = Boat(Point3D(0, 0, 0), Mesh::createFromOBJ("obj/boat.obj"), Vec3D(0,1,0), Vec3D(0, 0, 0), 0.005);
    //mesh = Mesh::createFromOBJ("obj/boat.obj");

    glutMainLoop();

    return (0);
}