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

#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include <vector>
#include <material.h>
#include <iostream>

void renderText(float x, float y, void *font, const char *string, float scaleFactor);

struct Handler {
    unsigned int mLeft, mRight, mTop, mBottom;
    void (*mHandlerFunc)();

    bool isInBounds(unsigned int x, unsigned int y) {        
        // std::cout << x << ", " << mLeft << "\n";
        // std::cout << x << ", " << mRight << "\n";

        // std::cout << y << ", " << mTop << "\n";
        // std::cout << y << ", " << mBottom << "\n";
        return (x > mLeft && x < mRight && y < mTop && y > mBottom);
    }

    void handleClickAt(unsigned int x, unsigned int y) {
        if (isInBounds(x, y)) {
            mHandlerFunc();
        }
    }
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

// Material globals
const float globalMaterials[5][4][4] = {
    {{0.0f, 0.1f, 0.06f, 1.0f}, //cyan plastic
     {0.0f, 0.50980392f, 0.50980392f, 1.0f},
     {0.50196078f, 0.50196078f, 0.50196078f, 1.0f}},
    {{0.1745f, 0.01175f, 0.01175f, 0.55f}, //ruby
     {0.61424f, 0.04136f, 0.04136f, 0.55f},
     {0.727811f, 0.626959f, 0.626959f, 0.55f}},
    {{0.0215f, 0.1745f, 0.0215f, 0.55f}, //emerald
     {0.07568f, 0.61424f, 0.07568f, 0.55f},
     {0.633f, 0.727811f, 0.633f, 0.55f}},
    {{0.05375f, 0.05f, 0.06625f, 0.82f}, //obsidian
     {0.18275f, 0.17f, 0.22525f, 0.82f},
     {0.332741f, 0.328634f, 0.346435f, 0.82f}},
     {{0.2125f, 0.1275f, 0.054f, 1.0f}, // bronze
      {0.714f, 0.4284f, 0.18144f, 1.0f},
      {0.393548f, 0.271906f, 0.166721f, 1.0f}}
}; 

// GLuint textures[2];

// GLubyte* water_img;
// int waterWidth, waterHeight, waterMax;

// GLubyte* buoy_img;
// int buoyWidth, buoyHeight, buoyMax;

// void loadTextures()
// {
//     glGenTextures(1, textures);

//     glBindTexture(GL_TEXTURE_2D, textures[0]);
//     water_img = LoadPPM("textures/ocean.ppm", &waterWidth, &waterHeight, &waterMax);
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, waterWidth, waterHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, water_img);
//     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  

//     glBindTexture(GL_TEXTURE_2D, textures[1]);
//     buoy_img = LoadPPM("textures/buoys.ppm", &buoyWidth, &buoyHeight, &buoyMax);

//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, buoyWidth, buoyHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, buoy_img);
//     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  
// }

#endif