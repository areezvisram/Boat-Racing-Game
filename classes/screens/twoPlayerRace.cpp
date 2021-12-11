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

#include <screens/twoPlayerRace.h>
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

// bool keystates2[256] = {false};
// bool sKeystates2[4] = {false};

Camera camera2;
// Mesh mesh;
// Upgrade upgrade;
Boat boatRace1, boatRace2;

float mat_ambient2 [4] ={ 0.329412f, 0.223529f, 0.027451f,1.0f };
float mat_diffuse2 [4] ={ 0.780392f, 0.568627f, 0.113725f, 1.0f };
float mat_specular2 [4] ={ 0.992157f, 0.941176f, 0.807843f, 1.0f };
float shine2 = 27.8974f;

float wall_ambient2 [4] = {0.0f, 0.1f, 0.06f, 1.0f};
float wall_diffuse2[4] =  {0.0f, 0.50980392f, 0.50980392f, 1.0f};
float wall_specular2[4] =    {0.50196078f, 0.50196078f, 0.50196078f, 1.0f};
float wall_shine2 = 10.0f;

FileReader floorReader2 = FileReader("map/floor.txt");
FileReader wallReader2 = FileReader("map/walls.txt");
Material floorMaterial2 = Material(mat_ambient2, mat_diffuse2, mat_specular2, shine2);
Material wallMaterial2 = Material(wall_ambient2, wall_diffuse2, wall_specular2, wall_shine2);

std::vector<Floor> test_floors2 = floorReader2.readFloorVertices(floorMaterial2);
std::vector<Wall> test_walls2 = wallReader2.readWallVertices(wallMaterial2);
Map map2 = Map(test_walls2, test_floors2);

bool keystates2[256] = {false};
bool sKeystates2[4] = {false};

// Global variables
float GLOBAL_WIDTH_2 = 800;
float GLOBAL_HEIGHT_2 = 800;
// float cameraX = 100;
// float cameraY = 600;
// float cameraZ = 0;

// float cameraDirX = 0;
// float cameraDirY = 50;
// float cameraDirZ = 0;

// bool showLights = false;

float lightPos2[2][4] = {
    {-20, 200, -20, 1},
    {20, 200, 20, 1}};

float lightAmb2[2][4] = {
    {0.1, 0.1, 0.1, 1},
    {0.1, 0.1, 0.1, 1}};

float lightEmis2[2][4] = {
    {1, 1, 1, 1},
    {1, 1, 1, 1}};

float floorAmb2 [4] = {0.05375f, 0.05f, 0.06625f, 0.82f};
float floorDiff2 [4] = {0.18275f, 0.17f, 0.22525f, 0.82f};
float floorSpec2 [4] = {0.332741f, 0.328634f, 0.346435f, 0.82f};

GLint twoPlayerRaceScreenOne, twoPlayerRaceScreenTwo;
int globalMaterialIndexRace1, globalMaterialIndexRace2;

TwoPlayerRaceScreen::TwoPlayerRaceScreen()
{
    width = 800;
    height = 800;
    windowPosX = 0;
    windowPosY = 0;
    windowName = "Sample Window";
}

TwoPlayerRaceScreen::TwoPlayerRaceScreen(int width, int height, int windowPosX, int windowPosY, const char* windowName, int boat1Index, int material1Index, int boat2Index, int material2Index)
{
    this->width = width;
    this->height = height;
    this->windowPosX = windowPosX;
    this->windowPosY = windowPosY;
    this->windowName = windowName;
    this->boat1Index = boat1Index;
    this->material1Index = material1Index;
    this->boat2Index = boat2Index;
    this->material2Index = material2Index;
}

void TwoPlayerRaceScreen::determineMaterial()
{
    globalMaterialIndexRace1 = material1Index;
    globalMaterialIndexRace2 = material2Index;
}

float red2[4] = {1,0,0,1};
float yellow2[4] = {1,1,0,1};
float blue2[4] = {0,0,1,1};
float black2[4] = {0,0,0,1};

void drawAxis2()
{
    glPushMatrix();
    glLineWidth(2);
    glBegin(GL_LINES);

    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, red2); // red x
    glColor3f (1.0, 1.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(5.0, 0.0, 0.0);

    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, yellow2); // yellow y
    glColor3f (1.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 5.0, 0.0);

    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blue2); // blue z
    glColor3f (0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 5.0);
    glEnd();
    glPopMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black2);
}

