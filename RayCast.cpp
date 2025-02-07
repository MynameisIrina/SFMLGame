#include "RayCast.h"
#include "Math.h"

// std::optional<RayCast::RayCastHit> RayCast::DoRaycast(sf::Vector2f start, sf::Vector2f end, const std::vector<Tile> &platforms)
// {
//     for (auto &platform : platforms)
//     {
//         sf::RectangleShape platformShape = platform.GetShape();
//         Tile::Tile_Type type = platform.GetType();

//         bool rayCollision_Right = start.x < platformShape.getGlobalBounds().left + platformShape.getGlobalBounds().width &&
//                                   end.x > platformShape.getGlobalBounds().left &&
//                                   start.y > platformShape.getGlobalBounds().top &&
//                                   start.y < platformShape.getGlobalBounds().top + platformShape.getGlobalBounds().height;

//         bool rayCollision_Left = end.x < platformShape.getGlobalBounds().left + platformShape.getGlobalBounds().width &&
//                                  start.x > platformShape.getGlobalBounds().left &&
//                                  start.y > platformShape.getGlobalBounds().top &&
//                                  start.y < platformShape.getGlobalBounds().top + platformShape.getGlobalBounds().height;

//         if (rayCollision_Right || rayCollision_Left)
//         {
//             RayCastHit hit{end, platformShape, type};
//             return hit;
//         }
//     }

//     return std::nullopt;
// }

std::optional<RayCast::RayCastHit> RayCast::DoRaycast(sf::Vector2f start, sf::Vector2f end, const std::vector<sf::RectangleShape> &platforms)
{
    for (const auto &platform : platforms)
    {
        sf::FloatRect bounds = platform.getGlobalBounds();
        
        // Check intersection with the left side of the rectangle
        if ((start.x < bounds.left && end.x > bounds.left))
        {
            // intersection along the ray
            float t = (bounds.left - start.x) / (end.x - start.x);
            if (t >= 0 && t <= 1)
            {
                float y_intersection = start.y + t * (end.y - start.y);
                if (y_intersection >= bounds.top && y_intersection <= bounds.top + bounds.height)
                {
                    return RayCastHit{sf::Vector2f(bounds.left, y_intersection), platform};
                }
            }
        }

        // Check intersection with the right side of the rectangle
        if ((start.x > bounds.left + bounds.width && end.x < bounds.left + bounds.width))
        {
            // intersection along the ray
            float t = (bounds.left + bounds.width - start.x) / (end.x - start.x);
            if (t >= 0 && t <= 1)
            {
                float y_intersection = start.y + t * (end.y - start.y);
                if (y_intersection >= bounds.top && y_intersection <= bounds.top + bounds.height)
                {
                    return RayCastHit{sf::Vector2f(bounds.left + bounds.width, y_intersection), platform};
                }
            }
        }

        // Check intersection with the top side of the rectangle
        if ((start.y < bounds.top && end.y > bounds.top))
        {
            // intersection along the ray
            float t = (bounds.top - start.y) / (end.y - start.y);
            if (t >= 0 && t <= 1)
            {
                float x_intersection = start.x + t * (end.x - start.x);
                if (x_intersection >= bounds.left && x_intersection <= bounds.left + bounds.width)
                {
                    return RayCastHit{sf::Vector2f(x_intersection, bounds.top), platform};
                }
            }
        }

        // Check intersection with the bottom side of the rectangle
        if ((start.y > bounds.top + bounds.height && end.y < bounds.top + bounds.height))
        {
            // intersection along the ray
            float t = (bounds.top + bounds.height - start.y) / (end.y - start.y);
            if (t >= 0 && t <= 1)
            {
                float x_intersection = start.x + t * (end.x - start.x);
                if (x_intersection >= bounds.left && x_intersection <= bounds.left + bounds.width)
                {
                    return RayCastHit{sf::Vector2f(x_intersection, bounds.top + bounds.height), platform};
                }
            }
        }
    }

    return std::nullopt;
}
