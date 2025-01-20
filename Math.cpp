#include "Math.h"

bool Math::CheckRectCollision(sf::FloatRect rect1, sf::FloatRect rect2)
{
    if(rect1.intersects(rect2))
    {
        return true;
    }
    return false;
}
