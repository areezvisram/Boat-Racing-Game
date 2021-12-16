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

#ifndef BOAT_SELECTION_SCREEN_H
#define BOAT_SELECTION_SCREEN_H

class BoatSelectionScreen
{
    public:
        BoatSelectionScreen();
        BoatSelectionScreen(int width, int height, int windowPosX, int windowPosY, const char* windowName, int numPlayers, int currentPlayer);

        int width, height, windowPosX, windowPosY, numPlayers, currentPlayer;
        const char* windowName;        

        void createWindow(bool addHandlers);

        void determineNumPlayers();
};


#endif