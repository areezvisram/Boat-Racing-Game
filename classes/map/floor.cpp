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

#include <map/floor.h>
#include <mathLib3D.h>
#include <material.h>
#include <PPM.h>

// GLuint textures[1];

// GLubyte* water_img;
// int waterWidth, waterHeight, waterMax;

// GLubyte* buoy_img;
// int buoyWidth, buoyHeight, buoyMax;


Floor::Floor() {
    vertices = {
        Vec3D(0, 0, 0),
        Vec3D(0, 0, 0),
        Vec3D(0, 0, 0),
        Vec3D(0, 0, 0),
    };
}

Floor::Floor(std::vector<Vec3D> vertices) {
    this->vertices = vertices;
}

Floor::Floor(std::vector<Vec3D> vertices, Material material) 
{
    this->vertices = vertices;
    this->material = material;
}

// void Floor::loadTextures()
// {
//     glEnable(GL_TEXTURE_2D);

//     glBindTexture(GL_TEXTURE_2D, textures[0]);
//     water_img = LoadPPM("textures/ocean.ppm", &waterWidth, &waterHeight, &waterMax);
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, waterWidth, waterHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, water_img);
//     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  //     glGenTextures(1, textures);

// }

void Floor::draw() {
    // glDisable(GL_LIGHTING);
    glPushMatrix();
    //glBindTexture(GL_TEXTURE_2D, textures[0]);
    glBegin(GL_POLYGON);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material.ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material.diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material.specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SPECULAR, material.shiny); 
    // glNormal3f(0,1,0);
    // for(Vec3D vertex : vertices) {
    //     glVertex3f(vertex.x, vertex.y, vertex.z);
    // }
    glTexCoord2f(0,0);
    glVertex3f(vertices.at(0).x, vertices.at(0).y, vertices.at(0).z);
    glTexCoord2f(1,0);
    glVertex3f(vertices.at(1).x, vertices.at(1).y, vertices.at(1).z);
    glTexCoord2f(1,1);
    glVertex3f(vertices.at(2).x, vertices.at(2).y, vertices.at(2).z);
    glTexCoord2f(0,1);
    glVertex3f(vertices.at(3).x, vertices.at(3).y, vertices.at(3).z);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    // glEnable(GL_LIGHTING);
}