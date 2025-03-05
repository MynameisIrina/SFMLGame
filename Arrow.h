#pragma once
#include <SFML/Graphics.hpp>
#include "TextureLoader.h"
#include "Player.h"

class Arrow
{

public:
    Arrow(sf::Sprite& sprite);
    void Update(const std::shared_ptr<Player>& player,const std::shared_ptr<Camera>& camera, const float dt);
    sf::RectangleShape GetBoundingBox() const;
    void HandleMovement(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera>& camera, const float dt);
    void UpdateView();
    bool IsArrowActive();
    void Reset();
    
    sf::Sprite sprite;
    bool isActive = false;
    sf::Vector2f position{0.f,0.f};
    float velocity = 0;
    float direction = 0;
    bool recentlyDeactivated = false;
    float vel = 0;
    sf::Vector2f previousPos;

private:
    sf::RectangleShape boundingBox;
};