void useCamera2(Boat b)
{
    Point3D pos = b.pos;
    Vec3D backVec = b.forwardVector().normalize().multiply(b.camera.pos.x);
    Point3D camPos = backVec.add(Vec3D::createVector(Point3D(), Point3D(0,b.camera.pos.y,0))).movePoint(pos);

    gluLookAt(camPos.x, camPos.y, camPos.z, pos.x, pos.y, pos.z, 0,1,0);
}

// Display function for the save screen
void TwoPlayerRaceScreenDisplayOne()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //glViewport(0,0,GLOBAL_WIDTH/2, GLOBAL_HEIGHT);
    //glLoadIdentity();
    useCamera2(boatRace1);    

    glPushMatrix();    
    glPushMatrix();
        glTranslatef(boatRace1.pos.x, boatRace1.pos.y, boatRace1.pos.z);
        glRotatef(boatRace1.rot.x, 1,0,0);
        glRotatef(boatRace1.rot.y, 0,1,0);
        glRotatef(boatRace1.rot.z, 0,0,1);
        //glColor3f(0.7, 0.1, 0);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, globalMaterials[globalMaterialIndexRace1][0]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, globalMaterials[globalMaterialIndexRace1][1]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, globalMaterials[globalMaterialIndexRace1][2]);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10);
        boatRace1.draw();
        drawAxis2();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(boatRace2.pos.x, boatRace2.pos.y, boatRace2.pos.z);
        glRotatef(boatRace2.rot.x, 1,0,0);
        glRotatef(boatRace2.rot.y, 0,1,0);
        glRotatef(boatRace2.rot.z, 0,0,1);
        //glColor3f(0.7, 0.1, 0);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, globalMaterials[globalMaterialIndexRace2][0]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, globalMaterials[globalMaterialIndexRace2][1]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, globalMaterials[globalMaterialIndexRace2][2]);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10);
        boatRace2.draw();
        drawAxis2();
    glPopMatrix();


    glPushMatrix();
        map2.render();
    glPopMatrix();    

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos2[0] );
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb2[0]);
    glColor3f(1, 1, 1);

    glPopMatrix();

    glutSwapBuffers();
}

// Display function
void TwoPlayerRaceScreenDisplayTwo(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //glViewport(0,0,GLOBAL_WIDTH/2, GLOBAL_HEIGHT);
    //glLoadIdentity();
    useCamera2(boatRace2);    

    glPushMatrix();    
    glPushMatrix();
        glTranslatef(boatRace1.pos.x, boatRace1.pos.y, boatRace1.pos.z);
        glRotatef(boatRace1.rot.x, 1,0,0);
        glRotatef(boatRace1.rot.y, 0,1,0);
        glRotatef(boatRace1.rot.z, 0,0,1);
        //glColor3f(0.7, 0.1, 0);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, globalMaterials[globalMaterialIndexRace1][0]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, globalMaterials[globalMaterialIndexRace1][1]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, globalMaterials[globalMaterialIndexRace1][2]);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10);
        boatRace1.draw();
        drawAxis2();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(boatRace2.pos.x, boatRace2.pos.y, boatRace2.pos.z);
        glRotatef(boatRace2.rot.x, 1,0,0);
        glRotatef(boatRace2.rot.y, 0,1,0);
        glRotatef(boatRace2.rot.z, 0,0,1);
        //glColor3f(0.7, 0.1, 0);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, globalMaterials[globalMaterialIndexRace2][0]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, globalMaterials[globalMaterialIndexRace2][1]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, globalMaterials[globalMaterialIndexRace2][2]);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10);
        boatRace2.draw();
        drawAxis2();
    glPopMatrix();


    glPushMatrix();
        map2.render();
    glPopMatrix();  

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos2[0] );
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb2[0]);
    glColor3f(1, 1, 1);

    glPopMatrix();

    glutSwapBuffers();
}

// Reshape function
void reshape2(int w, int h)
{
    GLOBAL_WIDTH_2 = w;
    GLOBAL_HEIGHT_2 = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w / h, 1, 1000);

    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
}

 void timer2(int value)
{
    boatRace1.update(sKeystates2[1], sKeystates2[3], sKeystates2[0], sKeystates2[2]);    
    boatRace2.update(keystates2[(unsigned char)'w'], keystates2[(unsigned char)'s'], keystates2[(unsigned char)'a'], keystates2[(unsigned char)'d']);
    glutSetWindow(twoPlayerRaceScreenOne);
    glutPostRedisplay();    
    // glutSetWindow(windowId2);
    // //glutDisplayFunc(secondWindowDisplay);
    // glutPostRedisplay();
    // glutSetWindow(windowId1);
    // //glutDisplayFunc(display);
    //std::cout << "BOAT:" << boat.speed << " ";
    glutTimerFunc(17, timer2, 0);
}

