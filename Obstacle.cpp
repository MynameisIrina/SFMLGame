#include "Obstacle.h"

Obstacle::Obstacle(std::shared_ptr<TextureLoader> txLoaderRef): txLoader(txLoaderRef)
{ sprite = txLoader->SetSprite(TextureLoader::TextureType::Obstacle); }


void Obstacle::Initialize(sf::Vector2f startPosition, float speed, float minX, float maxX)
{
    this->position = startPosition;
    this->speed = speed;
    this->minX = minX;
    this->maxX = maxX;
    sprite.setPosition(position);
}


void Obstacle::MoveObstacle(float dt)
{
    if(movingForward)
    {
        position.x += speed * dt;

        if(position.x >= maxX)
        {
            movingForward = false;
        }
    }
    else
    {
        position.x -= speed * dt;

        if(position.x <= minX)
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

