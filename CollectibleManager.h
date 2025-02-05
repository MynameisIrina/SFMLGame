#pragma once
#include "Collectible.h"
#include "TextureLoader.h"

class CollectibleManager
{

public: 

CollectibleManager(const std::shared_ptr<TextureLoader> txLoader);  
void AddCollectible(std::unique_ptr<Collectible> collectible);
void Draw(const std::shared_ptr<sf::RenderWindow> &window) const;
std::unique_ptr<Collectible> CreateCoin(const sf::Vector2f& position);
void UpdateCollectibles(float dt);

private:

sf::Sprite sprite;
std::vector<std::unique_ptr<Collectible>> collectibles;

};