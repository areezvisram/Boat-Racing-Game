#include <map/map.h>
#include <vector>
#include <map/wall.h>
#include <map/floor.h>
#include <iostream>

Map::Map(std::vector<Wall> walls, std::vector<Floor> floors)
{
    this->walls = walls;
    this->floors = floors;
}

void Map::render()
{
    for(Floor floor : floors) 
    {
        floor.draw();
    }

    for(Wall wall : walls)
    {
        wall.draw();
        break;
    }
}