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
#include <helperFunctions.h>

const char* text;


BoatSelectionScreen::BoatSelectionScreen()
{
    width = 800;
    height = 800;
    windowPosX = 0;
    windowPosY = 0;
    windowName = "Sample Window";
}

BoatSelectionScreen::BoatSelectionScreen(int width, int height, int windowPosX, int windowPosY, const char* windowName, int numPlayers)
{
    this->width = width;
    this->height = height;
    this->windowPosX = windowPosX;
    this->windowPosY = windowPosY;
    this->windowName = windowName;
    this->numPlayers = numPlayers;
}

void BoatSelectionScreen::determineNumPlayers()
{
    if(numPlayers == 1)
    {
        text = "ONE PLAYER";
    }
    else if(numPlayers == 2)
    {
        text = "TWO PLAYER";
    }
}

// Display function for the save screen
void onePlayerDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();       
    glColor3f(1,1,1);
    glScalef(5,5,5);
    renderText(-0.60, 0.70, GLUT_STROKE_ROMAN, text, 0.0008);    
    glPopMatrix();

    glutSwapBuffers();
}


int BoatSelectionScreen::createWindow()
{
    glutInitWindowSize(width, height);
    glutInitWindowPosition(windowPosX, windowPosY);    
    GLint windowId = glutCreateWindow(windowName);    
    glutDisplayFunc(onePlayerDisplay);  
    return windowId;

}