#include "Obstacle.h"
#include "Tile.h"
#include "Level_TileBased.h"


void Obstacle::Initialize(sf::Sprite& sprite, sf::Vector2f startPosition, float speed, float minX, float maxX)
{
    this->sprite = sprite;
    this->position = startPosition;
    this->speed = speed;
    this->minX = minX;
    this->maxX = maxX;
    this->sprite.setOrigin(this->sprite.getGlobalBounds().width / 2, this->sprite.getGlobalBounds().height / 2);
    rotation = 0;
}

void Obstacle::MoveObstacle(float dt)
{
    if (movingForward)
    {
        position.x += speed * dt;
        rotation += rotationSpeed;

        if (position.x >= maxX)
        {
            movingForward = false;
        }
    }
    else
    {
        position.x -= speed * dt;
        rotation -= rotationSpeed;

        if (position.x <= minX)
        {
            movingForward = true;
        }
    }
}

void Obstacle::UpdateTexture()
{
    sprite.setPosition(position);
    boundingRec.setPosition(position);
    sprite.setRotation(rotation);
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
    boundingRec.setSize(sf::Vector2f(32, 32));
    boundingRec.setFillColor(sf::Color::Transparent);
    boundingRec.setOutlineColor(sf::Color::Blue);
    boundingRec.setOutlineThickness(1);
    boundingRec.setOrigin(this->sprite.getGlobalBounds().width / 2, this->sprite.getGlobalBounds().height / 2);
    boundingRec.setPosition(position);
    return boundingRec;
}

sf::RectangleShape Obstacle::GetBoundingRec()
{
    return boundingRec;
}
