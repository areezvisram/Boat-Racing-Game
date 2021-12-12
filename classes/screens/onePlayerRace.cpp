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

#include <screens/onePlayerRace.h>
#include <helperFunctions.h>
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
#include <material.h>
#include <fileReader.h>
#include <map/map.h>
#include <map/wall.h>
#include <screens/startScreen.h>
#include <PPM.h>

Camera camera;
Boat boatRace;

float floor_ambient [4] ={ 1.0f, 1.0, 1.0f,1.0f };
float floor_diffuse [4] ={ 0.0, 0.0, 0.0, 1.0f };
float floor_specular [4] ={ 0.0, 0.0, 0.0, 1.0f };
float floor_shine = 0.0f;

// float wall_ambient [4] = {0.0f, 0.1f, 0.06f, 1.0f};
// float wall_diffuse[4] =  {0.0f, 0.50980392f, 0.50980392f, 1.0f};
// float wall_specular[4] =    {0.50196078f, 0.50196078f, 0.50196078f, 1.0f};
// float wall_shine = 10.0f;

FileReader floorReader = FileReader("map/floor.txt");
FileReader wallReader = FileReader("map/walls.txt");
Material floorMaterial = Material(floor_ambient, floor_diffuse, floor_specular, floor_shine);
Material wallMaterial = Material(floor_ambient, floor_diffuse, floor_specular, floor_shine);

std::vector<Floor> floors = floorReader.readFloorVertices(floorMaterial);
Floor loadingFloor = Floor();
Wall loadingWall = Wall();
std::vector<Wall> walls = wallReader.readWallVertices(wallMaterial);
Map map = Map(walls, floors);

bool keystates[256] = {false};
bool sKeystates[4] = {false};

// Global variables
float GLOBAL_WIDTH = 800;
float GLOBAL_HEIGHT = 800;

float lightPos[2][4] = {
    {-20, 200, -20, 1},
    {20, 200, 20, 1}};

float lightAmb[2][4] = {
    {0.1, 0.1, 0.1, 1},
    {0.1, 0.1, 0.1, 1}};


float floorAmb [4] = {0.05375f, 0.05f, 0.06625f, 0.82f};
float floorDiff [4] = {0.18275f, 0.17f, 0.22525f, 0.82f};
float floorSpec [4] = {0.332741f, 0.328634f, 0.346435f, 0.82f};

GLint onePlayerRace;
int globalMaterialIndexRace;

OnePlayerRaceScreen::OnePlayerRaceScreen()
{
    width = 800;
    height = 800;
    windowPosX = 0;
    windowPosY = 0;
    windowName = "Sample Window";
}

OnePlayerRaceScreen::OnePlayerRaceScreen(int width, int height, int windowPosX, int windowPosY, const char* windowName, int boatIndex, int materialIndex)
{
    this->width = width;
    this->height = height;
    this->windowPosX = windowPosX;
    this->windowPosY = windowPosY;
    this->windowName = windowName;
    this->boatIndex = boatIndex;
    this->materialIndex = materialIndex;
}

void OnePlayerRaceScreen::determineMaterial()
{
    globalMaterialIndexRace = materialIndex;
}

float red[4] = {1,0,0,1};
float yellow[4] = {1,1,0,1};
float blue[4] = {0,0,1,1};
float black[4] = {0,0,0,1};

void drawAxis()
{
    glPushMatrix();
    glLineWidth(2);
    glBegin(GL_LINES);

    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, red); // red x
    glColor3f (1.0, 1.0, 1.0);
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

// Display function for the save screen
void onePlayerRaceScreenDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    useCamera(boatRace);    

    glPushMatrix();    
    glPushMatrix();
        glTranslatef(boatRace.pos.x, boatRace.pos.y, boatRace.pos.z);
        glRotatef(boatRace.rot.x, 1,0,0);
        glRotatef(boatRace.rot.y, 0,1,0);
        glRotatef(boatRace.rot.z, 0,0,1);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, globalMaterials[globalMaterialIndexRace][0]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, globalMaterials[globalMaterialIndexRace][1]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, globalMaterials[globalMaterialIndexRace][2]);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10);
        boatRace.draw();
        drawAxis();
    glPopMatrix();

    glPushMatrix();
        map.render();
    glPopMatrix();    

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos[0] );
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb[0]);
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
    gluPerspective(45, (float)w / h, 1, 1000);

    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
}

void timer(int value)
{
    boatRace.update(sKeystates[1], sKeystates[3], sKeystates[0], sKeystates[2]);        
    glutSetWindow(onePlayerRace);
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

void init()
{
    glClearColor(0.6, 0.6, 0.6, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);    
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    // glEnable(GL_TEXTURE_2D);
}

void OnePlayerRaceScreen::createWindow()
{        
    boatRace = Boat(Point3D(200,10,0), Boat::BoatType(boatIndex), Vec3D(0, 180, 0), Camera(Point3D(-5, 2, 0), Vec3D::createVector(Point3D(-5, 0, 0), Point3D()), 45));
    glutInitWindowSize(width, height);
    glutInitWindowPosition(windowPosX, windowPosY);    
    onePlayerRace = glutCreateWindow(windowName);   
    init();
    // loadingFloor.loadTextures();    
    // loadingWall.loadTextures();
    map.loadTextures();
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialDown);
    glutSpecialUpFunc(specialUp);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutDisplayFunc(onePlayerRaceScreenDisplay);
    glutTimerFunc(17, timer, 0);    
}