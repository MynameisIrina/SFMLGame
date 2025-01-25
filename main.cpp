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
#include "Tile.h"

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

    std::shared_ptr<Camera> camera = std::make_shared<Camera>(window);

    std::shared_ptr<HealthBar> healthBar = std::make_shared<HealthBar>(txLoader);

    Level_TileBased level_tile(txLoader);

    Background background(txLoader);


    camera->Initialize();
    player->Initialize(sf::Vector2f(35.f, 20.f), 6, 2.5f);
    healthBar->Initialize(player, camera);
    level_tile.Initialize();
    background.Initialize(window);
    

    sf::Sprite sprite = txLoader->SetSprite(TextureLoader::Arrow);
    sprite.setPosition(10,10);

    Arrow arrow(sprite);
    sf::RectangleShape bb = arrow.CreateBoundingBox();




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

        background.GenerateNewSprite(player);
        float leftBound = camera->CalculateLeftBound();
        std::vector<Tile> tiles = level_tile.GetAllTiles();
        player->Jump(jumped, deltaTime);
        player->Update(moveRight, moveLeft, leftBound, respawn, deltaTime, tiles);
        player->UpdateView(moveRight, moveLeft);
        // Flag to ensure respawn happens only once per key press
        static bool respawnPressed = false;
        
        level_tile.UpdateLevel(player, camera, deltaTime);

        camera->Update(player);
        healthBar->Update(player, camera);

        // ----------------- UPDATE----------------

        // ----------------- DRAW -----------------
        window->clear();
        background.Draw(window);
        window->draw(sprite);
        window->draw(bb);
        player->Draw(window);
        level_tile.Draw(window);
        healthBar->Draw(window);
        window->display();
        // ----------------- DRAW -----------------
    }

    return 0;
}