#include "Projectile.h"
#include "Utilities.h"

Projectile::Projectile()
{
    circle.setRadius(radius);
    circle.setFillColor(sf::Color::Red);

}

void Projectile::Update(float dt)
{
    position.x += velocity * dt;

    UpdateView();
}


void Projectile::UpdateView()
{
    circle.setPosition(position);
}
