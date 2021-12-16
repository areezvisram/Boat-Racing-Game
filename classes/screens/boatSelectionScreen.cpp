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

#include <screens/boatSelectionScreen.h>
#include <screens/onePlayerRace.h>
#include <screens/twoPlayerRace.h>
#include <helperFunctions.h>
#include <object/boat.h>
#include <iostream>
#include <PPM.h>

// Globals
OnePlayerRaceScreen onePlayerRaceScreen;
TwoPlayerRaceScreen twoPlayerRaceScreen;
BoatSelectionScreen playerTwoSelectionScreen;

GLint boatWindowId;

int numberOfPlayers, currentPlayerNum;
std::vector<Boat> boats = 
{
    Boat(Point3D(0, -0.1, 0), Boat::SPEED, Vec3D(0, 0, 0), Camera(Point3D(-5, 2, 0), Vec3D::createVector(Point3D(-5, 0, 0), Point3D()), 45)),
    Boat(Point3D(0, -0.1, 0), Boat::FISHING, Vec3D(0, 0, 0), Camera(Point3D(-5, 2, 0), Vec3D::createVector(Point3D(-5, 0, 0), Point3D()), 45)),
    Boat(Point3D(0, -0.3, 0), Boat::PIRATE, Vec3D(0, 0, 0), Camera(Point3D(-5, 2, 0), Vec3D::createVector(Point3D(-5, 0, 0), Point3D()), 45)),    
    Boat(Point3D(0, -0.2, 0), Boat::SMALLPIRATE, Vec3D(0, 0, 0), Camera(Point3D(-5, 2, 0), Vec3D::createVector(Point3D(-5, 0, 0), Point3D()), 45)),    
};

int globalMaterialIndex = 1;
int boatIndex = 0;

int leftWidth, leftHeight, leftMax;
GLubyte * leftImage;

int rightWidth, rightHeight, rightMax;
GLubyte * rightImage;

int matsWidth, matsHeight, matsMax;
GLubyte * matsImage;

int doneWidth, doneHeight, doneMax;
GLubyte * doneImage;

std::vector<int> twoPlayerInfo;

BoatSelectionScreen::BoatSelectionScreen()
{
    width = 800;
    height = 800;
    windowPosX = 0;
    windowPosY = 0;
    windowName = "Sample Window";
}

BoatSelectionScreen::BoatSelectionScreen(int width, int height, int windowPosX, int windowPosY, const char* windowName, int numPlayers, int currentPlayer)
{
    this->width = width;
    this->height = height;
    this->windowPosX = windowPosX;
    this->windowPosY = windowPosY;
    this->windowName = windowName;
    this->numPlayers = numPlayers;
    this->currentPlayer = currentPlayer;
}

// Determine the number of players that was selected
void BoatSelectionScreen::determineNumPlayers()
{
    numberOfPlayers = numPlayers;
    currentPlayerNum = currentPlayer;
}

// Navigate to race screen based on number of players
void goToNextScreen()
{
    if(numberOfPlayers == 1)
    {
        glutDestroyWindow(boatWindowId);
        onePlayerRaceScreen = OnePlayerRaceScreen(800, 800, 200, 50, "Player 1", boatIndex, globalMaterialIndex);    
        onePlayerRaceScreen.determineMaterial();
        onePlayerRaceScreen.createWindow();
    } 
    else if(numberOfPlayers == 2)
    {        
        if(currentPlayerNum == 1)
        {
            glutDestroyWindow(boatWindowId);
            twoPlayerInfo.push_back(boatIndex);
            twoPlayerInfo.push_back(globalMaterialIndex);
            playerTwoSelectionScreen = BoatSelectionScreen(800, 800, 200, 50, "Boat Selection", 2, 2);
            globalMaterialIndex = 0;
            boatIndex = 0;
            playerTwoSelectionScreen.determineNumPlayers();
            playerTwoSelectionScreen.createWindow(false);
        } 
        else if(currentPlayerNum == 2)
        {
            glutDestroyWindow(boatWindowId);
            twoPlayerRaceScreen = TwoPlayerRaceScreen(800, 800, 0, 50, "Player One", twoPlayerInfo.at(0), twoPlayerInfo.at(1), boatIndex, globalMaterialIndex);
            twoPlayerRaceScreen.determineMaterial();
            twoPlayerRaceScreen.createWindow();
        }
    }
}

// Set materials based on mouse input
void setMaterialZero()
{
    globalMaterialIndex = 0;
    glutPostRedisplay();    
}

void setMaterialOne()
{
    globalMaterialIndex = 1;
    glutPostRedisplay();
}

void setMaterialTwo()
{
    globalMaterialIndex = 2;
    glutPostRedisplay();
}

void setMaterialThree()
{
    globalMaterialIndex = 3;
    glutPostRedisplay();
}

void setMaterialFour()
{
    globalMaterialIndex = 4;
    glutPostRedisplay();
}

void decreaseBoatIndex()
{
    if(boatIndex == 0)
    {
        boatIndex = 3;
    } else {
        boatIndex -= 1;
    }
    glutPostRedisplay();
}

void increaseBoatIndex()
{
    if(boatIndex == 3)
    {
        boatIndex = 0;
    } else {
        boatIndex += 1;
    }
    glutPostRedisplay();
}

