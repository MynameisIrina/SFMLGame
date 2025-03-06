#include "Obstacle.h"
#include "Tile.h"
#include "Level.h"
#include "Utilities.h"


void Obstacle::Initialize(const sf::Sprite& sprite, const sf::Vector2f& startPosition, const float speed, const float minX, const float maxX)
{
    this->sprite = sprite;
    position = startPosition;
    this->speed = speed;
    this->minX = minX;
    this->maxX = maxX;
    rotation = 0;

    const sf::FloatRect localBounds = this->sprite.getLocalBounds();
    this->sprite.setOrigin(localBounds.width * 0.5f, localBounds.height * 0.5f);

    boundingBox = Utilities::CreateBoundingBox(this->sprite, position);

    boundingBox.setSize(sf::Vector2f(boundingBox.getSize().x * 0.7f, boundingBox.getSize().y * 0.7f));

    // Adjust vertical position to account for bounding box height
    this->position.y += boundingBox.getGlobalBounds().height * 0.8f;

    CreateVisualLine(minX, maxX, position.y , position.y);
    
}

void Obstacle::MoveObstacle(float dt)
{
    const float deltaMove = speed * dt;
    const float deltaRotation = rotationSpeed * (movingForward ? 1.0f : -1.0f);

    position.x += movingForward ? deltaMove : -deltaMove;
    rotation += deltaRotation;

    movingForward = (position.x >= maxX) ? false : 
                    (position.x <= minX) ? true : movingForward;


    UpdateTexture();
}

void Obstacle::UpdateTexture()
{
    sprite.setPosition(position);
    boundingBox.setPosition(position.x + boundingBox.getSize().x * 0.25f, position.y + boundingBox.getSize().y * 0.25f);
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

void Obstacle::CreateVisualLine(const float minX, const float maxX, const float minY, const float maxY)
{
    line[0] = sf::Vertex(sf::Vector2f(minX, minY), sf::Color::Black);
    line[1] = sf::Vertex(sf::Vector2f(maxX, maxY), sf::Color::Black);
}

void Obstacle::Draw(const std::shared_ptr<sf::RenderWindow>& window) const
{
    window->draw(sprite);
    //window->draw(boundingBox);
    window->draw(line, 2, sf::Lines);
}
