#include "RayCast.h"

std::optional<RayCast::RayCastHit> RayCast::DoRaycast(sf::Vector2f start, sf::Vector2f end, std::vector<Tile> platforms)
{
    for (auto &platform : platforms)
    {
        sf::RectangleShape platformShape = platform.tileData.shape;

        bool rayCollision_Right = start.x < platformShape.getGlobalBounds().left + platformShape.getGlobalBounds().width &&
                                  end.x > platformShape.getGlobalBounds().left &&
                                  start.y > platformShape.getGlobalBounds().top &&
                                  start.y < platformShape.getGlobalBounds().top + platformShape.getGlobalBounds().height;

        bool rayCollision_Left = end.x < platformShape.getGlobalBounds().left + platformShape.getGlobalBounds().width &&
                                 start.x > platformShape.getGlobalBounds().left &&
                                 start.y > platformShape.getGlobalBounds().top &&
                                 start.y < platformShape.getGlobalBounds().top + platformShape.getGlobalBounds().height;

        if(rayCollision_Right || rayCollision_Left)
        {
            RayCastHit hit{end, platform.tileData.shape, platform.tileData.type};
            return hit;
        } 
    }

    return std::nullopt;
}

