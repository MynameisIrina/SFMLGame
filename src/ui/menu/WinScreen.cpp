#include "ui/menu/WinScreen.h"
#include <iostream>
#include <string>

WinScreen::WinScreen(const std::shared_ptr<sf::RenderWindow> &window, const std::shared_ptr<TextureLoader> &txLoader) : window(window)
{
    font.loadFromFile("Assets/Fonts/Pixel.ttf");
    font.setSmooth(false);

    restartButton = Button(300, 230, 180, 50, "Restart", font);
    exitButton = Button(300, 330, 180, 50, "Exit", font);

    message.setFont(font);
    message.setString("You Won!");
    message.setCharacterSize(50);
    message.setFillColor(sf::Color::Black);
    message.setPosition(310, 100);

    cat = txLoader->SetSprite(TextureLoader::Player);
    cat.setScale(2, 2);
    cat.setPosition(300, 166);
}

void WinScreen::Show(GameState &gameState, const float dt)
{
    sf::Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window->close();
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                if (restartButton.IsClicked(sf::Mouse::getPosition(*window)))
                {
                    gameState = GameState::RESTART;
                }
                else if (exitButton.IsClicked(sf::Mouse::getPosition(*window)))
                {
                    gameState = GameState::EXIT;
                }
            }
        }

    }
    UpdateCatAnimation(dt);
}

void WinScreen::Draw()
{
    window->clear(sf::Color::White);
    window->draw(cat);
    restartButton.Draw(window);
    exitButton.Draw(window);
    window->draw(message);
}

void WinScreen::UpdateCatAnimation(const float dt)
{
    animationTimer += dt;
    if (animationTimer >= animationInterval)
    {
        currentAnimationX++;

        if (currentAnimationX >= maxFrames)
        {
            currentAnimationX = 0;
        }
        animationTimer = 0.f;
    }

    cat.setTextureRect(sf::IntRect(currentAnimationX * frameSize, currentAnimationY * TextureLoader::rectHeightPlayer, TextureLoader::rectWidthPlayer, TextureLoader::rectHeightPlayer));
}
