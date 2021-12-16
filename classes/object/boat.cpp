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

#include <object/boat.h>
#include <cmath>
#include <iostream>

Boat::Boat() : Object(Point3D(), Mesh())
{
    rot = DirectionAngle();
    camera = Camera();
    dir = Vec3D(1,0,0);
    mass = 1;
    boundingBox = BoundingBox();
}

Boat::Boat(Point3D pos, BoatType type, Mesh mesh, DirectionAngle rot, float mass, float max_speed, float thrust_force_mag, Camera camera) : Object(pos, mesh)
{
    this->rot = rot;    
    this->camera = camera;
    this->dir = Vec3D(1,0,0);
    this->boundingBox = BoundingBox(Point3D(0,0,0), Vec3D(5,2.5,2.5), DirectionAngle(0,0));
    switch(type)
    {
        case BoatType::FISHING:
            this->mesh = Mesh::createFromOBJ("obj/boat2.obj");
            this->mass = 250;
            this->max_speed = 0.5;
            this->thrust_force_mag = 0.3;
            break;
        case BoatType::SPEED:
            this->mesh = Mesh::createFromOBJ("obj/speed.obj");
            this->mass = 100;
            this->max_speed = 0.5;
            this->thrust_force_mag = 0.5;
            break;
        case BoatType::PIRATE:
            this->mesh = Mesh::createFromOBJ("obj/pirate.obj");
            this->mass = 500;
            this->max_speed = 0.2;
            this->thrust_force_mag = 0.2;
            break;
        case BoatType::SMALLPIRATE:
            this->mesh = Mesh::createFromOBJ("obj/smallPirate.obj");
            this->mass = 325;
            this->max_speed = 0.4;
            this->thrust_force_mag = 0.4; 
            break;                                    
    }
}

const float Boat::BREAK_FORCE_MAG = 0.5;
const float Boat::FRICTION_FORCE_MAG = 0.1;
// const float Boat::MIN_SPEED = 0.0001;
//const float Boat::MAX_SPEED = 0.3;
const float Boat::MAX_ANGULAR_SPEED = 1.0;

void Boat::update(bool forward, bool back, bool left, bool right)
{      

    thrusting = forward;
    breaking = back;
    angularAcc = Vec3D();
    if (left)
    {
        angularAcc = Vec3D(0,-0.05,0);
    }
    if (right)
    {
        angularAcc = Vec3D(0,0.05,0);
    }
    angularVel = angularVel.add(angularAcc);
    if (std::abs(angularVel.y) > MAX_ANGULAR_SPEED)
    {
        angularVel.y = (angularVel.y / std::abs(angularVel.y)) * MAX_ANGULAR_SPEED;
    }
    //angular friction
    if (std::abs(angularVel.y) > 0.025)
    {
        if (angularVel.y > 0)
        {
            angularVel = angularVel.add(Vec3D(0,-0.025,0));
        }
        else if (angularVel.y < 0)
        {
            angularVel = angularVel.add(Vec3D(0,0.025,0));
        }
    }
    else
    {
        angularVel.y = 0;
    }
    rot = rot.add(DirectionAngle(angularVel.y, 0));

    Vec3D force = sumForces();
    acc = force.multiply(1.0 / mass);
    Vec3D vel = dir.multiply(speed);
    vel = vel.add(acc);
    speed = vel.length();
    if (speed >= max_speed)
    {
        speed = max_speed;
    }
    dir = vel.normalize();
    pos = vel.movePoint(pos);
}

Vec3D Boat::forwardVector()
{
    Vec3D straight = Vec3D(1,0,0);
    Vec3D rotVec = Vec3D(
        straight.x * cosf(-Vec3D::toRadians(rot.alpha)) - straight.z * sinf(-Vec3D::toRadians(rot.alpha)),
        0,
        -(straight.x * sinf(-Vec3D::toRadians(rot.alpha)) + straight.z * cosf(-Vec3D::toRadians(rot.alpha)))
    );
    return rotVec;
}

