#include <fileReader.h>
#include <mathLib3D.h>
#include <string>
#include <fstream>
#include <vector>
#include <map/floor.h>
#include <map/wall.h>
#include <map/racePlane.h>
#include <iostream>
#include <material.h>

FileReader::FileReader(std::string filePath)
{
    this->filePath = filePath;
}

std::vector<std::string> split(const char *str, char c = ' ')
{
    std::vector<std::string> result;

    do
    {
        const char *begin = str;

        while(*str != c && *str)
            str++;

        result.push_back(std::string(begin, str));
    } while (0 != *str++);

    return result;
}


std::vector<Floor> FileReader::readFloorVertices(Material material)
{
    std::vector<Floor> floors;
    std::string line;
    std::ifstream floor (filePath);
    if(floor.is_open())
    {
        while(getline(floor, line))
        {
            long long unsigned int pos = 0;
            std::vector<Vec3D> vertices;
            std::vector<std::string> tokens;
            std::string token;
            while ((pos = line.find("|")) != std::string::npos)
            {
                token = line.substr(0, pos);
                tokens.push_back(token);
                line.erase(0, pos + 1);
            }
            tokens.push_back(line);
            std::string first = tokens.at(0);
            std::string second = tokens.at(1);
            std::string third = tokens.at(2);
            std::string fourth = tokens.at(3);

            tokens.pop_back();

            for(std::string token : tokens)
            {
                std::vector<std::string> coords = split(token.c_str(), ',');
                vertices.push_back(Vec3D(::atof(coords[0].c_str()), ::atof(coords[1].c_str()), ::atof(coords[2].c_str())));
            }
            Floor floorObj = Floor(vertices, material);
            floors.push_back(floorObj);
        }
        floor.close();
    }
    return floors;
}

std::vector<Wall> FileReader::readWallVertices(Material material)
{
    std::vector<Wall> walls;
    std::string line;
    std::ifstream wall (filePath);
    if(wall.is_open())
    {
        while(getline(wall, line))
        {
            long long unsigned int pos = 0;
            std::vector<Point3D> vertices;
            std::vector<std::string> tokens;
            std::string token;
            while ((pos = line.find("|")) != std::string::npos)
            {
                token = line.substr(0, pos);
                tokens.push_back(token);
                line.erase(0, pos + 1);
            }
            tokens.push_back(line);
            std::string first = tokens.at(0);
            std::string second = tokens.at(1);
            std::string third = tokens.at(2);
            std::string fourth = tokens.at(3);

            tokens.pop_back();

            for(std::string token : tokens)
            {
                std::vector<std::string> coords = split(token.c_str(), ',');
                vertices.push_back(Point3D(::atof(coords[0].c_str()), ::atof(coords[1].c_str()), ::atof(coords[2].c_str())));
            }
            Wall wallObj = Wall(vertices, material);
            walls.push_back(wallObj);
        }
        wall.close();
    }
    return walls;
}

std::vector<RacePlane> FileReader::readRacePlaneVertices(Material material)
{
    std::vector<RacePlane> racePlanes;
    std::string line;
    std::ifstream racePlane (filePath);
    if(racePlane.is_open())
    {
        while(getline(racePlane, line))
        {
            long long unsigned int pos = 0;
            std::vector<Point3D> vertices;
            std::vector<std::string> tokens;
            std::string token;
            while ((pos = line.find("|")) != std::string::npos)
            {
                token = line.substr(0, pos);
                tokens.push_back(token);
                line.erase(0, pos + 1);
            }
            tokens.push_back(line);
            std::string first = tokens.at(0);
            std::string second = tokens.at(1);
            std::string third = tokens.at(2);
            std::string fourth = tokens.at(3);

            tokens.pop_back();

            for(std::string token : tokens)
            {
                std::vector<std::string> coords = split(token.c_str(), ',');
                vertices.push_back(Point3D(::atof(coords[0].c_str()), ::atof(coords[1].c_str()), ::atof(coords[2].c_str())));
            }
            RacePlane racePlaneObj = RacePlane(vertices, material);
            racePlanes.push_back(racePlaneObj);
        }
        racePlane.close();
    }
    return racePlanes;
}