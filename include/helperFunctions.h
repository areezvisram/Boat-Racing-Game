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

// Material mats[5] = {
//     Material(materials[0][0], materials[0][1], materials[0][2], 10),
//     Material(materials[1][0], materials[1][1], materials[1][2], 10),
//     Material(materials[2][0], materials[2][1], materials[2][2], 10),
//     Material(materials[3][0], materials[3][1], materials[3][2], 10),
//     Material(materials[4][0], materials[4][1], materials[4][2], 25.6)};   

#endif