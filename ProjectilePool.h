#pragma once
#include <SFML/Graphics.hpp>
#include "Projectile.h"
#include "Camera.h"
#include "TextureLoader.h"

class ProjectilePool
{

public:

    ProjectilePool(const std::shared_ptr<TextureLoader>& txLoader, const int size);
    Projectile* GetProjectile();
    void Update(const std::shared_ptr<Camera> &camera, const float dt);
    void Draw(const std::shared_ptr<sf::RenderTarget> &rt) const;
    std::vector<Projectile*> GetActiveProjectiles();
    void Clear();

private:
    std::vector<std::unique_ptr<Projectile>> projectiles;
    std::shared_ptr<TextureLoader> txLoader;
};