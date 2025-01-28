#pragma once
#include <SFML/Graphics.hpp>
#include "Projectile.h"

class ProjectilePool
{

public:

    ProjectilePool(int size);
    Projectile* GetProjectile();
    void Update(float dt);
    void Draw(const std::shared_ptr<sf::RenderTarget> &rt) const;
    std::vector<Projectile*> GetActiveProjectiles();

private:
    std::vector<std::unique_ptr<Projectile>> projectiles;
};