Vec3D Boat::sumForces()
{
    Vec3D total = Vec3D();

    if (thrusting)
    {
        total = total.add(forwardVector().multiply(thrust_force_mag));
    }

    if (breaking)
    {
        total = total.add(forwardVector().multiply(-1 * BREAK_FORCE_MAG));
    }

    total = total.add(dir.multiply(-1 * FRICTION_FORCE_MAG));

    return total;
}

void Boat::drawBoundingBoxes()
{
    glPushMatrix();
    // glLoadIdentity();
    glTranslatef(-pos.x, -pos.y, -pos.z);
    glDisable(GL_LIGHTING);
    glPointSize(10);
    glColor3f(1,0,0);
    glBegin(GL_POINTS);
    glVertex3f(pos.x, pos.y, pos.z);

    Point3D end = forwardVector().multiply(boundingBox.size.x / 2.0).movePoint(pos);
    glVertex3f(end.x, end.y, end.z);
    // end = forwardVector().multiply(boundingBox.size.x / 2.0).multiply(-1).movePoint(pos);
    // glVertex3f(end.x, end.y, end.z);
    // end = Vec3D::createVector(forwardVector().calcRotation().add(DirectionAngle(90,0)), 1).multiply(boundingBox.size.z / 2.0).movePoint(pos);
    // glVertex3f(end.x, end.y, end.z);
    // end = Vec3D::createVector(forwardVector().calcRotation().add(DirectionAngle(-90,0)), 1).multiply(boundingBox.size.z / 2.0).movePoint(pos);
    // glVertex3f(end.x, end.y, end.z);

    glEnd();

    glTranslatef(pos.x, pos.y, pos.z);
    glRotatef(-this->rot.beta, 1,0,0);
    glRotatef(-this->rot.alpha, 0,1,0);
    glScalef(boundingBox.size.x,boundingBox.size.y,boundingBox.size.z);

    glutWireCube(1);

    glEnable(GL_LIGHTING);
    glPopMatrix();
}

std::vector<Plane> Boat::calculatePlanes()
{
    std::vector<Plane> out;

    Vec3D n = forwardVector();
    Point3D planePos = n.multiply(boundingBox.size.x / 2.0).movePoint(pos);
    out.push_back(Plane(planePos, n, boundingBox.size.z, boundingBox.size.y / 2.0));

    n = forwardVector().multiply(-1);
    planePos = n.multiply(boundingBox.size.x / 2.0).movePoint(pos);
    out.push_back(Plane(planePos, n, boundingBox.size.z, boundingBox.size.y / 2.0));

    // n = forwardVector();
    n = Vec3D::createVector(forwardVector().calcRotation().add(DirectionAngle(90,0)), 1);
    planePos = n.multiply(boundingBox.size.z / 2.0).movePoint(pos);
    out.push_back(Plane(planePos, n, boundingBox.size.x, boundingBox.size.y / 2.0));

    n = Vec3D::createVector(forwardVector().calcRotation().add(DirectionAngle(-90,0)), 1);
    planePos = n.multiply(boundingBox.size.z / 2.0).movePoint(pos);
    out.push_back(Plane(planePos, n, boundingBox.size.x, boundingBox.size.y / 2.0));

    return out;
}

void Boat::intersects(std::vector<Plane> planes, Plane wall)
{

    // for(long long unsigned int i = 0; i < planes.size(); i++)
    // {
        int i = 0;
        Plane p = Plane(planes[i].pos, planes[i].normal, planes[i].width, planes[i].height);
        Plane w = Plane(wall.pos, wall.normal, wall.width, wall.height);

        Vec3D itrans = Vec3D::createVector(p.pos, Point3D());
        DirectionAngle irot = p.normal.calcRotation();
        irot.alpha *= -1;
        irot.beta *= -1;

        //inverse translate
        p.pos = itrans.movePoint(p.pos);
        w.pos = itrans.movePoint(w.pos);

        std::cout << p.pos.toString() << " " << p.normal.toString() << std::endl;
        std::cout << w.pos.toString() << " " << w.normal.toString() << std::endl;

    // }
}