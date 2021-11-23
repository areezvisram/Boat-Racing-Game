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

float mat_ambient [4] ={ 0.329412f, 0.223529f, 0.027451f,1.0f };
float mat_diffuse [4] ={ 0.780392f, 0.568627f, 0.113725f, 1.0f };
float mat_specular [4] ={ 0.992157f, 0.941176f, 0.807843f, 1.0f };
float shine = 27.8974f;

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

void Floor::draw() {
    glBegin(GL_POLYGON);
    for(Vec3D vertex : vertices) {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SPECULAR, shine); 
        glVertex3f(vertex.x, vertex.y, vertex.z);
    }
    glEnd();
}