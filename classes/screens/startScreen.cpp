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

#include <screens/startScreen.h>
#include <screens/boatSelectionScreen.h>
#include <helperFunctions.h>
#include <PPM.h>
#include <iostream>
#include <vector>

// Globals
GLint windowId;

int onePlayerWidth, onePlayerHeight, onePlayerMax;
GLubyte * onePlayerImage;

int twoPlayerWidth, twoPlayerHeight, twoPlayerMax;
GLubyte * twoPlayerImage;

BoatSelectionScreen boatSelectionScreen;
float GLOBAL_WIDTH_START;
float GLOBAL_HEIGHT_START;

StartScreen::StartScreen()
{
    width = 800;
    height = 800;
    windowPosX = 0;
    windowPosY = 0;
    windowName = "Sample Window";
}

StartScreen::StartScreen(int width, int height, int windowPosX, int windowPosY, const char* windowName)
{
    this->width = width;
    this->height = height;
    this->windowPosX = windowPosX;
    this->windowPosY = windowPosY;
    this->windowName = windowName;
}

void StartScreen::setProperties()
{
    GLOBAL_HEIGHT_START = height;
    GLOBAL_WIDTH_START = width;
}

// Display function for the start screen
void startScreenDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();       
    glColor3f(1,1,1);
    glScalef(5,5,5);
    renderText(-0.60, 0.70, GLUT_STROKE_ROMAN, "Boat Racing Game", 0.0008);
    renderText(-0.75, 0.55, GLUT_STROKE_ROMAN, "SFWRENG 3GC3 Final Project", 0.0008);    
    renderText(-0.85, 0.40, GLUT_STROKE_ROMAN, "By Areez Visram and Neil Lobo", 0.0008);

    renderText(-0.95, -0.20, GLUT_STROKE_ROMAN, "How many players would you like to race with?", 0.0006);        
    glPopMatrix();

    glPushMatrix();
    onePlayerImage = LoadPPM("images/oneplayer.ppm", &onePlayerWidth, &onePlayerHeight, &onePlayerMax);
    glRasterPos2f(-0.10, -0.50);
    glPixelZoom(-1, 1);
    glDrawPixels(onePlayerWidth, onePlayerHeight, GL_RGB, GL_UNSIGNED_BYTE, onePlayerImage);
    glPopMatrix();

    glPushMatrix();
    twoPlayerImage = LoadPPM("images/twoplayer.ppm", &twoPlayerWidth, &twoPlayerHeight, &twoPlayerMax);
    glRasterPos2f(0.80, -0.50);
    glPixelZoom(-1, 1);
    glDrawPixels(twoPlayerWidth, twoPlayerHeight, GL_RGB, GL_UNSIGNED_BYTE, twoPlayerImage);
    glPopMatrix();

    glutSwapBuffers();
}

// Function if one player is chosen
void oneBoat()
{    
    glutDestroyWindow(windowId);
    boatSelectionScreen = BoatSelectionScreen(800, 800, 2000, 50, "Boat Selection", 1, 1);
    boatSelectionScreen.determineNumPlayers();
    boatSelectionScreen.createWindow(true);
}    

// Function if two player is chosen
void twoBoats()
{
    glutDestroyWindow(windowId);
    boatSelectionScreen = BoatSelectionScreen(800, 800, 2000, 50, "Boat Selection", 2, 1);
    boatSelectionScreen.determineNumPlayers();
    boatSelectionScreen.createWindow(true);
}

// Mouse handlers for detecting mouse position
Handler onePlayerClicked = {
    60,
    360,
    250,
    200,
    oneBoat
};

Handler twoPlayerClicked = {
    420,
    720,
    250,
    200,
    twoBoats
};

InteractionHandler mouseHandler;

// Mouse function
void startScreenMouse(int button, int state, int x, int y)
{
    y = 800 - y;
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        mouseHandler.leftClickDown(x,y);
    }
}

// Reshape function
void reshapeStart(int w, int h)
{
    GLOBAL_WIDTH_START = w;
    GLOBAL_HEIGHT_START = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w / h, 1, 1000);

    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
}

// "main" function for this class
void StartScreen::createWindow()
{
    mouseHandler.addHandler(&onePlayerClicked);
    mouseHandler.addHandler(&twoPlayerClicked);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(windowPosX, windowPosY);    
    windowId = glutCreateWindow(windowName);    
    glutDisplayFunc(startScreenDisplay);    
    glutMouseFunc(startScreenMouse);

}