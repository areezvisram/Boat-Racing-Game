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

#ifndef TWO_PLAYER_RACE_H
#define TWO_PLAYER_RACE_H

class TwoPlayerRaceScreen
{
    public:
        TwoPlayerRaceScreen();
        TwoPlayerRaceScreen(int width, int height, int windowPosX, int windowPosY, const char* windowName, int boat1Index, int material1Index, int boat2Index, int material2Index);

        int width, height, windowPosX, windowPosY, material1Index, boat1Index, boat2Index, material2Index;
        const char* windowName;               

        void createWindow();

        void determineMaterial();
};


#endif