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
#include <map/racePlane.h>
#include <screens/startScreen.h>
#include <directionAngle.h>

// Globals

Camera camera2;
Boat boatRace1, boatRace2;

float floor_ambient_2 [4] ={ 1.0f, 1.0, 1.0f,1.0f };
float floor_diffuse_2 [4] ={ 0.0, 0.0, 0.0, 1.0f };
float floor_specular_2 [4] ={ 0.0, 0.0, 0.0, 1.0f };
float floor_shine_2 = 0.0f;

float plane_amb2[4] = {0.1745f, 0.01175f, 0.01175f, 0.55f};
float plane_diff2[4] = {0.61424f, 0.04136f, 0.04136f, 0.55f};
float plane_spec2[4] = {0.727811f, 0.626959f, 0.626959f, 0.55f};
float plane_shine2 = 10.0f;

float mini_ambient2 [4] ={ 1.0f, 1.0, 1.0f,1.0f };
float mini_diffuse2 [4] ={ 0.780392f, 0.568627f, 0.113725f, 1.0f };
float mini_specular2 [4] ={ 0.0, 0.0, 0.0, 1.0f };
float mini_shine2 = 0.0f;

float cameraX2 = 5;
float cameraY2 = 500;
float cameraZ2 = 0;

float cameraDirX2 = 0;
float cameraDirY2 = 50;
float cameraDirZ2 = 0;

FileReader floorReader2 = FileReader("map/floor.txt");
FileReader wallReader2 = FileReader("map/walls.txt");
FileReader racePlaneReader2 = FileReader("map/racePlanes.txt");
Material floorMaterial2 = Material(floor_ambient_2, floor_diffuse_2, floor_specular_2, floor_shine_2);
Material wallMaterial2 = Material(floor_ambient_2, floor_diffuse_2, floor_specular_2, floor_shine_2);
Material racePlaneMaterial2 = Material(plane_amb2, plane_diff2, plane_spec2, plane_shine2);
Material miniMapMaterial2 = Material(mini_ambient2, mini_diffuse2, mini_specular2, mini_shine2);


std::vector<Floor> miniMapFloors2 = floorReader2.readFloorVertices(miniMapMaterial2);
std::vector<Wall> miniMapWalls2;
std::vector<RacePlane> miniPlanes2;
Map miniMap2 = Map(miniMapWalls2, miniMapFloors2, miniPlanes2);

std::vector<Floor> floors2 = floorReader2.readFloorVertices(floorMaterial2);
std::vector<Wall> walls2 = wallReader2.readWallVertices(wallMaterial2);
std::vector<RacePlane> racePlanes2 = racePlaneReader2.readRacePlaneVertices(racePlaneMaterial2);
Map map2 = Map(walls2, floors2, racePlanes2);

bool keystates2[256] = {false};
bool sKeystates2[4] = {false};

// Global variables
float GLOBAL_WIDTH_2 = 800;
float GLOBAL_HEIGHT_2 = 800;

float lightPos2[2][4] = {
    {-20, 200, -20, 1},
    {20, 200, 20, 1}};

float lightAmb2[2][4] = {
    {0.1, 0.1, 0.1, 1},
    {0.1, 0.1, 0.1, 1}};

float floorAmb2 [4] = {0.05375f, 0.05f, 0.06625f, 0.82f};
float floorDiff2 [4] = {0.18275f, 0.17f, 0.22525f, 0.82f};
float floorSpec2 [4] = {0.332741f, 0.328634f, 0.346435f, 0.82f};

GLint twoPlayerRaceScreenOne, twoPlayerRaceScreenTwo;
int globalMaterialIndexRace1, globalMaterialIndexRace2;
int globalBoatIndex1, globalBoatIndex2;
bool raceComplete2 = false;
const char* winner;

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
    globalBoatIndex1 = boat1Index;
    globalBoatIndex2 = boat2Index;
    GLOBAL_HEIGHT_2 = height;
    GLOBAL_WIDTH_2 = width;
}

// Function to have camera follow boat
void useCamera2(Boat b)
{
    Point3D pos = b.pos;
    Vec3D backVec = b.forwardVector().normalize().multiply(b.camera.pos.x);
    Point3D camPos = backVec.add(Vec3D::createVector(Point3D(), Point3D(0,b.camera.pos.y,0))).movePoint(pos);

    gluLookAt(camPos.x, camPos.y, camPos.z, pos.x, pos.y, pos.z, 0,1,0);
}

