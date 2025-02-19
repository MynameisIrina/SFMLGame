#pragma once
#include <SFML/Graphics.hpp>
#include "TextureLoader.h"
#include "Player.h"
#include "Background.h"
#include "Level.h"
#include "Camera.h"
#include "ObstacleManager.h"
#include "HealthBar.h"
#include "ProjectilePool.h"
#include "ProjectileBar.h"
#include "CoinBar.h"
#include "EnemyManager.h"
#include "CollectibleManager.h"
#include "RespawnManager.h"
#include "GameState.h"
#include "Menu.h"
#include "WinScreen.h"
#include "AudioManager.h"


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
        std::vector<sf::RectangleShape> &obstacles;
    };

public:

    GameManager();
    ~GameManager() = default;

    void Run();
    void ProcessEvents();
    void Update(const float dt);
    void Render();
    PlayerInput HandleInput();
    void HandleAudio(GameManager::PlayerInput input);
    bool ShouldEnterMenuState(GameManager::PlayerInput input);
    bool CheckWinCondition();
    void UpdatePlayer(const PlayerInput& input, float dt, float leftBound, 
                             const BoundingBoxes& boxes);
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


    ProjectilePool projectilePool;
    std::unique_ptr<Background> background;

    GameState state;
    sf::View savedView;
    sf::Sound coinCollectedSound;

    const int windowWidth = 800;
    const int windowHeight = 600;

    // Player settings
    const float playerStartX = 30.f;
    const float playerStartY = 236.f;
    const int playerLives = 6;
    const int playerProjectiles = 5;
    const float playerScale = 2.5f;
    const int firstPlatformheight = 8;
};