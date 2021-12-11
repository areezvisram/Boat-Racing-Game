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

#include <helperFunctions.h>
#include <vector>

// Function that renders text onto the screen as a bitmap at the given (x,y) position in the given font
void renderText(float x, float y, void *font, const char *string, float scaleFactor) 
{  
    const char *character;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    //glRasterPos2f(x,y);
    glTranslatef(x,y,0);
    glColor3f(1.0f, 1.0f, 1.0f);
    glScalef(scaleFactor, scaleFactor, scaleFactor);
    for (character=string; *character != '\0'; character++) 
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *character);
    }

    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}