#pragma once
#include <SFML/Graphics.hpp>
#include "graphics/TextureLoader.h"
#include "player/Player.h"
#include "graphics/Background.h"
#include "level/Level.h"
#include "camera/Camera.h"
#include "managers/ObstacleManager.h"
#include "ui/HealthBar.h"
#include "player/projectiles/ProjectilePool.h"
#include "ui/ProjectileBar.h"
#include "ui/CoinBar.h"
#include "managers/EnemyManager.h"
#include "managers/CollectibleManager.h"
#include "managers/RespawnManager.h"
#include "GameState.h"
#include "ui/menu/Menu.h"
#include "ui/menu/WinScreen.h"
#include "managers/AudioManager.h"


class GameManager
{

    struct PlayerInput
    {
        bool moveRight;
        bool moveLeft;
        bool jumped;
        bool respawn;
        bool shoot;
        bool exchangeCoins;
        bool paused;
    };

    struct BoundingBoxes
    {
        std::vector<sf::RectangleShape> &tiles;
        std::vector<sf::RectangleShape> &enemies;
        std::vector<sf::RectangleShape> &flyingeEnemies;
        std::vector<sf::RectangleShape> &obstacles;
    };

public:

    GameManager() = default;
    ~GameManager() = default;

    void InitializeGame();
    void RestartGame();
    void Run();
    void ProcessEvents();
    void Update(const float dt);
    void Render();
    PlayerInput HandleInput();
    void HandleAudio(GameManager::PlayerInput input);
    bool ShouldEnterMenuState(GameManager::PlayerInput input);
    bool CheckWinCondition();
    void InitializeWindow();
    void InitializeAudio();
    void Restart();
    GameManager::BoundingBoxes CollectBoundingBoxes();

private:
    sf::ContextSettings settings;
    std::shared_ptr<sf::RenderWindow> window;
    sf::Clock timer;
    float deltaTime;

    std::shared_ptr<TextureLoader> txLoader;
    std::shared_ptr<AudioManager> audioManager;
    std::shared_ptr<Player> player;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<HealthBar> healthBar;
    std::shared_ptr<ProjectileBar> projectileBar;
    std::shared_ptr<CoinBar> coinBar;
    std::shared_ptr<CollectibleManager> collectibleManager;
    std::shared_ptr<EnemyManager> enemyManager;
    std::shared_ptr<ObstacleManager> obstacleManager;
    std::shared_ptr<RespawnManager> respawnManager;
    std::shared_ptr<Level> level;
    std::unique_ptr<Menu> menu;
    std::unique_ptr<WinScreen> winScreen;



    std::unique_ptr<ProjectilePool> projectilePool;
    std::unique_ptr<Background> background;

    GameState state;
    sf::View savedView;
    sf::Sound coinCollectedSound;

    const int windowWidth = 800;
    const int windowHeight = 600;

    // Player settings
    const float playerStartX = 30.f;
    const float playerStartY = 210.f;
    const int playerLives = 6;
    const int playerProjectiles = 5;
    const float playerScale = 2.5f;
    const int firstPlatformheight = 8;
    const int coinWinCondition = 8;
};