#ifndef FILE_READER_H
#define FILE_READER_H

#include <mathLib3D.h>
#include <string>
#include <vector>
#include <map/floor.h>
#include <map/wall.h>
#include <map/racePlane.h>

class FileReader
{
    public:
        FileReader(std::string filePath);

        std::string filePath;
        
        std::vector<Floor> readFloorVertices(Material material);
        std::vector<Wall> readWallVertices(Material material);
        std::vector<RacePlane> readRacePlaneVertices(Material material);
};

#endif