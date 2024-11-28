#include "Player.h"
#include <iostream>

void Player::Initialize(const sf::Vector2f &pos)
{
    speed = 0.05f;
    velocity = sf::Vector2f(0.f, 0.f);
    currentAnim = 0;
    animationTimer = 0.f;
    animationInterval = 0.1f;
    position = pos;

    if (texture.loadFromFile("SFMLGame/Assets/Player/Textures/Cat-Sheet.png"))
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

void Player::Update(float dt)
{
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

}

void Player::Move(bool moveRight, bool moveLeft)
{
    if (moveRight)
    {
        position.x += 0.01f;
    }
    else if (moveLeft)
    {
        // move the player
        if (position.x - (32 / 2) > 0)
        {
            position.x -= 0.01f;
        }
    }
}

void Player::UpdateView(bool moveRight, bool moveLeft)
{
    int YIndex = 4;

   if(moveRight)
   {
        sprite.setScale(1 * 3, 1 * 3);
        sprite.setTextureRect(sf::IntRect(currentAnim * 32, YIndex * 32, 32, 32));
        sprite.setPosition(position);
   }
   if(moveLeft)
   {
        sprite.setScale(-1 * 3, 1 * 3);
        sprite.setTextureRect(sf::IntRect(currentAnim * 32, YIndex * 32, 32, 32));
        sprite.setPosition(position);
   }


}

void Player::Draw(sf::RenderTarget &rt) const
{
    rt.draw(sprite);
}

sf::Vector2f Player::GetPosition() const
{
    return position;
}
