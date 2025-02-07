#include "GameManager.h"

GameManager::GameManager() : projectilePool(10), state(GameState::MENU)
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    window = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 600), "SFML game", sf::Style::Default, settings);

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    sf::Clock timer;
    float deltaTime = 0.0f;

    txLoader = std::make_shared<TextureLoader>();
    txLoader->Initialize();

    player = std::make_shared<Player>(txLoader, projectilePool);

    camera = std::make_shared<Camera>(window);

    healthBar = std::make_shared<HealthBar>(txLoader);

    projectileBar = std::make_shared<ProjectileBar>();

    coinBar = std::make_shared<CoinBar>(txLoader);

    background = std::make_unique<Background>(txLoader);

    collectibleManager = std::make_shared<CollectibleManager>(txLoader);
    enemyManager = std::make_shared<EnemyManager>(txLoader);
    obstacleManager = std::make_shared<ObstacleManager>(txLoader);

    respawnManager = std::make_shared<RespawnManager>(player, enemyManager, collectibleManager);

    level = std::make_shared<Level>(obstacleManager, enemyManager, collectibleManager, txLoader);

    menu = std::make_unique<Menu>(window, txLoader);

    camera->Initialize();
    player->Initialize(sf::Vector2f(playerStartX, playerStartY), playerLives, playerProjectiles, playerSpeed, window->getSize().y);
    healthBar->Initialize(player, camera);
    projectileBar->Initialize(player, camera);
    coinBar->Initialize(player, camera);
    level->Initialize(firstPlatformheight);
    background->Initialize(window);
}

void GameManager::Run()
{
    while (window->isOpen())
    {
        deltaTime = timer.restart().asSeconds();

        if (state == GameState::MENU)
        {
            menu->Show(state, deltaTime);
        }
        else if (state == GameState::RUNNING)
        {
            ProcessEvents();
            Update(deltaTime);
        }
        else if (state == GameState::EXIT)
        {
            window->close();
        }

        Render();
    }
}

void GameManager::ProcessEvents()
{
    sf::Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window->close();
    }
}

void GameManager::Update(const float dt)
{

    const PlayerInput &input = HandleInput();

    if (input.paused)
    {
        state = GameState::MENU;
        return;
    }

    const float leftBound = camera->CalculateLeftBound();

    std::vector<sf::RectangleShape> &tilesBoundBoxes = level->GetAllTiles();
    std::vector<sf::RectangleShape> &enemiesBoundBoxes = enemyManager->GetEnemiesBoundingBoxes();
    std::vector<sf::RectangleShape> &obstaclesBoundBoxes = obstacleManager->GetObstaclesBoundingBoxes();

    background->GenerateNewSprite(player);

    player->Jump(input.jumped, deltaTime);
    player->Update(respawnManager, camera, input.moveRight, input.moveLeft, input.shoot, leftBound, input.respawn, input.exchangeCoins, deltaTime, tilesBoundBoxes, enemiesBoundBoxes, obstaclesBoundBoxes);

    level->UpdateLevel(player, camera, deltaTime);

    camera->Update(player);
    healthBar->Update(player, camera);
    coinBar->Update(player, camera);
    projectileBar->Update(player, camera, deltaTime);
}

void GameManager::Render()
{
    if (state == GameState::MENU)
    {
        menu->Draw();
    }

    else if (state == GameState::RUNNING)
    {

        window->clear();

        background->Draw(window);


        level->Draw(window);
        player->Draw(window);


        healthBar->Draw(window);
        coinBar->Draw(window);
        projectileBar->Draw(window);

    }

     window->display();
}

GameManager::PlayerInput GameManager::HandleInput()
{
    return {sf::Keyboard::isKeyPressed(sf::Keyboard::D),
            sf::Keyboard::isKeyPressed(sf::Keyboard::A),
            sf::Keyboard::isKeyPressed(sf::Keyboard::Space),
            sf::Keyboard::isKeyPressed(sf::Keyboard::R),
            sf::Keyboard::isKeyPressed(sf::Keyboard::Enter),
            sf::Keyboard::isKeyPressed(sf::Keyboard::P),
            sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)};
}