// Creates handlers to detect mouse position
Handler material0Clicked = {
    160,
    240,
    140,
    60,
    setMaterialOne
};

Handler material1Clicked = {
    260,
    340,
    140,
    60,
    setMaterialZero
};

Handler material2Clicked = {
    360,
    440,
    140,
    60,
    setMaterialTwo
};

Handler material3Clicked = {
    460,
    540,
    140,
    60,
    setMaterialThree
};

Handler material4Clicked = {
    560,
    640,
    140,
    60,
    setMaterialFour
};

Handler leftClicked = {
    220,
    320,
    240,
    180,
    decreaseBoatIndex
};

Handler rightClicked = {
    460,
    560,
    240,
    180,
    increaseBoatIndex
};

Handler doneClicked =
{
    670,
    750,
    100,
    50,
    goToNextScreen
};

InteractionHandler boatSelectionMouseHandler;

// Mouse function
void boatSelectionMouse(int button, int state, int x, int y)
{
    y = 800 - y;
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {        
        boatSelectionMouseHandler.leftClickDown(x,y);
    }
}

// Display function
void boatSelectionDisplay()
{    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();       
    glColor3f(1,1,1);
    glScalef(5,5,5);
    glDisable(GL_LIGHTING);
    renderText(-0.40, 0.80, GLUT_STROKE_ROMAN, "Boat Selection", 0.001);    
    glPopMatrix();

    glPushMatrix();       
    glColor3f(1,1,1);
    glScalef(5,5,5);
    glDisable(GL_LIGHTING);
    if(currentPlayerNum == 1)
    {
        renderText(-0.30, 0.65, GLUT_STROKE_ROMAN, "Player One", 0.001);    
    } 
    else if(currentPlayerNum == 2)
    {
        renderText(-0.30, 0.65, GLUT_STROKE_ROMAN, "Player Two", 0.001);    
    }

    glPopMatrix();


    Boat boatSelection = boats.at(boatIndex);

    glPushMatrix();
    glEnable(GL_LIGHTING);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, globalMaterials[globalMaterialIndex][0]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, globalMaterials[globalMaterialIndex][1]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, globalMaterials[globalMaterialIndex][2]);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10);
    glTranslatef(boatSelection.pos.x, boatSelection.pos.y, boatSelection.pos.z);
    glRotatef(boatSelection.rot.x, 1,0,0);
    glRotatef(boatSelection.rot.y, 0,1,0);
    glRotatef(boatSelection.rot.z, 0,0,1);
    boatSelection.draw();
    glPopMatrix();

    glPushMatrix();    
    glRasterPos2f(-0.20, -0.60);    
    glPixelZoom(-1, 1);
    glDrawPixels(leftWidth, leftHeight, GL_RGB, GL_UNSIGNED_BYTE, leftImage);
    glPopMatrix();

    glPushMatrix();    
    glRasterPos2f(0.40, -0.60);    
    glPixelZoom(-1, 1);
    glDrawPixels(rightWidth, rightHeight, GL_RGB, GL_UNSIGNED_BYTE, rightImage);
    glPopMatrix();

    glPushMatrix();    
    glRasterPos2f(0.60, -0.90);
    glPixelZoom(-1, 1);
    glDrawPixels(matsWidth, matsHeight, GL_RGB, GL_UNSIGNED_BYTE, matsImage);
    glPopMatrix();

    glPushMatrix();
    glRasterPos2f(0.90, -0.95);
    glPixelZoom(-1, 1);
    glDrawPixels(doneWidth, doneHeight, GL_RGB, GL_UNSIGNED_BYTE, doneImage);
    glPopMatrix();

    glutSwapBuffers();
}

// Timer function
void boatSelectionTimer(int value)
{      
    glutPostRedisplay();    
    glutTimerFunc(17, boatSelectionTimer, 0);

}

// "main" for this class
void BoatSelectionScreen::createWindow(bool addHandlers)
{
    leftImage = LoadPPM("images/left.ppm", &leftWidth, &leftHeight, &leftMax);
    rightImage = LoadPPM("images/right.ppm", &rightWidth, &rightHeight, &rightMax);
    matsImage = LoadPPM("images/materials.ppm", &matsWidth, &matsHeight, &matsMax);
    doneImage = LoadPPM("images/done.ppm", &doneWidth, &doneHeight, &doneMax);
    if(addHandlers)
    {
        boatSelectionMouseHandler.addHandler(&material0Clicked);
        boatSelectionMouseHandler.addHandler(&material1Clicked);
        boatSelectionMouseHandler.addHandler(&material2Clicked);
        boatSelectionMouseHandler.addHandler(&material3Clicked);
        boatSelectionMouseHandler.addHandler(&material4Clicked);
        boatSelectionMouseHandler.addHandler(&leftClicked);
        boatSelectionMouseHandler.addHandler(&rightClicked);
        boatSelectionMouseHandler.addHandler(&doneClicked);
    }

    glutInitWindowSize(width, height);
    glutInitWindowPosition(windowPosX, windowPosY);        
    boatWindowId = glutCreateWindow(windowName);         
    glutDisplayFunc(boatSelectionDisplay);  
    glutTimerFunc(17, boatSelectionTimer, 0);
    glutMouseFunc(boatSelectionMouse);

    glutSetWindow(boatWindowId);
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);    
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

}