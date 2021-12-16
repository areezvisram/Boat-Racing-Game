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

#include <screens/startScreen.h>
#include <helperFunctions.h>

StartScreen startScreen = StartScreen(800, 800, 200, 100, "Title Screen");

// Initialize glut 
void initGlut()
{
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    startScreen.createWindow();
    startScreen.setProperties();
}

// main function - entry of the program
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    initGlut();
    glutMainLoop();
    return (0);
}