// Display function for the player one race of two players
void TwoPlayerRaceScreenDisplayOne()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glScissor(0,0,800,800);
    glViewport(0,0,800, 800);
    glLoadIdentity();    

    useCamera2(boatRace1);    

    glPushMatrix(); 
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos2[0] );
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb2[0]);
    glColor3f(1, 1, 1);   
    glPushMatrix();
        glTranslatef(boatRace1.pos.x, boatRace1.pos.y, boatRace1.pos.z);
        glRotatef(-boatRace1.rot.alpha, 1,0,0);
        glRotatef(-boatRace1.rot.beta, 0,1,0);
        // glRotatef(boatRace1.rot.z, 0,0,1);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, globalMaterials[globalMaterialIndexRace1][0]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, globalMaterials[globalMaterialIndexRace1][1]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, globalMaterials[globalMaterialIndexRace1][2]);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10);
        boatRace1.draw();        
    glPopMatrix();

    glPushMatrix();
        glTranslatef(boatRace2.pos.x, boatRace2.pos.y, boatRace2.pos.z);
        glRotatef(-boatRace2.rot.alpha, 1,0,0);
        glRotatef(-boatRace2.rot.beta, 0,1,0);
        // glRotatef(boatRace2.rot.z, 0,0,1);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, globalMaterials[globalMaterialIndexRace2][0]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, globalMaterials[globalMaterialIndexRace2][1]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, globalMaterials[globalMaterialIndexRace2][2]);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10);
        boatRace2.draw();        
    glPopMatrix();

    glPushMatrix();
        map2.render();
    glPopMatrix();    
  
    glClear(GL_DEPTH_BUFFER_BIT);

    if(raceComplete2)
    {    
        renderText(-0.50, 0.65, GLUT_STROKE_ROMAN, "Race Complete", 0.001);
        renderText(-0.28, 0.50, GLUT_STROKE_ROMAN, "Winner:", 0.001);
        renderText(-0.35, 0.35, GLUT_STROKE_ROMAN, winner, 0.001);
        renderText(-0.45, 0.20, GLUT_STROKE_ROMAN, "Press r to restart the race", 0.0005);
        renderText(-0.45, 0.05, GLUT_STROKE_ROMAN, "Press q to quit the program", 0.0005);
    }
        
    glViewport(600, 600, 200, 200);
    glLoadIdentity();
    gluLookAt(cameraX2, cameraY2, cameraZ2, cameraDirX2, cameraDirY2, cameraDirZ2, 0, 1, 0);

    glPushMatrix();
        glTranslatef(boatRace1.pos.x, boatRace1.pos.y, boatRace1.pos.z);
        glutSolidSphere(10,16,16);
    glPopMatrix();

    glPushMatrix();
    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);
        miniMap2.render();
    glPopMatrix();  
    glEnable(GL_LIGHTING);   

    glPopMatrix();

    glutSwapBuffers();
}

// Display function for the player two race of two players
void TwoPlayerRaceScreenDisplayTwo(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glScissor(0,0,800,800);
    glViewport(0,0,800, 800);
    glLoadIdentity();

    useCamera2(boatRace2);    

    glPushMatrix();  
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos2[0] );
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb2[0]);  
    glPushMatrix();
        glTranslatef(boatRace1.pos.x, boatRace1.pos.y, boatRace1.pos.z);
        glRotatef(-boatRace1.rot.alpha, 1,0,0);
        glRotatef(-boatRace1.rot.beta, 0,1,0);
        // glRotatef(boatRace1.rot.z, 0,0,1);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, globalMaterials[globalMaterialIndexRace1][0]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, globalMaterials[globalMaterialIndexRace1][1]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, globalMaterials[globalMaterialIndexRace1][2]);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10);
        boatRace1.draw();        
    glPopMatrix();

    glPushMatrix();
        glTranslatef(boatRace2.pos.x, boatRace2.pos.y, boatRace2.pos.z);
        glRotatef(-boatRace2.rot.alpha, 1,0,0);
        glRotatef(-boatRace2.rot.beta, 0,1,0);
        // glRotatef(boatRace2.rot.z, 0,0,1);     
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, globalMaterials[globalMaterialIndexRace2][0]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, globalMaterials[globalMaterialIndexRace2][1]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, globalMaterials[globalMaterialIndexRace2][2]);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10);
        boatRace2.draw();
    glPopMatrix();


    glPushMatrix();
        map2.render();
    glPopMatrix();  

    glClear(GL_DEPTH_BUFFER_BIT);
    if(raceComplete2)
    {    
        renderText(-0.50, 0.65, GLUT_STROKE_ROMAN, "Race Complete", 0.001);
        renderText(-0.28, 0.50, GLUT_STROKE_ROMAN, "Winner:", 0.001);
        renderText(-0.35, 0.35, GLUT_STROKE_ROMAN, winner, 0.001);
        renderText(-0.45, 0.20, GLUT_STROKE_ROMAN, "Press r to restart the race", 0.0005);
        renderText(-0.45, 0.05, GLUT_STROKE_ROMAN, "Press q to quit the program", 0.0005);
    }
        
    glViewport(600, 600, 200, 200);
    glLoadIdentity();
    gluLookAt(cameraX2, cameraY2, cameraZ2, cameraDirX2, cameraDirY2, cameraDirZ2, 0, 1, 0);

    glPushMatrix();
        glTranslatef(boatRace2.pos.x, boatRace2.pos.y, boatRace2.pos.z);
        glutSolidSphere(10,16,16);
    glPopMatrix();

    glPushMatrix();
    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);
        miniMap2.render();
    glPopMatrix();  
    glEnable(GL_LIGHTING);   

    glPopMatrix();
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

