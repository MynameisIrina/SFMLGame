#pragma once
#include <SFML/Graphics.hpp>

class Tile {
public:
    enum Tile_Type {
        Empty = 0,
        Dirt = 1,
        Grass = 2,
        Obstacle = 3,
        Enemy = 4,
        Tree = 5
    };

    Tile(Tile_Type type, const sf::RectangleShape& shape)
        : type(type), shape(shape) {}

    Tile_Type GetType() const { return type; }
    const sf::RectangleShape& GetShape() const { return shape; }

Tile_Type type;
sf::RectangleShape shape;
};
