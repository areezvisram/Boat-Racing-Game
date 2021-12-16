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

#include <map/map.h>
#include <vector>
#include <map/wall.h>
#include <map/floor.h>
#include <map/racePlane.h>
#include <iostream>
#include <PPM.h>

GLuint textures[2];

GLubyte* water_img;
int waterWidth, waterHeight, waterMax;

GLubyte* buoy_img;
int buoyWidth, buoyHeight, buoyMax;

Map::Map(std::vector<Wall> walls, std::vector<Floor> floors, std::vector<RacePlane> racePlanes)
{
    this->walls = walls;
    this->floors = floors;
    this->racePlanes = racePlanes;
}

void Map::loadTextures()
{
    glEnable(GL_TEXTURE_2D);
    glGenTextures(2, textures);

    glBindTexture(GL_TEXTURE_2D, textures[0]);
    water_img = LoadPPM("textures/ocean.ppm", &waterWidth, &waterHeight, &waterMax);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, waterWidth, waterHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, water_img);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  

    glBindTexture(GL_TEXTURE_2D, textures[1]);
    buoy_img = LoadPPM("textures/stone.ppm", &buoyWidth, &buoyHeight, &buoyMax);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, buoyWidth, buoyHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, buoy_img);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  
}

void Map::render()
{
    for(Floor floor : floors) 
    {
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        floor.draw();
    }

    for(Wall wall : walls)
    {
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        wall.draw();
    }

    for(RacePlane racePlane : racePlanes)
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glDisable(GL_LIGHTING);
        racePlane.draw();
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);
    }
}