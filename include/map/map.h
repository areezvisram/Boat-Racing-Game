#ifndef MAP_H
#define MAP_H

#include <map/floor.h>
#include <map/wall.h>
#include <vector>

class Map
{
    public:
        Map(std::vector<Wall> walls, std::vector<Floor> floors);

        std::vector<Wall> walls;
        std::vector<Floor> floors;

        void render();
        void loadTextures();
};


#endif