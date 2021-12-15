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
#include <chrono>

Camera camera;
Boat boatRace;
// MiniMap miniMapScreen;

// std::chrono::steady_clock raceTimer;
auto begin = std::chrono::steady_clock::now();
//std::chrono::duration<double> time_duration;
std::string duration;

float floor_ambient [4] ={ 1.0f, 1.0, 1.0f,1.0f };
float floor_diffuse [4] ={ 0.0, 0.0, 0.0, 1.0f };
float floor_specular [4] ={ 0.0, 0.0, 0.0, 1.0f };
float floor_shine = 0.0f;

float mini_ambient [4] ={ 1.0f, 1.0, 1.0f,1.0f };
float mini_diffuse [4] ={ 0.780392f, 0.568627f, 0.113725f, 1.0f };
float mini_specular [4] ={ 0.0, 0.0, 0.0, 1.0f };
float mini_shine = 0.0f;

float plane_amb[4] = {0.1745f, 0.01175f, 0.01175f, 0.55f};
float plane_diff[4] = {0.61424f, 0.04136f, 0.04136f, 0.55f};
float plane_spec[4] = {0.727811f, 0.626959f, 0.626959f, 0.55f};
float plane_shine = 10.0f;

float cameraX = 5;
float cameraY = 500;
float cameraZ = 0;

float cameraDirX = 0;
float cameraDirY = 50;
float cameraDirZ = 0;

FileReader floorReader = FileReader("map/floor.txt");
FileReader wallReader = FileReader("map/walls.txt");
FileReader racePlaneReader = FileReader("map/racePlanes.txt");
Material floorMaterial = Material(floor_ambient, floor_diffuse, floor_specular, floor_shine);
Material wallMaterial = Material(floor_ambient, floor_diffuse, floor_specular, floor_shine);
Material racePlaneMaterial = Material(plane_amb, plane_diff, plane_spec, plane_shine);

Material miniMapMaterial = Material(mini_ambient, mini_diffuse, mini_specular, mini_shine);

std::vector<Floor> floors = floorReader.readFloorVertices(floorMaterial);
std::vector<Wall> walls = wallReader.readWallVertices(wallMaterial);
std::vector<RacePlane> racePlanes = racePlaneReader.readRacePlaneVertices(racePlaneMaterial);
Map map = Map(walls, floors, racePlanes);

std::vector<Floor> miniMapFloors = floorReader.readFloorVertices(miniMapMaterial);
std::vector<Wall> miniMapWalls;
std::vector<RacePlane> miniPlanes;
Map miniMap = Map(miniMapWalls, miniMapFloors, miniPlanes);

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
int globalMaterialIndexRace, boatIndexRace;
bool raceComplete = false;

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
    boatIndexRace = boatIndex;
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

    glScissor(0,0,800,800);
    glViewport(0,0,800, 800);
    glLoadIdentity();

    useCamera(boatRace);    

    glPushMatrix();    

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos[0] );
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb[0]);
    glColor3f(1, 1, 1);
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
    glClear(GL_DEPTH_BUFFER_BIT);

    if(raceComplete)
    {    
        renderText(-0.50, 0.65, GLUT_STROKE_ROMAN, "Race Complete", 0.001);
        renderText(-0.35, 0.50, GLUT_STROKE_ROMAN, "Your Time:", 0.001);
        renderText(-0.35, 0.35, GLUT_STROKE_ROMAN, duration.c_str(), 0.001);
        renderText(-0.45, 0.20, GLUT_STROKE_ROMAN, "Press r to restart the race", 0.0005);
        renderText(-0.45, 0.05, GLUT_STROKE_ROMAN, "Press q to quit the program", 0.0005);
    }
        
    glViewport(600, 600, 200, 200);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, cameraDirX, cameraDirY, cameraDirZ, 0, 1, 0);

    glPushMatrix();
        glTranslatef(boatRace.pos.x, boatRace.pos.y, boatRace.pos.z);
        glutSolidSphere(10,16,16);
    glPopMatrix();

    glPushMatrix();
    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);
        miniMap.render();
    glPopMatrix();  
    glEnable(GL_LIGHTING);        

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
        case 'r':
            if(raceComplete)
            {
                glutDestroyWindow(onePlayerRace);
                begin = std::chrono::steady_clock::now();
                OnePlayerRaceScreen newScreen = OnePlayerRaceScreen(800, 800, 2000, 50, "Player 1", boatIndexRace, globalMaterialIndexRace);    
                newScreen.determineMaterial();
                newScreen.createWindow();
            }            
        case 't':
            raceComplete = !raceComplete;                                             
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed = end - begin;
            auto x = std::chrono::duration_cast<std::chrono::seconds>(elapsed);
            duration = std::to_string(x.count());
            duration += " seconds";
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
    glEnable(GL_SCISSOR_TEST);
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