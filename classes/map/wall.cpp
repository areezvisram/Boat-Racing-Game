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

Wall::Wall() {
    vertices = {
        Point3D(0, 0, 0),
        Point3D(0, 0, 0),
        Point3D(0, 0, 0),
        Point3D(0, 0, 0),
    };
}

Wall::Wall(std::vector<Point3D> vertices) {
    this->vertices = vertices;
}

Wall::Wall(std::vector<Point3D> vertices, Material material) 
{
    this->vertices = vertices;
    this->material = material;
}

Vec3D calculateNormal(Point3D vertex1, Point3D vertex2, Point3D vertex3)
{
    // std::cout << vertex1.x << ", " << vertex1.y << ", " << vertex1.z << "\n";
    // std::cout << vertex2.x << ", " << vertex2.y << ", " << vertex2.z << "\n";
    // std::cout << vertex3.x << ", " << vertex3.y << ", " << vertex3.z << "\n";
    Vec3D edgeOne = Vec3D::createVector(vertex1, vertex2);
    Vec3D edgeTwo = Vec3D::createVector(vertex1, vertex3);

    Vec3D cross = Vec3D::cross(edgeOne, edgeTwo).normalize();

    return cross;
}

void Wall::draw() {
    glBegin(GL_POLYGON);
    Vec3D normal = calculateNormal(vertices.at(0), vertices.at(1), vertices.at(2)).multiply(-1);
    //std::cout << normal.x << ", " << normal.y << ", " << normal.z << "\n";
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material.ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material.diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material.specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SPECULAR, material.shiny); 
    glNormal3f(normal.x, normal.y, normal.z);
    for(Point3D vertex : vertices) {
        glVertex3f(vertex.x, vertex.y, vertex.z);
    }
    glEnd();
}