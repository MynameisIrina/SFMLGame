#pragma once
#include <SFML/Graphics.hpp>
#include "Projectile.h"
#include "Camera.h"

class ProjectilePool
{

public:

    ProjectilePool(const int size);
    Projectile* GetProjectile();
    void Update(const std::shared_ptr<Camera> &camera, const float dt);
    void Draw(const std::shared_ptr<sf::RenderTarget> &rt) const;
    std::vector<Projectile*> GetActiveProjectiles();

private:
    std::vector<std::unique_ptr<Projectile>> projectiles;
};