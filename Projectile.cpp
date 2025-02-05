#include "Projectile.h"
#include "Utilities.h"
#include <iostream>

Projectile::Projectile()
{
    circle.setRadius(radius);
    circle.setFillColor(sf::Color::Red);
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
    circle.setPosition(position);
}
