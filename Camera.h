#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

class Camera
{

private:
    sf::View view;
    const Player &player;
    sf::RenderWindow &window;
    float viewThreshold;
    bool isCameraFollowing;

public:
    Camera(sf::RenderWindow &window, const Player &player);
    void Initialize();
    void Update(bool moveLeft);
    sf::View GetView() const;
    float CalculateLeftBound() const;
    float CalculateRightBound() const;
};