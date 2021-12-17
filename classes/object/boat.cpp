#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
    #include <GLUT/glut.h>
    #define MAX_SPEED 0.3
#else
    #include <windows.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
    #include <GL/freeglut.h>
    #define MAX_SPEED 0.1
#endif

#include <object/boat.h>
#include <cmath>
#include <float.h>
#include <iostream>

Boat::Boat() : Object(Point3D(), Mesh())
{
    rot = DirectionAngle();
    camera = Camera();
    dir = Vec3D(1,0,0);
    mass = 1;
    boundingBox = BoundingBox();
    std::vector<Point3D> a;
    colForce = Force();
    checkpoints = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};
}

Boat::Boat(Point3D pos, BoatType type, DirectionAngle rot, Camera camera) : Object(pos, Mesh())
{
    this->checkpoints = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};
    this->rot = rot;    
    this->camera = camera;
    this->dir = Vec3D(1,0,0);
    this->boundingBox = BoundingBox(Point3D(0,0,0), Vec3D(5,2.5,2.5), DirectionAngle(0,0));
    std::vector<Point3D> a;
    this->colForce = Force();
    switch(type)
    {
        case BoatType::FISHING:
            this->mesh = Mesh::createFromOBJ("objects/boat2.obj");
            this->mass = 250;            
            this->max_speed = MAX_SPEED;
            this->thrust_force_mag = 0.3;
            break;
        case BoatType::SPEED:
            this->mesh = Mesh::createFromOBJ("objects/speed.obj");
            this->mass = 100;            
            this->max_speed = MAX_SPEED;
            this->thrust_force_mag = 0.5;
            break;
        case BoatType::PIRATE:
            this->mesh = Mesh::createFromOBJ("objects/pirate.obj");
            this->mass = 500;            
            this->max_speed = MAX_SPEED;
            this->thrust_force_mag = 0.2;
            break;
        case BoatType::SMALLPIRATE:
            this->mesh = Mesh::createFromOBJ("objects/smallPirate.obj");
            this->mass = 325;            
            this->max_speed = MAX_SPEED;
            this->thrust_force_mag = 0.4; 
            break;                                    
    }
}

const float Boat::BREAK_FORCE_MAG = 0.5;
const float Boat::FRICTION_FORCE_MAG = 0.1;
// const float Boat::MIN_SPEED = 0.0001;
//const float Boat::MAX_SPEED = 0.3;
const float Boat::MAX_ANGULAR_SPEED = 1.0;

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

