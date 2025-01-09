#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

class Camera
{

private:
    sf::View view;
    const std::shared_ptr<Player> player;
    std::shared_ptr<sf::RenderWindow> window;
    float viewThreshold;
    bool isCameraFollowing;

public:
    Camera(std::shared_ptr<sf::RenderWindow> window, const std::shared_ptr<Player> player);
    void Initialize();
    void Update(bool moveLeft, bool respawn);
    sf::View GetView() const;
    float CalculateLeftBound() const;
    float CalculateRightBound() const;
};