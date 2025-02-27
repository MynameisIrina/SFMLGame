#include "Projectile.h"
#include "Utilities.h"
#include <iostream>
#include "Utilities.h"

Projectile::Projectile(const std::shared_ptr<TextureLoader>& txLoader)
{
    sprite = txLoader->SetSprite(TextureLoader::Hairball);
    sprite.setScale(sf::Vector2f(0.02, 0.02));

    boundingCircle = Utilities::CreateBoundingCircle(sprite, sf::Vector2f());

    // circle.setRadius(radius);
    // circle.setFillColor(sf::Color::Red);
}

void Projectile::Update(const std::shared_ptr<Camera> &camera, const float dt)
{
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
