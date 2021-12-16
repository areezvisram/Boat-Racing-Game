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

void Wall::draw() {
    // Point3D pos;

    // for(Point3D vertex : vertices)
    // {
    //     pos.x += vertex.x;
    //     pos.y += vertex.y;
    //     pos.z += vertex.z;
    // }

    // pos.x /= 4.0;
    // pos.y /= 4.0;
    // pos.z /= 4.0;

    // glPushMatrix();
    // // glTranslatef(pos.x,pos.y,pos.z);
    // glDisable(GL_LIGHTING);
    // glLoadIdentity();
    // glPointSize(10);
    // glColor3f(1,0,0);
    // glBegin(GL_POINTS);
    // glVertex3f(pos.x,pos.y,pos.z);
    // glEnd();
    // // drawAxis(3);
    // glEnable(GL_LIGHTING);
    // glPopMatrix();



    // Vec3D n = calculateNormal(vertices.at(0), vertices.at(1), vertices.at(2)).multiply(-1);
    glBegin(GL_POLYGON);
    //std::cout << normal.x << ", " << normal.y << ", " << normal.z << "\n";
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material.ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material.diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material.specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SPECULAR, material.shiny); 
    glNormal3f(this->normal.x, this->normal.y, this->normal.z);
    for(Point3D vertex : vertices) {
        glVertex3f(vertex.x, vertex.y, vertex.z);
    }
    glEnd();

    Point3D pos = Point3D();

    glDisable(GL_LIGHTING);
    glPointSize(20);
    glBegin(GL_POINTS);
    glColor3f(1,0,0);
    for(Point3D vertex : vertices)
    {
        glVertex3f(vertex.x, vertex.y, vertex.z);
        pos.x += vertex.x;
        pos.y += vertex.y;
        pos.z += vertex.z;
    }
    pos.x /= 4.0;
    pos.y /= 4.0;
    pos.z /= 4.0;

    glVertex3f(pos.x, 15, pos.z);
    glEnd();
    glEnable(GL_LIGHTING);
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