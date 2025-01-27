#include "Obstacle.h"
#include "Tile.h"
#include "Level.h"
#include "Utilities.h"


void Obstacle::Initialize(sf::Sprite& sprite, sf::Vector2f startPosition, float speed, float minX, float maxX)
{
    this->sprite = sprite;
    this->position = startPosition;
    this->speed = speed;
    this->minX = minX;
    this->maxX = maxX;
    rotation = 0;

    this->sprite.setOrigin(this->sprite.getGlobalBounds().width / 2, this->sprite.getGlobalBounds().height / 2);
    boundingBox = Utilities::CreateBoundingBox(this->sprite, this->position);
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

sf::RectangleShape Obstacle::GetBoundingBox() const
{
    return boundingBox;
}

void Obstacle::CreateVisualLine(float minX, float maxX, float minY, float maxY)
{
    line[0] = sf::Vertex(sf::Vector2f(minX, minY), sf::Color::Black);
    line[1] = sf::Vertex(sf::Vector2f(maxX, maxY), sf::Color::Black);
}

void Obstacle::Draw(const std::shared_ptr<sf::RenderWindow> window) const
{
    window->draw(sprite);
    window->draw(boundingBox);
    window->draw(line, 2, sf::Lines);
}