void secondTimer(int value)
{
    boatRace1.update(sKeystates2[1], sKeystates2[3], sKeystates2[0], sKeystates2[2]);    
    boatRace2.update(keystates2[(unsigned char)'w'], keystates2[(unsigned char)'s'], keystates2[(unsigned char)'a'], keystates2[(unsigned char)'d']);
    glutSetWindow(twoPlayerRaceScreenTwo);
    glutPostRedisplay();    
    // glutSetWindow(windowId2);
    // //glutDisplayFunc(secondWindowDisplay);
    // glutPostRedisplay();
    // glutSetWindow(windowId1);
    // //glutDisplayFunc(display);    
    //std::cout << "BOAT 2: " << boat2.speed << "\n";
    glutTimerFunc(17, secondTimer, 0);
}

void keyDown2(unsigned char key, int x, int y)
{
    keystates2[tolower(key)] = true;
}

void keyUp2(unsigned char key, int x, int y)
{
    keystates2[tolower(key)] = false;
    switch (key)
    {
        case 'q':
            exit(0);
            break;        
    }
}

void specialDown2(int key, int x, int y)
{
    switch(key) {
        case GLUT_KEY_LEFT:
            sKeystates2[0] = true;
            break;
        case GLUT_KEY_UP:
            sKeystates2[1] = true;
            break;
        case GLUT_KEY_RIGHT:
            sKeystates2[2] = true;
            break;
        case GLUT_KEY_DOWN:
            sKeystates2[3] = true;
            break;            
    }
}

void specialUp2(int key, int x, int y)
{
    switch(key) {
        case GLUT_KEY_LEFT:
            sKeystates2[0] = false;
            break;
        case GLUT_KEY_UP:
            sKeystates2[1] = false;
            break;
        case GLUT_KEY_RIGHT:
            sKeystates2[2] = false;
            break;
        case GLUT_KEY_DOWN:
            sKeystates2[3] = false;
            break;            
    }
}

void init2()
{    
    glutSetWindow(twoPlayerRaceScreenTwo);
    glClearColor(0.6, 0.6, 0.6, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);    
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glutSetWindow(twoPlayerRaceScreenOne);
    glClearColor(0.6, 0.6, 0.6, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);    
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
}

int TwoPlayerRaceScreen::createWindow()
{        
    // boatRace = Boat(Point3D(200, 10, 0), Boat::SPEED, Vec3D(0, 180, 0), Camera(Point3D(-5, 2, 0), Vec3D::createVector(Point3D(-5, 0, 0), Point3D()), 45));
    boatRace1 = Boat(Point3D(200,10,-2), Boat::BoatType(boat1Index), Vec3D(0, 180, 0), Camera(Point3D(-5, 2, 0), Vec3D::createVector(Point3D(-5, 0, 0), Point3D()), 45));
    boatRace2 = Boat(Point3D(200,10,2), Boat::BoatType(boat2Index), Vec3D(0, 180, 0), Camera(Point3D(-5, 2, 0), Vec3D::createVector(Point3D(-5, 0, 0), Point3D()), 45));

    glutInitWindowSize(width, height);
    glutInitWindowPosition(windowPosX + 1000, windowPosY);    
    twoPlayerRaceScreenTwo = glutCreateWindow(windowName);       
    glutReshapeFunc(reshape2);
    // glutSpecialFunc(specialDown2);
    // glutSpecialUpFunc(specialUp);
    // glutKeyboardFunc(keyDown2);
    // glutKeyboardUpFunc(keyUp2);
    glutDisplayFunc(TwoPlayerRaceScreenDisplayTwo);
    glutTimerFunc(17, secondTimer, 0);   

    glutInitWindowSize(width, height);
    glutInitWindowPosition(windowPosX, windowPosY);    
    twoPlayerRaceScreenOne = glutCreateWindow(windowName);       
    glutReshapeFunc(reshape2);
    glutSpecialFunc(specialDown2);
    glutSpecialUpFunc(specialUp2);
    glutKeyboardFunc(keyDown2);
    glutKeyboardUpFunc(keyUp2);
    glutDisplayFunc(TwoPlayerRaceScreenDisplayOne);
    glutTimerFunc(17, timer2, 0);    

    init2();
}