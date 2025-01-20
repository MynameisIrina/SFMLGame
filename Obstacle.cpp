#include "Obstacle.h"
#include "Tile.h"
#include "Level_TileBased.h"


void Obstacle::Initialize(sf::Sprite sprite, sf::Vector2f startPosition, float speed, float minX, float maxX)
{
    this->sprite = sprite;
    this->position = startPosition;
    this->speed = speed;
    this->minX = minX;
    this->maxX = maxX;
}

void Obstacle::MoveObstacle(float dt)
{
    if (movingForward)
    {
        position.x += speed * dt;

        if (position.x >= maxX)
        {
            movingForward = false;
        }
    }
    else
    {
        position.x -= speed * dt;

        if (position.x <= minX)
        {
            movingForward = true;
        }
    }
}

void Obstacle::UpdateTexture()
{
    sprite.setPosition(position);
}

sf::Vector2f Obstacle::GetPosition() const
{
    return position;
}

sf::Sprite Obstacle::GetSprite() const
{
    return sprite;
}


sf::RectangleShape Obstacle::CreateBoundingRec()
{
    sf::RectangleShape boundingRec;
    boundingRec.setSize(sf::Vector2f(32, 32));
    boundingRec.setFillColor(sf::Color::Transparent);
    boundingRec.setOutlineColor(sf::Color::Blue);
    boundingRec.setOutlineThickness(1);
    boundingRec.setPosition(position);
    return boundingRec;
}
