#pragma once
#include <SFML/Graphics.hpp>
#include "Camera.h"
#include "TextureLoader.h"

class Projectile
{

public:
    Projectile(const std::shared_ptr<TextureLoader> &txLoader);
    void Update(const std::shared_ptr<Camera> &camera, const float dt);
    void UpdateView();
    sf::Sprite GetSprite() const;
    bool IsRecentlyDeactivated() const;
    sf::CircleShape GetBoundingCircle() const;
    bool IsProjectileActive() const;

    sf::Vector2f position;
    float velocity;
    bool isActive = false;



private:
    sf::Sprite sprite;
    sf::CircleShape boundingCircle;
    bool recentlyDeactivated;
};