#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#endif

#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <fstream>
#include <mathLib3D.h>
#include <event.h>
#include <camera.h>
#include <mesh/mesh.h>
#include <object/upgrade.h>
#include <object/boat.h>
#include <map/floor.h>
#include <material.h>
#include <fileReader.h>
#include <map/map.h>
#include <map/wall.h>

Camera camera;
Mesh mesh;
Upgrade upgrade;
Boat boat;
float mat_ambient [4] ={ 0.329412f, 0.223529f, 0.027451f,1.0f };
float mat_diffuse [4] ={ 0.780392f, 0.568627f, 0.113725f, 1.0f };
float mat_specular [4] ={ 0.992157f, 0.941176f, 0.807843f, 1.0f };
float shine = 27.8974f;

float wall_ambient [4] = {0.0f, 0.1f, 0.06f, 1.0f};
float wall_diffuse[4] =  {0.0f, 0.50980392f, 0.50980392f, 1.0f};
float wall_specular[4] =    {0.50196078f, 0.50196078f, 0.50196078f, 1.0f};
float wall_shine = 10.0f;

FileReader floorReader = FileReader("map/floor.txt");
FileReader wallReader = FileReader("map/walls.txt");
Material floorMaterial = Material(mat_ambient, mat_diffuse, mat_specular, shine);
Material wallMaterial = Material(wall_ambient, wall_diffuse, wall_specular, wall_shine);

std::vector<Floor> test_floors = floorReader.readFloorVertices(floorMaterial);
std::vector<Wall> test_walls = wallReader.readWallVertices(wallMaterial);
Map map = Map(test_walls, test_floors);

bool keystates[256] = {false};
bool sKeystates[4] = {false};

// Global variables
float GLOBAL_WIDTH = 800;
float GLOBAL_HEIGHT = 800;
float cameraX = 100;
float cameraY = 600;
float cameraZ = 0;

float cameraDirX = 0;
float cameraDirY = 50;
float cameraDirZ = 0;

// float cameraX = 0;
// float cameraY = 300;
// float cameraZ = 400;

// float cameraDirX = 0;
// float cameraDirY = 50;
// float cameraDirZ = 0;

bool showLights = false;

float lightPos[2][4] = {
    {-20, 200, -20, 1},
    {20, 200, 20, 1}};

float lightAmb[2][4] = {
    {0.1, 0.1, 0.1, 1},
    {0.1, 0.1, 0.1, 1}};

float lightEmis[2][4] = {
    {1, 1, 1, 1},
    {1, 1, 1, 1}};

float floorAmb [4] = {0.05375f, 0.05f, 0.06625f, 0.82f};
float floorDiff [4] = {0.18275f, 0.17f, 0.22525f, 0.82f};
float floorSpec [4] = {0.332741f, 0.328634f, 0.346435f, 0.82f};

float red[4] = {1,0,0,1};
float yellow[4] = {1,1,0,1};
float blue[4] = {0,0,1,1};
float black[4] = {0,0,0,1};

void drawAxis()
{
    glPushMatrix();
    glLineWidth(2);
    glBegin(GL_LINES);

    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, red); // red x
    glColor3f (1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(5.0, 0.0, 0.0);

    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, yellow); // yellow y
    glColor3f (1.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 5.0, 0.0);

    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blue); // blue z
    glColor3f (0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 5.0);
    glEnd();
    glPopMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
}

// Draw the floor on which particles bounce
void drawFloor()
{
    glPushMatrix();
    glBegin(GL_POLYGON);
    glColor3f(0.3, 0.3, 1);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, floorAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, floorDiff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, floorSpec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SPECULAR, 10);
    glNormal3f(0, 1, 0);
    glVertex3f(200, 0, 200);
    glVertex3f(200, 0, -200);
    glVertex3f(-200, 0, -200);
    glVertex3f(-200, 0, 200);
    glEnd();

    glPopMatrix();
}

void useCamera(Boat b)
{
    Point3D pos = b.pos;
    Vec3D backVec = b.forwardVector().normalize().multiply(b.camera.pos.x);
    Point3D camPos = backVec.add(Vec3D::createVector(Point3D(), Point3D(0,b.camera.pos.y,0))).movePoint(pos);

    gluLookAt(camPos.x, camPos.y, camPos.z, pos.x, pos.y, pos.z, 0,1,0);
}

// Display function
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    useCamera(boat);
    //gluLookAt(cameraX, cameraY, cameraZ, cameraDirX, cameraDirY, cameraDirZ, 0, 1, 0);

    glPushMatrix();

    // glPushMatrix();
    //     drawFloor();
    // glPopMatrix();
    glPushMatrix();
        glTranslatef(boat.pos.x, boat.pos.y, boat.pos.z);
        glRotatef(boat.rot.x, 1,0,0);
        glRotatef(boat.rot.y, 0,1,0);
        glRotatef(boat.rot.z, 0,0,1);
        glColor3f(0.7, 0.1, 0);
        boat.draw();
        drawAxis();
    glPopMatrix();

    glPushMatrix();
        map.render();
    glPopMatrix();

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos[0] );
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb[0]);
    glColor3f(1, 1, 1);

    glPopMatrix();

    glutSwapBuffers();
}

// Reshape function
void reshape(int w, int h)
{
    GLOBAL_WIDTH = w;
    GLOBAL_HEIGHT = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w / h, 1, 1000);

    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
}

void timer(int value)
{
    boat.update(sKeystates[1], sKeystates[3], sKeystates[0], sKeystates[2]);
    glutPostRedisplay();    
    glutTimerFunc(17, timer, 0);
}

void keyDown(unsigned char key, int x, int y)
{
    keystates[tolower(key)] = true;
}

void keyUp(unsigned char key, int x, int y)
{
    keystates[tolower(key)] = false;
    switch (key)
    {
        case 'q':
            exit(0);
            break;
    }
}

void specialDown(int key, int x, int y)
{
    switch(key) {
        case GLUT_KEY_LEFT:
            sKeystates[0] = true;
            break;
        case GLUT_KEY_UP:
            sKeystates[1] = true;
            break;
        case GLUT_KEY_RIGHT:
            sKeystates[2] = true;
            break;
        case GLUT_KEY_DOWN:
            sKeystates[3] = true;
            break;            
    }
}

void specialUp(int key, int x, int y)
{
    switch(key) {
        case GLUT_KEY_LEFT:
            sKeystates[0] = false;
            break;
        case GLUT_KEY_UP:
            sKeystates[1] = false;
            break;
        case GLUT_KEY_RIGHT:
            sKeystates[2] = false;
            break;
        case GLUT_KEY_DOWN:
            sKeystates[3] = false;
            break;            
    }
}

// Initialize glut 
void initGlut()
{
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);    
    glutInitWindowSize(GLOBAL_WIDTH, GLOBAL_HEIGHT);
    glutInitWindowPosition(2000, 50);    
    glutCreateWindow("Final Project");   
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialDown);
    glutSpecialUpFunc(specialUp);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutDisplayFunc(display);
    glutTimerFunc(17, timer, 0);
}

// Initialize program
void init()
{
    initGlut();
    glClearColor(0.6, 0.6, 0.6, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

}

// main function - entry of the program
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    init();
    boat = Boat(Point3D(200, 15, 0), Mesh::createFromOBJ("obj/fishingTex.obj"), Vec3D(0, 180, 0), 100, 0.3, 0.7, Camera(Point3D(-5, 2, 0), Vec3D::createVector(Point3D(-5, 10, 0), Point3D()), 45));
    glutMainLoop();
    return (0);
}