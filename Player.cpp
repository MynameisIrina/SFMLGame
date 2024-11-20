#include "Player.h"
#include <iostream>

void Player::Initialize()
{
    speed = 0.05f;
    velocity = sf::Vector2f(0.f, 0.f);
    isMovingRight = false;
    currentAnim = 0;
    animationTimer = 0.f;
    animationInterval = 0.1f;
}

void Player::Load(const sf::Vector2f &pos)
{
    if (texture.loadFromFile("Assets/Player/Textures/Cat-Sheet.png"))
    {
        std::cout << "Player image loaded" << std::endl;
        sprite.setTexture(texture);

        // pick an image from the sprite
        int XIndex = 0;
        int YIndex = 4;
        sprite.setTextureRect(sf::IntRect(XIndex * 32, YIndex * 32, 32, 32));

        // change the origin of the sprite
        sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
        sprite.setScale(3, 3);
        sprite.setPosition(pos);
    }
    else
    {
        std::cout << "Player image failed to load" << std::endl;
    }
}

void Player::Update()
{
}

void Player::Move(float &dt)
{
    int YIndex = 4;
    int XIndex = 0;

    // calculate current sprite sheet image
    animationTimer += dt;
    if (animationTimer >= animationInterval)
    {
        currentAnim++;
        if (currentAnim >= 8)
        {
            currentAnim = 0;
        }
        animationTimer = 0.f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        isMovingRight = true;
        // if the player moves to the right, set scale to (1,1) in order to rotate it
        sprite.setScale(1 * 3, 1 * 3);
        // calculate which animation image to take depending on the position on the x axis
        sprite.setTextureRect(sf::IntRect(currentAnim * 32, YIndex * 32, 32, 32));
        // move the player
        sprite.move(0.01f, 0);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        isMovingRight = false;
        // if the player moves to the left, set scale to (-1,1) in order to rotate it
        sprite.setScale(-1 * 3, 1 * 3);
        // move the player
        if (sprite.getPosition().x - (sprite.getGlobalBounds().width / 3) > 0)
        {
            // calculate which sprite sheet image to take depending on the position on the x axis
            sprite.setTextureRect(sf::IntRect(currentAnim * 32, YIndex * 32, 32, 32));
            sprite.move(-0.01f, 0);
        }
        else
        {
            sprite.setTextureRect(sf::IntRect(currentAnim * 32, YIndex * 32, 32, 32));
        }
    }
    else
    {
        isMovingRight = false;
    }
}

void Player::Draw(sf::RenderTarget &rt) const
{
    rt.draw(sprite);
}
