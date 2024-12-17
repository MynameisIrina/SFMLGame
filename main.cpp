#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#include "Background.h"
#include "Perlin.h"
#include "Level_TileBased.h"
#include "Camera.h"

int main()
{
    // ----------------- INITIALIZE -----------------
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 600), "SFML game", sf::Style::Default, settings);
    std::unique_ptr<sf::Uint8[]> pixels = std::make_unique<sf::Uint8[]>(800 * 600 * 4);
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    sf::Clock timer;
    float deltaTime = 0.0f;

    std::shared_ptr<Player> player = std::make_shared<Player>();
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(window, player);

    Background background(player);
    background.Initialize(window);
    player->Initialize(sf::Vector2f(50.f, 502.f));
    camera->Initialize();

    Level_TileBased level_tile(player, camera);
    level_tile.Initialize();

    // ----------------- INITIALIZE -----------------

    while (window->isOpen())
    {
        //  ----------------- UPDATE -----------------
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
        }

        deltaTime = timer.restart().asSeconds();

        bool moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        bool moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        bool jumped = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

        background.Move(deltaTime);
        background.UpdateView();

        float leftBound = camera->CalculateLeftBound();
        player->Move(moveRight, moveLeft, deltaTime, leftBound);
        player->Jump(jumped, deltaTime);
        player->Update(deltaTime);
        player->UpdateView(moveRight, moveLeft);
        level_tile.UpdateLevel();

        camera->Update(moveLeft);

        // ----------------- UPDATE----------------

        // ----------------- DRAW -----------------
        window->clear();
        background.Draw(window);
        player->Draw(window);
        level_tile.Draw(window);
        window->display();
        // ----------------- DRAW -----------------
    }

    return 0;
}