#pragma once
#include <SFML/Graphics.hpp>
#include "enemies/enemyarrow/projectiles/Arrow.h"

class Player;
class Camera;
class TextureLoader;

class ArrowPool
{

public:
    ArrowPool(const std::shared_ptr<TextureLoader> &txLoader, int size);
    Arrow* GetArrow();
    void Update(const std::shared_ptr<Player>& player,const std::shared_ptr<Camera>& camera, float dt);
    void Draw(const std::shared_ptr<sf::RenderWindow> &window) const;
    void Clear();
    void ResetArrow(std::unique_ptr<Arrow> &arrow);

private:
    std::vector<std::unique_ptr<Arrow>> arrowPool;
    sf::Sprite arrowSprite;
};