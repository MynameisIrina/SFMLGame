#pragma once
#include <SFML/Graphics.hpp>

class Tile
{
public:

    enum Tile_Type
    {
        Empty = 0,
        Dirt = 1,
        Grass = 2,
        Obstacle = 3
    };

    struct TileData
    {
        Tile_Type type;
        sf::RectangleShape shape;

        TileData(Tile_Type type, const sf::RectangleShape &shape)
            : type(type), shape(shape) {}
    };

    TileData tileData;
    Tile(Tile_Type type, const sf::RectangleShape& shape): tileData(type, shape){}
};