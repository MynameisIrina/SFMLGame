#include <SFML/Graphics.hpp>
#include <iostream>

// #include <SFML/Graphics.hpp>
// #include "TextureLoader.h"
// #include "Player.h"
// #include "Background.h"
// #include "Level.h"
// #include "Camera.h"
// #include "ObstacleManager.h"
// #include "HealthBar.h"
// #include "ProjectilePool.h"
// #include "ProjectileBar.h"
// #include "CoinBar.h"
// #include "EnemyManager.h"
// #include "CollectibleManager.h"
// #include "RespawnManager.h"

#include "GameManager.h"

int main()
{
    GameManager gameManager;
    gameManager.Run();
    // // ----------------- INITIALIZE -----------------
    // sf::ContextSettings settings;
    // settings.antialiasingLevel = 8;
    // std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 600), "SFML game", sf::Style::Default, settings);
    // std::unique_ptr<sf::Uint8[]> pixels = std::make_unique<sf::Uint8[]>(800 * 600 * 4);
    // std::srand(static_cast<unsigned>(std::time(nullptr)));
    // sf::Clock timer;
    // float deltaTime = 0.0f;

    // std::shared_ptr<TextureLoader> txLoader = std::make_shared<TextureLoader>();
    // txLoader->Initialize();

    // ProjectilePool projectilePool(10);

    // std::shared_ptr<Player> player = std::make_shared<Player>(txLoader, projectilePool);

    // std::shared_ptr<Camera> camera = std::make_shared<Camera>(window);

    // std::shared_ptr<HealthBar> healthBar = std::make_shared<HealthBar>(txLoader);

    // std::shared_ptr<ProjectileBar> projectileBar = std::make_shared<ProjectileBar>(); 

    // std::shared_ptr<CoinBar> coinBar = std::make_shared<CoinBar>(txLoader);

    // Background background(txLoader);

    // std::shared_ptr<CollectibleManager> collectibleManager = std::make_shared<CollectibleManager>(txLoader);
    // std::shared_ptr<EnemyManager> enemyManager = std::make_shared<EnemyManager>(txLoader);
    // std::shared_ptr<ObstacleManager> obstacleManager = std::make_shared<ObstacleManager>(txLoader);

    // std::shared_ptr<RespawnManager> respawnManager = std::make_shared<RespawnManager>(player, enemyManager, collectibleManager);

    // Level level(obstacleManager, enemyManager, collectibleManager, txLoader);

    // camera->Initialize();
    // player->Initialize(sf::Vector2f(30.f, 8.f * 32.f - 20.f), 6, 5, 2.5f, window->getSize().y);
    // healthBar->Initialize(player, camera);
    // projectileBar->Initialize(player, camera);
    // coinBar->Initialize(player, camera);
    // level.Initialize(8);
    // background.Initialize(window);

    // // ----------------- INITIALIZE -----------------

    // while (window->isOpen())
    // {
    //     //  ----------------- UPDATE -----------------
    //     sf::Event event;
    //     while (window->pollEvent(event))
    //     {
    //         if (event.type == sf::Event::Closed)
    //             window->close();
    //     }

    //     deltaTime = timer.restart().asSeconds();

    //     bool moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    //     bool moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    //     bool jumped = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    //     bool respawn = sf::Keyboard::isKeyPressed(sf::Keyboard::R);
    //     bool shoot = sf::Keyboard::isKeyPressed(sf::Keyboard::Enter);
    //     bool exchangeCoins = sf::Keyboard::isKeyPressed(sf::Keyboard::P);

    //     background.GenerateNewSprite(player);
    //     float leftBound = camera->CalculateLeftBound();
    //     std::vector<sf::RectangleShape> tilesBoundBoxes = level.GetAllTiles();
    //     std::vector<sf::RectangleShape> enemiesBoundBoxes = enemyManager->GetEnemiesBoundingBoxes();
    //     std::vector<sf::RectangleShape> obstaclesBoundBoxes = obstacleManager->GetObstaclesBoundingBoxes();

    //     player->Jump(jumped, deltaTime);
    //     player->Update(respawnManager, camera, moveRight, moveLeft, shoot, leftBound, respawn, exchangeCoins, deltaTime, tilesBoundBoxes, enemiesBoundBoxes, obstaclesBoundBoxes);

    //     level.UpdateLevel(player, camera, deltaTime);

    //     camera->Update(player);
    //     healthBar->Update(player, camera);
    //     coinBar->Update(player, camera);
    //     projectileBar->Update(player, camera, deltaTime);

    //     // ----------------- UPDATE----------------

    //     // ----------------- DRAW -----------------
    //     window->clear();
    //     background.Draw(window);
    //     level.Draw(window);
    //     player->Draw(window);
    //     healthBar->Draw(window);
    //     coinBar->Draw(window);
    //     projectileBar->Draw(window);
    //     window->display();
    //     // ----------------- DRAW -----------------
    // }

    return 0;
}