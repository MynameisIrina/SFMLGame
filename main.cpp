#include <SFML/Graphics.hpp>
#include <iostream>
#include "TextureLoader.h"
#include "Player.h"
#include "Background.h"
#include "Perlin.h"
#include "Level_TileBased.h"
#include "Camera.h"
#include "Obstacle.h"
#include "HealthBar.h"

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

    std::shared_ptr<TextureLoader> txLoader = std::make_shared<TextureLoader>();
    txLoader->Initialize();

    std::shared_ptr<Player> player = std::make_shared<Player>(txLoader);
    player->Initialize(sf::Vector2f(35.f, 20.f));

    std::shared_ptr<Camera> camera = std::make_shared<Camera>(window);
    camera->Initialize();

    std::shared_ptr<HealthBar> healthBar = std::make_shared<HealthBar>(txLoader, camera);
    healthBar->Initialize(6);

    Level_TileBased level_tile(player, camera, txLoader);
    level_tile.Initialize();

    Background background(player, txLoader);
    background.Initialize(window);
    



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
        bool respawn = sf::Keyboard::isKeyPressed(sf::Keyboard::R);

        background.GenerateNewSprite();
        float leftBound = camera->CalculateLeftBound();
        std::vector<sf::RectangleShape>& boundRecs = level_tile.GetAllBoundingRecs();
        player->Jump(jumped, deltaTime);
        player->Update(moveRight, moveLeft, leftBound, deltaTime, boundRecs, healthBar);
        player->UpdateView(moveRight, moveLeft);
        // Flag to ensure respawn happens only once per key press
        static bool respawnPressed = false;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::P))
        {
            healthBar->GainLife();
        }

        if(respawn && !respawnPressed)
        {
            player->Respawn();
            respawnPressed = true;
        }
        if(!respawn)
        {
            respawnPressed = false;
        }
        
        level_tile.UpdateLevel(deltaTime, respawn);

        camera->Update(player, moveLeft, respawn);
        healthBar->Update(camera);

        // ----------------- UPDATE----------------

        // ----------------- DRAW -----------------
        window->clear();
        background.Draw(window);
        player->Draw(window);
        level_tile.Draw(window);
        healthBar->Draw(window);
        window->display();
        // ----------------- DRAW -----------------
    }

    return 0;
}