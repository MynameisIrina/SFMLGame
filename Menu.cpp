#include "Menu.h"
#include <iostream>

Menu::Menu(std::shared_ptr<sf::RenderWindow> &window, std::shared_ptr<TextureLoader> &txLoader) : window(window)
{
    font.loadFromFile("Assets/Fonts/Pixel.ttf");

    startButton = Button(300, 230, 180, 50, "Start", font);
    exitButton = Button(300, 330, 180, 50, "Exit", font);

    title.setFont(font);
    title.setString("sf::Cat");
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::Black);
    title.setPosition(330, 100);

    cat = txLoader->SetSprite(TextureLoader::Player);
    cat.setScale(2,2);
    cat.setPosition(300, 198);
}

void Menu::Show(GameState &gameState, const float dt)
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
                if (startButton.IsClicked(sf::Mouse::getPosition(*window)))
                {
                    gameState = GameState::RUNNING;
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

void Menu::Draw()
{
    window->clear(sf::Color::White);
    startButton.Draw(window);
    exitButton.Draw(window);
    window->draw(title);
    window->draw(cat);
}

void Menu::UpdateCatAnimation(const float dt)
{
    animationTimer += dt;
        if (animationTimer >= animationInterval)
        {
            currentAnimationX++;

            if(currentAnimationX > maxFrames)
            {
                currentAnimationX = 0;
                currentAnimationY += diffBetweenAnimation;

                if(currentAnimationY > maxAnimationY)
                {
                    currentAnimationY = minAnimationY;
                }
            }
            animationTimer = 0.f;
        }

        cat.setTextureRect(sf::IntRect(currentAnimationX * frameSize + TextureLoader::playerOffsetX, currentAnimationY * TextureLoader::rectHeightPlayer, TextureLoader::rectWidthPlayer, TextureLoader::rectHeightPlayer));
}
