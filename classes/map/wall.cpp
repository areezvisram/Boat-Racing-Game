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

#include <map/wall.h>
#include <mathLib3D.h>
#include <material.h>
#include <iostream>
#include <PPM.h>

Wall::Wall() {
    vertices = {
        Point3D(0, 0, 0),
        Point3D(0, 0, 0),
        Point3D(0, 0, 0),
        Point3D(0, 0, 0),
    };
    normal = Vec3D(0,1,0);
    width = 0;
    height = 0;
}

Vec3D calculateNormal(Point3D vertex1, Point3D vertex2, Point3D vertex3)
{
    Vec3D edgeOne = Vec3D::createVector(vertex1, vertex2);
    Vec3D edgeTwo = Vec3D::createVector(vertex1, vertex3);

    Vec3D cross = Vec3D::cross(edgeOne, edgeTwo).normalize();

    return cross;
}

Wall::Wall(std::vector<Point3D> vertices) {
    this->vertices = vertices;
    this->normal = calculateNormal(vertices.at(0), vertices.at(1), vertices.at(2)).multiply(-1);
    this->width = vertices[0].distanceTo(vertices[1]);
    this->height = vertices[1].distanceTo(vertices[2]);
}

Wall::Wall(std::vector<Point3D> vertices, Material material) 
{
    this->vertices = vertices;
    this->material = material;
    this->normal = calculateNormal(vertices.at(0), vertices.at(1), vertices.at(2)).multiply(-1);
}

void drawAxis(float size)
{
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glLineWidth(2);
    glBegin(GL_LINES);

    // glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, red); // red x
    glColor3f (1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(size, 0.0, 0.0);

    // glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, yellow); // yellow y
    glColor3f (1.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, size, 0.0);

    // glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blue); // blue z
    glColor3f (0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, size);
    glEnd();
    glPopMatrix();
    // glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);

    glEnable(GL_LIGHTING);
}

// Draw wall
void Wall::draw() {
    glPushMatrix();
    glBegin(GL_POLYGON);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material.ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material.diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material.specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SPECULAR, material.shiny); 
    glNormal3f(this->normal.x, this->normal.y, this->normal.z);
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

Plane Wall::toPlane()
{
    Point3D pos;

    for(Point3D v : this->vertices)
    {
        pos.x += v.x;
        pos.y += v.y;
        pos.z += v.z;
    }
    pos.x /= 4.0;
    pos.y /= 4.0;
    pos.z /= 4.0;

    return Plane(pos, this->normal, width, height);
}