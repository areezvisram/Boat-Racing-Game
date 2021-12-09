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

int onePlayerWidth, onePlayerHeight, onePlayerMax;
GLubyte * onePlayerImage;

int twoPlayerWidth, twoPlayerHeight, twoPlayerMax;
GLubyte * twoPlayerImage;

BoatSelectionScreen boatSelectionScreen;


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

// Display function for the save screen
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

struct Handler {
    unsigned int mLeft, mRight, mTop, mBottom;
    void (*mHandlerFunc)();

    bool isInBounds(unsigned int x, unsigned int y) {
        return (x > mLeft && x < mRight && y < mTop && y > mBottom);
    }

    void handleClickAt(unsigned int x, unsigned int y) {
        if (isInBounds(x, y)) {
            mHandlerFunc();
        }
    }
};

void printPlayerOne()
{
    glutHideWindow();
    boatSelectionScreen = BoatSelectionScreen(800, 800, 4000, 50, "One Player Screen", 1);
    boatSelectionScreen.determineNumPlayers();
    boatSelectionScreen.createWindow();
}    

void printPlayerTwo()
{
    glutHideWindow();
    boatSelectionScreen = BoatSelectionScreen(800, 800, 4000, 50, "One Player Screen", 2);
    boatSelectionScreen.determineNumPlayers();
    boatSelectionScreen.createWindow();
}


Handler onePlayerClicked = {
    60,
    360,
    275,
    225,
    printPlayerOne
};

Handler twoPlayerClicked = {
    420,
    720,
    275,
    225,
    printPlayerTwo
};

struct InteractionHandler {
    std::vector<Handler *> mHandlers;

    void leftClickDown(int x, int y) {
        // std::cout << "Left click at " << x << ", " << y << std::endl;
        for (Handler *handler : mHandlers) {            
            handler->handleClickAt(x,y);
        }
    }

    void addHandler(Handler *handler) {
        mHandlers.push_back(handler);
    }
};

InteractionHandler mouseHandler;

void startScreenMouse(int button, int state, int x, int y)
{
    y = 800 - y;
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        mouseHandler.leftClickDown(x,y);
    }
}

int StartScreen::createWindow()
{
    mouseHandler.addHandler(&onePlayerClicked);
    mouseHandler.addHandler(&twoPlayerClicked);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(windowPosX, windowPosY);    
    GLint windowId = glutCreateWindow(windowName);    
    glutDisplayFunc(startScreenDisplay);    
    glutMouseFunc(startScreenMouse);
    return windowId;

}