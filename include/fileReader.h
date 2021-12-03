#ifndef FILE_READER_H
#define FILE_READER_H

#include <mathLib3D.h>
#include <string>
#include <vector>
#include <map/floor.h>
#include <map/wall.h>

class FileReader
{
    public:
        FileReader(std::string filePath);

        std::string filePath;
        
        std::vector<Floor> readFloorVertices(Material material);
        std::vector<Wall> readWallVertices(Material material);
    
};

#endif