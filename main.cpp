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
#include <camera.h>
#include <mesh/mesh.h>
#include <object/upgrade.h>
#include <object/boat.h>
#include <map/floor.h>

using std::cout;
using std::endl;

float cameraX = 100;
float cameraY = 600;
float cameraZ = 0;

float cameraDirX = 0;
float cameraDirY = 50;
float cameraDirZ = 0;

// Global variables
float GLOBAL_WIDTH = 800;
float GLOBAL_HEIGHT = 800;

Camera camera;
Mesh mesh;
Upgrade upgrade;
Boat boat;

std::vector<Vec3D> test_floor_vert = {
    Vec3D(200, 10, 5),
    Vec3D(200, 10, -5),
    Vec3D(150, 10, -5),
    Vec3D(140, 10, 5)
};

Floor testFloor = Floor(test_floor_vert);

bool keystates[256] = {false};
bool sKeystates[4] = {false};

float lightPos[4] = {-20, 200, -20, 1};
float lightAmb[4] = {0.1, 0.1, 0.1, 1};
float lightDiff[4] = {1, 1, 1, 1};
float lightSpec[4] = {1, 1, 1, 1};

float red[4] = {1,0,0,1};
float yellow[4] = {1,1,0,1};
float blue[4] = {0,0,1,1};
float black[4] = {0,0,0,1};

float floorAmb [4] = {0.05375f, 0.05f, 0.06625f, 0.82f};
float floorDiff [4] = {0.18275f, 0.17f, 0.22525f, 0.82f};
float floorSpec [4] = {0.332741f, 0.328634f, 0.346435f, 0.82f};

void drawFloor()
{
    glPushMatrix();
        glBegin(GL_POLYGON);
        glColor3f(0.3, 0.3, 1);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, floorAmb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, floorDiff);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, floorSpec);
        glMaterialf(GL_FRONT_AND_BACK, GL_SPECULAR, 10);
        glNormal3f(0, 1, 0);
        glVertex3f(200, 0, 200);
        glVertex3f(200, 0, -200);
        glVertex3f(-200, 0, -200);
        glVertex3f(-200, 0, 200);
        glEnd();
    glPopMatrix();
}

void drawAxis(){
    glPushMatrix();
    glLineWidth(2);
    glBegin(GL_LINES);

    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, red); // red x
    glColor3f (1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(5.0, 0.0, 0.0);

    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, yellow); // yellow y
    glColor3f (1.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 5.0, 0.0);

    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blue); // blue z
    glColor3f (0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 5.0);
    glEnd();
    glPopMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
}

void useCamera(Boat b)
{
    Point3D pos = b.pos;
    Vec3D backVec = b.forwardVector().normalize().multiply(b.camera.pos.x);
    Point3D camPos = backVec.add(Vec3D::createVector(Point3D(), Point3D(0,b.camera.pos.y,0))).movePoint(pos);

    gluLookAt(camPos.x, camPos.y, camPos.z, pos.x, pos.y, pos.z, 0,1,0);
}

// Display function
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //useCamera(boat);
    //gluLookAt(-5, 20, 5, 0, 0, 0, 0, 1, 0);    
    gluLookAt(cameraX, cameraY, cameraZ, cameraDirX, cameraDirY, cameraDirZ, 0, 1, 0);
    
    glPushMatrix();

    glPushMatrix();
        drawFloor();
    glPopMatrix();

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glColor3f(1, 1, 1);

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
    boat.update(sKeystates[1], sKeystates[3], sKeystates[0], sKeystates[2]);
    glutPostRedisplay();    
    glutTimerFunc(17, timer, 0);
}

void keyDown(unsigned char key, int x, int y)
{
    keystates[tolower(key)] = true;
}

void keyUp(unsigned char key, int x, int y)
{
    keystates[tolower(key)] = false;
    switch (key)
    {
        case 'q':
            exit(0);
            break;
    }
}

void specialDown(int key, int x, int y)
{
    switch(key) {
        case GLUT_KEY_LEFT:
            sKeystates[0] = true;
            break;
        case GLUT_KEY_UP:
            sKeystates[1] = true;
            break;
        case GLUT_KEY_RIGHT:
            sKeystates[2] = true;
            break;
        case GLUT_KEY_DOWN:
            sKeystates[3] = true;
            break;            
    }
}

void specialUp(int key, int x, int y)
{
    switch(key) {
        case GLUT_KEY_LEFT:
            sKeystates[0] = false;
            break;
        case GLUT_KEY_UP:
            sKeystates[1] = false;
            break;
        case GLUT_KEY_RIGHT:
            sKeystates[2] = false;
            break;
        case GLUT_KEY_DOWN:
            sKeystates[3] = false;
            break;            
    }
}

void initGlut()
{
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);    
    glutInitWindowSize(GLOBAL_WIDTH, GLOBAL_HEIGHT);
    glutInitWindowPosition(50, 50);    
    glutCreateWindow("Final Project");   
    glutReshapeFunc(reshape);
    //glutSpecialFunc(specialDown);
    //glutSpecialUpFunc(specialUp);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutDisplayFunc(display);
    //glutTimerFunc(17, timer, 0);
    // glutFullScreen();
}

void init()
{
    initGlut();
    glClearColor(0.6, 0.6, 0.6, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
}

// main function - entry of the program
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    init();

    //upgrade = Upgrade(Point3D(), Mesh::createFromOBJ("obj/upgrade.obj"), Vec3D(0,1,0));
    //boat = Boat(Point3D(0, 0, 0), Mesh::createFromOBJ("obj/boat2.obj"), Vec3D(0, 0, 0), 100, Camera(Point3D(-10, 5, 0), Vec3D::createVector(Point3D(-5, 10, 0), Point3D()), 45));
    //mesh = Mesh::createFromOBJ("obj/boat.obj");

    glutMainLoop();

    return (0);
}