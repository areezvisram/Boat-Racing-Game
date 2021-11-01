#include <windows.h>
#include <event.h>

void Event::keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'q':
        exit(0);
        break;
    }
}