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

#ifndef START_SCREEN_H
#define START_SCREEN_H

class StartScreen
{
    public:
        StartScreen();
        StartScreen(int width, int height, int windowPosX, int windowPosY, const char* windowName);

        int width, height, windowPosX, windowPosY;
        const char* windowName;        

        void createWindow();
        void setProperties();
};


#endif