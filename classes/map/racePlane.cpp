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

#include <map/racePlane.h>
#include <mathLib3D.h>
#include <material.h>
#include <iostream>
#include <PPM.h>

RacePlane::RacePlane() {
    vertices = {
        Point3D(0, 0, 0),
        Point3D(0, 0, 0),
        Point3D(0, 0, 0),
        Point3D(0, 0, 0),
    };
}

RacePlane::RacePlane(std::vector<Point3D> vertices) {
    this->vertices = vertices;
}

RacePlane::RacePlane(std::vector<Point3D> vertices, Material material) 
{
    this->vertices = vertices;
    this->material = material;
}

Vec3D calculateNormal2(Point3D vertex1, Point3D vertex2, Point3D vertex3)
{
    Vec3D edgeOne = Vec3D::createVector(vertex1, vertex2);
    Vec3D edgeTwo = Vec3D::createVector(vertex1, vertex3);

    Vec3D cross = Vec3D::cross(edgeOne, edgeTwo).normalize();

    return cross;
}

void RacePlane::draw() {
    glPushMatrix();
    glBegin(GL_POLYGON);
    Vec3D normal = calculateNormal2(vertices.at(0), vertices.at(1), vertices.at(2)).multiply(-1);    
    glColor4f(1,1,1,0);
    glNormal3f(normal.x, normal.y, normal.z);    
    glVertex3f(vertices.at(0).x, vertices.at(0).y, vertices.at(0).z);    
    glVertex3f(vertices.at(1).x, vertices.at(1).y, vertices.at(1).z);    
    glVertex3f(vertices.at(2).x, vertices.at(2).y, vertices.at(2).z);
    glVertex3f(vertices.at(3).x, vertices.at(3).y, vertices.at(3).z);
    glEnd();
    glPopMatrix();    
}