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

void Floor::draw() {
    glPushMatrix();
    glBegin(GL_POLYGON);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material.ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material.diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material.specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SPECULAR, material.shiny); 
    glNormal3f(0,1,0);
    for(Vec3D vertex : vertices) {
        glVertex3f(vertex.x, vertex.y, vertex.z);
    }
    glEnd();
    glPopMatrix();
}