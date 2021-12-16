#ifndef MAP_H
#define MAP_H

#include <map/floor.h>
#include <map/wall.h>
#include <map/racePlane.h>
#include <vector>

class Map
{
    public:
        Map(std::vector<Wall> walls, std::vector<Floor> floors, std::vector<RacePlane> racePlanes);

        std::vector<Wall> walls;
        std::vector<Floor> floors;
        std::vector<RacePlane> racePlanes;

        void render();
        void loadTextures();
};


#endif