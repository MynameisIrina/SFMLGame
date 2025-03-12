#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "TextureLoader.h"
#include "Button.h"

class WinScreen
{

public:
    WinScreen(const std::shared_ptr<sf::RenderWindow> &window, const std::shared_ptr<TextureLoader> &txLoader);
    void Show(GameState &gameState, const float dt);
    void Draw();
    void UpdateCatAnimation(const float dt);

private:
    std::shared_ptr<sf::RenderWindow> window;
    Button restartButton;
    Button exitButton;
    sf::Text message;
    sf::Font font;
    sf::Sprite cat;
    float animationTimer = 0.f;
    const int maxFrames = 8;
    const float animationInterval = 0.2f;
    int currentAnimationX = 0;
    int currentAnimationY = 0;
    const int frameSize = 32;
};