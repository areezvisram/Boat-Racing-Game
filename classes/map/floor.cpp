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

// Draw floor
void Floor::draw() {
    glPushMatrix();
    glBegin(GL_POLYGON);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material.ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material.diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material.specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SPECULAR, material.shiny); 
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
}