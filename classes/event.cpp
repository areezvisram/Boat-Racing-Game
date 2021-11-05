#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#else
#include <windows.h>
#endif
#include <event.h>
#include <iostream>

void Event::keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'q':
        exit(0);
        break;
    }
}