// Timer function
 void timer2(int value)
{
    boatRace1.update(sKeystates2[1], sKeystates2[3], sKeystates2[0], sKeystates2[2]);    
    boatRace2.update(keystates2[(unsigned char)'w'], keystates2[(unsigned char)'s'], keystates2[(unsigned char)'a'], keystates2[(unsigned char)'d']);
    glutSetWindow(twoPlayerRaceScreenOne);
    glutPostRedisplay();    
    glutTimerFunc(17, timer2, 0);
}

// Timer function for second screen
void secondTimer(int value)
{
    boatRace1.update(sKeystates2[1], sKeystates2[3], sKeystates2[0], sKeystates2[2]);    
    boatRace2.update(keystates2[(unsigned char)'w'], keystates2[(unsigned char)'s'], keystates2[(unsigned char)'a'], keystates2[(unsigned char)'d']);
    glutSetWindow(twoPlayerRaceScreenTwo);
    glutPostRedisplay();    
    glutTimerFunc(17, secondTimer, 0);
}

// Key down function
void keyDown2(unsigned char key, int x, int y)
{
    keystates2[tolower(key)] = true;
}

// Key up function
void keyUp2(unsigned char key, int x, int y)
{
    keystates2[tolower(key)] = false;
    switch (key)
    {
        case 'q':
            exit(0);
            break;
        case 'r':
            if(raceComplete2)
            {
                glutDestroyWindow(twoPlayerRaceScreenTwo);
                glutDestroyWindow(twoPlayerRaceScreenOne);                
                raceComplete2 = false;
                TwoPlayerRaceScreen newTwoScreen = TwoPlayerRaceScreen(800, 800, 0, 50, "Player 2", globalBoatIndex1, globalMaterialIndexRace1, globalBoatIndex2, globalMaterialIndexRace2);
                newTwoScreen.determineMaterial();
                newTwoScreen.createWindow();
            }  
            break;          
        case 't': //step 2
            raceComplete2 = !raceComplete2;                                             
            break;        
        case '1': //step 1
            winner = "Player One";
            break;
        case '2':
            winner = "Player Two";
            break;                                     
    }
}

// Special key down function
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

// Special key up function
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

// Initialize screen
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
    glEnable(GL_SCISSOR_TEST);

    glutSetWindow(twoPlayerRaceScreenOne);
    glClearColor(0.6, 0.6, 0.6, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);    
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
}

// "main" function for this class
void TwoPlayerRaceScreen::createWindow()
{        
    boatRace1 = Boat(Point3D(200,10,-2), Boat::BoatType(boat1Index), DirectionAngle(180,0), Camera(Point3D(-5, 2, 0), Vec3D::createVector(Point3D(-5, 0, 0), Point3D()), 45));
    boatRace2 = Boat(Point3D(200,10,2), Boat::BoatType(boat2Index), DirectionAngle(180,0), Camera(Point3D(-5, 2, 0), Vec3D::createVector(Point3D(-5, 0, 0), Point3D()), 45));

    glutInitWindowSize(width, height);
    glutInitWindowPosition(windowPosX + 800, windowPosY);    
    twoPlayerRaceScreenTwo = glutCreateWindow("Player Two");
    map2.loadTextures();       
    glutReshapeFunc(reshape2);
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
    map2.loadTextures();
}