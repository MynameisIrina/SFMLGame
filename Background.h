#pragma once
#include <SFML/Graphics.hpp>

class Player;
class TextureLoader;

class Background
{
public:
    Background(const std::shared_ptr<TextureLoader>& txLoaderRef);
    void Initialize(const std::shared_ptr<sf::RenderWindow>&  window);
    void Draw(const std::shared_ptr<sf::RenderTarget>& rt) const;
    void GenerateNewSprite(const std::shared_ptr<Player>& player);
    void AddNewBackgroundSprite(float x, float scaleX, float scaleY);

private:
    std::shared_ptr<TextureLoader> txLoader;
    const float offsetPlacement = 1600.f;
    std::vector<sf::Sprite> sprites;
    sf::Vector2u windowSize;
    sf::Vector2u textureSize;
};