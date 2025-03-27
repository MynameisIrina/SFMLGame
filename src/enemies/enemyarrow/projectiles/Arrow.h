#pragma once
#include <SFML/Graphics.hpp>

class Player;
class Camera;

class Arrow
{

public:

    enum Direction
    {
        Left = -1,
        Right = 1
    };

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
    int velocity = 0;
    Direction direction = Direction::Left;
    bool recentlyDeactivated = false;
    sf::RectangleShape boundingBox;

};