Force Boat::intersects(std::vector<Plane> planes, std::vector<Wall> walls)
{
    std::vector<Point3D> pois;
    Vec3D normal;
    for (long long unsigned int j = 0; j < walls.size(); j++)
    {
        Plane wall = walls[j].toPlane();
        // normal = normal.add(wall.normal).normalize();
        normal = wall.normal;
        for(long long unsigned int i = 0; i < planes.size(); i++)
        {
            // int i = 2;
            Plane p = Plane(planes[i].pos, planes[i].normal, planes[i].width, planes[i].height);
            Plane w = Plane(wall.pos, wall.normal.multiply(-1), wall.width, wall.height);
            // w.pos.y = p.pos.y;

            Vec3D itrans = Vec3D::createVector(p.pos, Point3D());
            DirectionAngle irot = p.normal.calcRotation();
            irot.alpha *= -1;
            irot.beta *= -1;

            //inverse translate
            p.pos = itrans.movePoint(p.pos);
            w.pos = itrans.movePoint(w.pos);

            // std::cout << p.pos.toString() << " " << p.normal.toString() << std::endl;
            // std::cout << w.pos.toString() << " " << w.normal.toString() << std::endl;

            //inverse rot
            p.normal = Vec3D::createVector(p.normal.calcRotation().add(irot), 1);
            w.normal = Vec3D::createVector(w.normal.calcRotation().add(irot), 1);
            Vec3D wPos = Vec3D::createVector(Point3D(), w.pos);
            wPos = Vec3D::createVector(wPos.calcRotation().add(irot), wPos.length());
            w.pos = wPos.movePoint(Point3D());

            // std::cout << p.pos.toString() << " " << p.normal.toString() << std::endl;
            // std::cout << w.pos.toString() << " " << w.normal.toString() << std::endl;

            if (std::abs(std::abs(Vec3D::dot(p.normal, w.normal))-1) <= FLT_EPSILON) //is parallel
            {
                // std::cout << "parallel: " << Vec3D::dot(p.normal, w.normal) << std::endl;
                continue;
                // return;
            }

            // float wDist = w.pos.distanceTo(Point3D());
            float wDist = -(w.normal.x*w.pos.x + w.normal.y*w.pos.y + w.normal.z*w.pos.z);
            float z = (-wDist * p.normal.x) / ((-p.normal.z * w.normal.x) + (w.normal.z * p.normal.x));
            // float z = p.normal.z / w.normal.z;
            float x = (-p.normal.z * z) / p.normal.x;

            if (std::abs(z) > p.width/2.0)
            {
                // std::cout << "out of bounds: " << z << " " << p.width/2.0 << std::endl;
                continue;
                // return;
            }
            if (Point3D(x,w.pos.y,z).distanceTo(w.pos) > w.width/2.0)
            {
                // std::cout << "out of wall bounds: " << Point3D(x,0,z).distanceTo(w.pos) << " " << w.width/2.0 << std::endl;
                // std::cout << "out of wall bounds: ";
                // std::cout << wall.width << std::endl;
                continue;
                // return;
            }

            // std::cout << x << " " << z << std::endl;
            Vec3D vb = Vec3D();
            switch(i)
            {
                case 0:
                case 1:
                    vb = planes[i].normal.multiply(this->boundingBox.size.x/2.0);
                    break;
                case 2:
                case 3:
                    vb = planes[i].normal.multiply(this->boundingBox.size.z/2.0);
                    break;
            }
            Point3D hit = Vec3D::createVector(planes[i].normal.calcRotation().add(DirectionAngle(90,0)), z).add(vb).movePoint(Point3D());
            pois.push_back(hit);
            // std::vector<Plane> ps;
            // ps.push_back(p);
            // ps.push_back(w);

            // return ps;
        }
    }
    Vec3D poi = Vec3D();
    for(Point3D point : pois)
    {
        poi.x += point.x;
        poi.y += point.y;
        poi.z += point.z;
    }
    if (pois.size() > 0)
    {
        poi.multiply(1.0 / pois.size());
    }

    // return poi.movePoint(Point3D());
    return Force(pois, normal);
}

int Boat::intersectsCheckpoint(std::vector<Plane> planes, std::vector<RacePlane> walls)
{
    std::vector<Point3D> pois;
    Vec3D normal;
    for (long long unsigned int j = 0; j < walls.size(); j++)
    {
        Plane wall = walls[j].toWall().toPlane();
        // normal = normal.add(wall.normal).normalize();
        normal = wall.normal;
        for(long long unsigned int i = 0; i < planes.size(); i++)
        {
            // int i = 2;
            Plane p = Plane(planes[i].pos, planes[i].normal, planes[i].width, planes[i].height);
            Plane w = Plane(wall.pos, wall.normal.multiply(-1), wall.width, wall.height);
            // w.pos.y = p.pos.y;

            Vec3D itrans = Vec3D::createVector(p.pos, Point3D());
            DirectionAngle irot = p.normal.calcRotation();
            irot.alpha *= -1;
            irot.beta *= -1;

            //inverse translate
            p.pos = itrans.movePoint(p.pos);
            w.pos = itrans.movePoint(w.pos);

            // std::cout << p.pos.toString() << " " << p.normal.toString() << std::endl;
            // std::cout << w.pos.toString() << " " << w.normal.toString() << std::endl;

            //inverse rot
            p.normal = Vec3D::createVector(p.normal.calcRotation().add(irot), 1);
            w.normal = Vec3D::createVector(w.normal.calcRotation().add(irot), 1);
            Vec3D wPos = Vec3D::createVector(Point3D(), w.pos);
            wPos = Vec3D::createVector(wPos.calcRotation().add(irot), wPos.length());
            w.pos = wPos.movePoint(Point3D());

            // std::cout << p.pos.toString() << " " << p.normal.toString() << std::endl;
            // std::cout << w.pos.toString() << " " << w.normal.toString() << std::endl;

            if (std::abs(std::abs(Vec3D::dot(p.normal, w.normal))-1) <= FLT_EPSILON) //is parallel
            {
                // std::cout << "parallel: " << Vec3D::dot(p.normal, w.normal) << std::endl;
                continue;
                // return;
            }

            // float wDist = w.pos.distanceTo(Point3D());
            float wDist = -(w.normal.x*w.pos.x + w.normal.y*w.pos.y + w.normal.z*w.pos.z);
            float z = (-wDist * p.normal.x) / ((-p.normal.z * w.normal.x) + (w.normal.z * p.normal.x));
            // float z = p.normal.z / w.normal.z;
            float x = (-p.normal.z * z) / p.normal.x;

            if (std::abs(z) > p.width/2.0)
            {
                // std::cout << "out of bounds: " << z << " " << p.width/2.0 << std::endl;
                continue;
                // return;
            }
            if (Point3D(x,w.pos.y,z).distanceTo(w.pos) > w.width/2.0)
            {
                // std::cout << "out of wall bounds: " << Point3D(x,0,z).distanceTo(w.pos) << " " << w.width/2.0 << std::endl;
                // std::cout << "out of wall bounds: ";
                // std::cout << wall.width << std::endl;
                continue;
                // return;
            }

            return j;
        }
    }
    return -1;
}

