#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include <optional>

class RayCast
{
public:

    struct Ray
    {
        sf::Vector2f start;
        sf::Vector2f end;

        Ray(sf::Vector2f startRay, sf::Vector2f endRay) : start(startRay), end(endRay) {}
    };

    struct RayCastHit
    {
        sf::Vector2f position;
        sf::RectangleShape hitRect;
    };

    static std::optional<RayCast::RayCastHit> DoRaycast(sf::Vector2f start, sf::Vector2f end, const std::vector<sf::RectangleShape> &otherShape);
};
