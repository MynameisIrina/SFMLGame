#pragma once
#include <SFML/Graphics.hpp>
#include "Arrow.h"

class ArrowPool
{

public:
    ArrowPool(const std::shared_ptr<TextureLoader> &txLoader, int size);
    Arrow* GetArrow();
    void Update(const std::shared_ptr<Player>& player,const std::shared_ptr<Camera>& camera, float dt);
    void Draw(const std::shared_ptr<sf::RenderWindow> &window) const;
    void Clear();

private:
    std::vector<std::unique_ptr<Arrow>> arrowPool;
    sf::Sprite arrowSprite;
};