void Boat::update(bool forward, bool back, bool left, bool right, std::vector<Wall> walls, std::vector<RacePlane> racePlanes)
{      
    int cp = intersectsCheckpoint(calculatePlanes(), racePlanes);
    if (cp != -1 && this->checkpoints.size() > 0)
    {
        // std::cout << cp << std::endl;
        if (this->checkpoints[0] == cp)
        {
            this->checkpoints.erase(this->checkpoints.begin());
            std::cout << this->checkpoints.size() << std::endl;
        }
    }
    this->colForce = intersects(calculatePlanes(), walls);
    std::vector<Point3D> points = this->colForce.points;
    // std::cout << points.size() << std::endl;

    Point3D avg = Point3D();
    for(Point3D point : points)
    {
        avg.x += point.x;
        avg.y += point.y;
        avg.z += point.z;
    }
    if (points.size() > 0)
    {
        avg.x /= points.size();
        avg.y /= points.size();
        avg.z /= points.size();
    }


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
    if (this->colForce.points.size() > 0)
    {
        force = force.add(this->colForce.dir.multiply(0));
    }
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
    end = forwardVector().multiply(boundingBox.size.x / 2.0).multiply(-1).movePoint(pos);
    glVertex3f(end.x, end.y, end.z);
    end = Vec3D::createVector(forwardVector().calcRotation().add(DirectionAngle(90,0)), 1).multiply(boundingBox.size.z / 2.0).movePoint(pos);
    glVertex3f(end.x, end.y, end.z);
    // end = Vec3D::createVector(forwardVector().calcRotation().add(DirectionAngle(-90,0)), 1).multiply(boundingBox.size.z / 2.0).movePoint(pos);
    // glVertex3f(end.x, end.y, end.z);

    glEnd();

    // if (this->colForce.points.size() > 0)
    // {
    //     glLineWidth(10);
    //     glBegin(GL_LINES);
    //     glVertex3f(this->pos.x, this->pos.y, this->pos.z);
    //     Point3D end = this->colForce.dir.multiply(5).movePoint(this->pos);
    //     glVertex3f(end.x, end.y, end.z);
    //     glEnd();
    // }
    glTranslatef(pos.x, pos.y, pos.z);
    glRotatef(-this->rot.beta, 1,0,0);
    glRotatef(-this->rot.alpha, 0,1,0);
    glScalef(boundingBox.size.x,boundingBox.size.y,boundingBox.size.z);

    glutWireCube(1);

    glEnable(GL_LIGHTING);
    glPopMatrix();
}