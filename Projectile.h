#pragma once
#include <SFML/Graphics.hpp>

class Projectile
{

public:
    Projectile();
    void Update(float dt);
    void UpdateView();

    sf::Sprite sprite;
    sf::CircleShape circle;
    const float radius = 2.f;
    bool isActive = false;
    sf::Vector2f position;
    float velocity;
    bool recentlyDeactivated;

private:
};