#include "ProjectilePool.h"

ProjectilePool::ProjectilePool(const std::shared_ptr<TextureLoader> &txLoader, const int size) : txLoader(txLoader)
{
    for (int i = 0; i < size; i++)
    {
        projectiles.emplace_back(std::make_unique<Projectile>(txLoader));
    }
}

Projectile *ProjectilePool::GetProjectile()
{
    for (const auto &projectile : projectiles)
    {
        if (!projectile->isActive)
        {
            if (projectile->IsRecentlyDeactivated())
            {
                continue;
            }

            projectile->isActive = true;
            return projectile.get();
        }
    }

    projectiles.emplace_back(std::make_unique<Projectile>(txLoader));
    projectiles.back()->isActive = true;
    return projectiles.back().get();
}

void ProjectilePool::Update(const std::shared_ptr<Camera> &camera, const float dt)
{
    for (const auto &projectile : projectiles)
    {
        projectile->Update(camera, dt);
    }
}

void ProjectilePool::Draw(const std::shared_ptr<sf::RenderTarget> &rt) const
{
    for (const auto &projectile : projectiles)
    {
        if (projectile->isActive)
        {
            rt->draw(projectile->GetSprite());
        }
    }
}

std::vector<Projectile *> ProjectilePool::GetActiveProjectiles()
{
    std::vector<Projectile *> activeProjectiles;
    for (auto &projectile : projectiles)
    {
        if (projectile->isActive)
        {
            activeProjectiles.push_back(projectile.get());
        }
    }
    return activeProjectiles;
}

void ProjectilePool::Clear()
{
    projectiles.clear();
}
