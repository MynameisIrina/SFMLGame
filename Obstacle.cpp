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
    boundingBox = CreateBoundingBox(); 
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
    boundingBox.setPosition(position);
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


sf::RectangleShape Obstacle::CreateBoundingBox()
{
    boundingBox.setSize(sf::Vector2f(32, 32));
    boundingBox.setFillColor(sf::Color::Transparent);
    boundingBox.setOutlineColor(sf::Color::Blue);
    boundingBox.setOutlineThickness(1);
    boundingBox.setOrigin(this->sprite.getGlobalBounds().width / 2, this->sprite.getGlobalBounds().height / 2);
    boundingBox.setPosition(position);
    return boundingBox;
}

sf::RectangleShape Obstacle::GetBoundingBox() const
{
    return boundingBox;
}
