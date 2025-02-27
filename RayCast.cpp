#include "RayCast.h"
#include "Math.h"

std::optional<RayCast::RayCastHit> RayCast::DoRaycast(sf::Vector2f start, sf::Vector2f end, const std::vector<sf::RectangleShape> &platforms)
{
    for (const auto &platform : platforms)
    {
        sf::FloatRect bounds = platform.getGlobalBounds();

        /*
            check right direction && collision left
        */
        if (start.x <= bounds.left + bounds.width && end.x >= bounds.left + bounds.width)
        {
            if (start.y < bounds.top + bounds.height && end.y > bounds.top)
            {
                return RayCastHit{sf::Vector2f(bounds.left + bounds.width, bounds.top), platform};
            }
        }


        /*
            check left direction && collision left
        */
        if (start.x >= bounds.left + bounds.width && end.x <= bounds.left + bounds.width)
        {
            if (start.y < bounds.top + bounds.height && end.y > bounds.top)
            {
                return RayCastHit{sf::Vector2f(bounds.left + bounds.width, bounds.top), platform};
            }
        }

        
        /*
            check right direction && collision right
        */
        if (start.x >= bounds.left && end.x <= bounds.left)
        {
            if (start.y < bounds.top + bounds.height && end.y > bounds.top)
            {
                return RayCastHit{sf::Vector2f(bounds.left, bounds.top), platform};
            }
        }

        /*
            check right direction && collision right
        */
        if (start.x <= bounds.left && end.x >= bounds.left)
        {
            if (start.y < bounds.top + bounds.height && end.y > bounds.top)
            {
                //std::cout << "right direction && collision right" << std::endl;
                return RayCastHit{sf::Vector2f(bounds.left, bounds.top), platform};
            }
        }
    }

    return std::nullopt;
}
