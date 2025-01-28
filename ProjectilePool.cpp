#include "ProjectilePool.h"

ProjectilePool::ProjectilePool(int size)
{
    for(int i = 0; i < size; i++)
    {
        projectiles.emplace_back(std::make_unique<Projectile>());
    }
}

Projectile *ProjectilePool::GetProjectile()
{
    for(const auto& projectile: projectiles)
    {
        if(!projectile->isActive)
        {
            if(projectile->recentlyDeactivated)
            {
                continue;
            }

            projectile->isActive = true;
            return projectile.get();
        }
    }

    projectiles.emplace_back(std::make_unique<Projectile>());
    projectiles.back()->isActive = true;
    return projectiles.back().get();
}

void ProjectilePool::Update(float dt)
{
    for(const auto& projectile: projectiles)
    {
        if(projectile->isActive)
        {
            projectile->Update(dt);
        }
    }
}

void ProjectilePool::Draw(const std::shared_ptr<sf::RenderTarget> &rt) const
{
    for(const auto& projectile: projectiles)
    {
        if(projectile->isActive)
        {
            rt->draw(projectile->circle);
        }
    }
}

std::vector<Projectile*> ProjectilePool::GetActiveProjectiles()
{
    std::vector<Projectile*> activeProjectiles;
   for(auto& projectile: projectiles)
   {
        if(projectile->isActive)
        {
            activeProjectiles.push_back(projectile.get());
        }
   }
   return activeProjectiles;
}
