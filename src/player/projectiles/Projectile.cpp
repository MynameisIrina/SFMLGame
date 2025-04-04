#include "player/projectiles/Projectile.h"
#include "utilities/Utilities.h"

Projectile::Projectile(const std::shared_ptr<TextureLoader>& txLoader)
{
    sprite = txLoader->SetSprite(TextureLoader::Hairball);
    sprite.setScale(sf::Vector2f(scale, scale));

    boundingCircle = Utilities::CreateBoundingCircle(sprite, sf::Vector2f());
}

void Projectile::Update(const std::shared_ptr<Camera> &camera, const float dt)
{
    if(!IsProjectileActive()) return;

    const bool outOfCameraBounds = velocity < 0 ? position.x < camera->CalculateLeftBound() : position.x > camera->CalculateRightBound();

    if(outOfCameraBounds)
    {
        isActive = false;
    }

    position.x += velocity * dt;

    UpdateView();
}


void Projectile::UpdateView()
{
    sprite.setPosition(position);
    boundingCircle.setPosition(position);
}

sf::Sprite Projectile::GetSprite() const
{
    return sprite;
}

bool Projectile::IsRecentlyDeactivated() const
{
    return recentlyDeactivated;
}

sf::CircleShape Projectile::GetBoundingCircle() const
{
    return boundingCircle;
}

bool Projectile::IsProjectileActive() const
{
    if(isActive) return true;

    return false;
}
