#pragma once
#include "TextureLoader.h"
#include "Camera.h"

class Player;

class HealthBar
{

public:
    struct Heart
    {
        sf::Sprite sprite;
        sf::Vector2f position;
        bool isActive;
    };

    HealthBar(const std::shared_ptr<TextureLoader>& txLoader);
    void Initialize(const std::shared_ptr<Player>& player, const std::shared_ptr<Camera>& camera);
    void Draw(const std::shared_ptr<sf::RenderTarget>& rt) const;
    void Update(const std::shared_ptr<Player>& player, const std::shared_ptr<Camera>& camera);
    void UpdateView();

private:
    std::vector<Heart> hearts;
    std::shared_ptr<TextureLoader> txLoader;
    float offsetX = 200.f;
    float offsetY = 10.f;
    float scale = 2;
    sf::Color opaque = sf::Color(255,255,255, 255);
    sf::Color transparent = sf::Color(255,255,255, 0);

};