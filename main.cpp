#include <SFML/Graphics.hpp>
#include <iostream>
#include "TextureLoader.h"
#include "Player.h"
#include "Background.h"
#include "Perlin.h"
#include "Level.h"
#include "Camera.h"
#include "Obstacle.h"
#include "HealthBar.h"
#include "Tile.h"
#include "ProjectilePool.h"
#include "ProjectileBar.h"
#include <cstdlib>

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

    ProjectilePool projectilePool(10);

    std::shared_ptr<Player> player = std::make_shared<Player>(txLoader, projectilePool);

    std::shared_ptr<Camera> camera = std::make_shared<Camera>(window);

    std::shared_ptr<HealthBar> healthBar = std::make_shared<HealthBar>(txLoader);

    std::shared_ptr<ProjectileBar> projectileBar = std::make_shared<ProjectileBar>(); 

    Level level(txLoader);

    Background background(txLoader);

    camera->Initialize();
    player->Initialize(sf::Vector2f(30.f, 8.f * 32.f - 50.f), 6, 5, 2.5f, window->getSize().y);
    healthBar->Initialize(player, camera);
    projectileBar->Initialize(player, camera);
    level.Initialize(8);
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
        bool shoot = sf::Keyboard::isKeyPressed(sf::Keyboard::Enter);

        background.GenerateNewSprite(player);
        float leftBound = camera->CalculateLeftBound();
        std::vector<Tile> tiles = level.GetAllTiles();

        player->Jump(jumped, deltaTime);
        player->Update(moveRight, moveLeft, shoot, leftBound, respawn, deltaTime, tiles);
        player->UpdateView(moveRight, moveLeft);

        level.UpdateLevel(player, camera, deltaTime);

        camera->Update(player);
        healthBar->Update(player, camera);
        projectileBar->Update(player, camera, deltaTime);

        // ----------------- UPDATE----------------

        // ----------------- DRAW -----------------
        window->clear();
        background.Draw(window);
        player->Draw(window);
        level.Draw(window);
        healthBar->Draw(window);
        projectileBar->Draw(window);
        window->display();
        // ----------------- DRAW -----------------
    }

    return 0;
}