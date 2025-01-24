#pragma once
#include <SFML/Graphics.hpp>
class Player;

class Camera
{

private:
    sf::View view;
    std::shared_ptr<sf::RenderWindow> window;

public:
    Camera(const std::shared_ptr<sf::RenderWindow>& window);
    void Initialize();
    void Update(const std::shared_ptr<Player>& player);
    sf::View GetView() const;
    int CalculateLeftBound() const;
};