#include "CollectibleManager.h"
#include "Coin.h"

CollectibleManager::CollectibleManager(const std::shared_ptr<TextureLoader> txLoader)
{
    this->sprite = txLoader->SetSprite(TextureLoader::Coin);
}

void CollectibleManager::AddCollectible(std::unique_ptr<Collectible> collectible)
{
    collectibles.push_back(std::move(collectible));
}

void CollectibleManager::Draw(const std::shared_ptr<sf::RenderWindow> &window) const
{
    for (const auto &collectible : collectibles)
    {
        collectible->Draw(window);
    }
}

std::unique_ptr<Collectible> CollectibleManager::CreateCoin(const sf::Vector2f& position)
{
    auto coin = std::make_unique<Coin>();
    coin->Initialize(sprite, position);
    return coin;
}

void CollectibleManager::UpdateCollectibles(float dt)
{
    for (auto &collectible : collectibles)
    {
        collectible->Update(dt);
    }
}
