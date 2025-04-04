#pragma once
#include <SFML/Graphics.hpp>

class Player;

class Camera
{

private:
    sf::View view;
    std::shared_ptr<sf::RenderWindow> window;
    float topBound = 0;
    float bottomBound = 0;
    const float topBoundOffset = 40.f;
    const float bottomBoundOffset = 40.f;
    const float cameraSmoothingFactor = 5.0f;

public:
    Camera(const std::shared_ptr<sf::RenderWindow>& window);
    void Initialize();
    void Update(const std::shared_ptr<Player>& player, const float dt);
    void Reset();
    sf::View GetView() const;
    int CalculateLeftBound() const;
    int CalculateRightBound() const;
    int CalculateTopBound() const;
    int CalculateBottomBound() const;
};