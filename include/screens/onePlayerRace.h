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

#ifndef ONE_PLAYER_RACE_H
#define ONE_PLAYER_RACE_H

class OnePlayerRaceScreen
{
    public:
        OnePlayerRaceScreen();
        OnePlayerRaceScreen(int width, int height, int windowPosX, int windowPosY, const char* windowName, int boatIndex, int materialIndex);

        int width, height, windowPosX, windowPosY, materialIndex, boatIndex;
        const char* windowName;               

        int createWindow();

        void determineMaterial();
};


#endif