#pragma once
#include <SFML/Graphics.hpp>

class Player;
class Camera;

class Arrow
{

public:
    Arrow(const sf::Sprite& sprite);
    void Update(const std::shared_ptr<Player>& player,const std::shared_ptr<Camera>& camera, const float dt);
    sf::RectangleShape GetBoundingBox() const;
    void HandleMovement(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera>& camera, const float dt);
    void UpdateView();
    bool IsArrowActive();
    void HandleCollision(const std::shared_ptr<Player>& player, const std::shared_ptr<Camera> &camera);
    
    sf::Sprite sprite;
    bool isActive = false;
    sf::Vector2f position{0.f,0.f};
    float velocity = 0;
    float direction = 0;
    bool recentlyDeactivated = false;
    sf::RectangleShape boundingBox;

};
