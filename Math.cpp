#include "Math.h"

bool Math::CheckRectCollision(sf::FloatRect rect1, sf::FloatRect rect2)
{
    if(rect1.intersects(rect2))
    {
        return true;
    }
    return false;
}

// bool Math::CheckGroundCollision(sf::FloatRect rect1, sf::FloatRect rect2)
// {
//     if(rect1.top + rect1.height >= rect2.top){
//         return true;
//     }
//     return false;
// }
