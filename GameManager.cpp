#include "GameManager.h"

void GameManager::InitializeWindow()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    window = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 600), "sf::Cat", sf::Style::Close, settings);
}

void GameManager::InitializeAudio()
{
    audioManager = std::make_shared<AudioManager>();
    audioManager->LoadSound("coinCollected", "Assets/Audio/sounds/coin.wav");
    audioManager->LoadSound("jump", "Assets/Audio/sounds/jump.wav");
    audioManager->LoadSound("kill enemy", "Assets/Audio/sounds/explosion.wav");
}

void GameManager::InitializeGame()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    sf::Clock timer;
    deltaTime = 0.0f;
    state = GameState::MENU;

    txLoader = std::make_shared<TextureLoader>();
    txLoader->Initialize();

    InitializeAudio();
    audioManager->PlayMusic("Assets/Audio/sounds/time_for_adventure.mp3", 100);

    projectilePool = std::make_unique<ProjectilePool>(txLoader, 10);

    player = std::make_shared<Player>(txLoader, std::move(projectilePool), audioManager);

    camera = std::make_shared<Camera>(window);

    healthBar = std::make_shared<HealthBar>(txLoader);

    projectileBar = std::make_shared<ProjectileBar>();

    coinBar = std::make_shared<CoinBar>(txLoader);

    background = std::make_unique<Background>(txLoader);

    collectibleManager = std::make_shared<CollectibleManager>(txLoader);
    enemyManager = std::make_shared<EnemyManager>(txLoader, audioManager);
    obstacleManager = std::make_shared<ObstacleManager>(txLoader);

    respawnManager = std::make_shared<RespawnManager>(player, enemyManager, collectibleManager, camera, projectileBar);

    level = std::make_shared<Level>(obstacleManager, enemyManager, collectibleManager, txLoader);

    menu = std::make_unique<Menu>(window, txLoader);
    winScreen = std::make_unique<WinScreen>(window, txLoader);

    camera->Initialize();
    player->Initialize(sf::Vector2f(playerStartX, playerStartY), playerLives, playerProjectiles, playerScale, window->getSize().y);
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
            savedView = window->getView();
            window->setView(window->getDefaultView());

            menu->Show(state, deltaTime);
        }
        else if (state == GameState::RUNNING)
        {
            window->setView(savedView); // restore camera position

            ProcessEvents();
            Update(deltaTime);
        }
        else if (state == GameState::EXIT)
        {
            window->close();
        }
        else if (state == GameState::WIN)
        {
            window->setView(window->getDefaultView());
            winScreen->Show(state, deltaTime);
        }
        else if (state == GameState::RESTART)
        {
            RestartGame();
            state = GameState::RUNNING;
        }

        Render();
    }
}
void GameManager::RestartGame()
{
    InitializeGame();
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

    if (ShouldEnterMenuState(input))
        return;

    auto boundingBoxes = CollectBoundingBoxes();

    background->GenerateNewSprite(player);

    level->UpdateLevel(player, camera, deltaTime);

    player->Jump(input.jumped, deltaTime);
    player->Update(respawnManager, camera, input.moveRight, input.moveLeft, input.shoot, input.respawn, input.exchangeCoins,
                   deltaTime, boundingBoxes.tiles, boundingBoxes.enemies, boundingBoxes.flyingeEnemies, boundingBoxes.obstacles);

    if (CheckWinCondition())
        return;

    camera->Update(player, deltaTime);
    healthBar->Update(player, camera);
    coinBar->Update(player, camera);
    projectileBar->Update(player, camera, deltaTime);
}

GameManager::BoundingBoxes GameManager::CollectBoundingBoxes()
{
    return {
        level->GetAllTiles(),
        enemyManager->GetEnemiesBoundingBoxes(),
        enemyManager->GetFlyingEnemiesBoundingBoxes(),
        obstacleManager->GetObstaclesBoundingBoxes()};
}

void GameManager::Render()
{
    if (state == GameState::MENU)
    {
        menu->Draw();
    }
    else if (state == GameState::WIN)
    {
        winScreen->Draw();
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

bool GameManager::ShouldEnterMenuState(GameManager::PlayerInput input)
{
    if (input.paused)
    {
        state = GameState::MENU;
        return true;
    }

    return false;
}

bool GameManager::CheckWinCondition()
{
    if (player->GetCoins() == coinWinCondition)
    {
        state = GameState::WIN;
        return true;
    }

    return false;
}
