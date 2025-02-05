#include "Coin.h"
#include "Enemy.h"
#include "Utilities.h"

void Coin::Initialize(sf::Sprite &sprite, const sf::Vector2f position)
{
    this->sprite = sprite;
    this->position = position;
    this->sprite.setPosition(position);

    const auto &bounds = this->sprite.getLocalBounds();
    this->sprite.setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);

    boundingBox = Utilities::CreateBoundingBox(this->sprite, this->position);
}

void Coin::Update(const std::shared_ptr<Player>& player, const float dt)
{
    Collectible::Update(player, dt);
    UpdateAnimation(dt);
    UpdateView();
}   

void Coin::UpdateAnimation(float dt)
{
    animationTimer += dt;
    if (animationTimer >= animationInterval)
    {
        if (currentAnim >= maxFrames)
        {
            currentAnim = 0;
        }

        currentAnim++;
        animationTimer = 0.f;
    }
}

void Coin::UpdateView()
{
    sprite.setTextureRect(sf::IntRect(currentAnim * frameSize + TextureLoader::coinOffsetX , 0, TextureLoader::rectWidthCoin, TextureLoader::rectHeightCoin));
}

void Coin::Pickup(const std::shared_ptr<Player> &player) {}

void Coin::ConvertEnemyToCoin(sf::Sprite &sprite, const sf::Vector2f position)
{
    Initialize(sprite, position);
}
