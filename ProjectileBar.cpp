#include "ProjectileBar.h"
#include "Utilities.h"

ProjectileBar::ProjectileBar() {}

void ProjectileBar::Initialize(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera> &camera)
{

    const sf::Vector2f position = sf::Vector2f(camera->GetView().getCenter().x - offsetX, offsetY);
    previousProjectileCount = player->GetProjectilesCount();
    amountToReduce = maxAmount / player->GetProjectilesCount();

    projectileBar.setSize(sf::Vector2f(maxAmount, 10));
    projectileBar.setFillColor(sf::Color(150, 0, 0));
    projectileBar.setPosition(position);

    backgroundBar.setSize(sf::Vector2f(maxAmount, 10));
    backgroundBar.setFillColor(sf::Color(255, 0, 0, 0));
    backgroundBar.setOutlineThickness(2);
    backgroundBar.setOutlineColor(sf::Color::Black);
    backgroundBar.setPosition(position);
}

void ProjectileBar::Draw(const std::shared_ptr<sf::RenderTarget> &rt) const
{
    rt->draw(projectileBar);
    rt->draw(backgroundBar);
}

void ProjectileBar::Update(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera> &camera, const float dt)
{
    position = sf::Vector2f(camera->GetView().getCenter().x - offsetX, offsetY);

    // Reduce amount when player shoots
    bool playerShot = player->GetProjectilesCount() < previousProjectileCount;
    if (playerShot)
    {
        if (projectileBar.getSize().x > 0)
        {
            ReduceAmount(amountToReduce);
        }

        previousProjectileCount = player->GetProjectilesCount();
    }

    // Renew projectile bar over time
    if (player->GetProjectilesCount() == 0)
    {
        AddVisualEffects(dt);
        IncrementAmount(amountToIncrease, dt);
        previousProjectileCount = 0;

        bool readyToShoot = projectileBar.getSize().x >= maxAmount;
        if (readyToShoot)
        {
            player->ResetProjectilesCount();
            previousProjectileCount = player->GetProjectilesCount();
        }
    }
    else
    {
        projectileBar.setFillColor(color);
        isVisible = true;
    }

    UpdateView();
}

void ProjectileBar::UpdateView()
{
    projectileBar.setPosition(position);
    backgroundBar.setPosition(position);
}

void ProjectileBar::ReduceAmount(const float amountToReduce)
{
    const float newAmount = projectileBar.getSize().x - amountToReduce;
    projectileBar.setSize(sf::Vector2f(newAmount, projectileBar.getSize().y));
}

void ProjectileBar::IncrementAmount(const float amountToIncrease, const float dt)
{
    const float delta = amountToIncrease * dt;
    const float newAmount = projectileBar.getSize().x + delta;
    projectileBar.setSize(sf::Vector2f(newAmount, 10));
}

void ProjectileBar::AddVisualEffects(const float dt)
{
    if (blinkingTimer.getElapsedTime().asSeconds() >= blinkingInterval)
    {
        projectileBar.setFillColor(isVisible ? Utilities::GetTransparentColor(color) : Utilities::GetOpaqueColor(color));
        isVisible = !isVisible;
        blinkingTimer.restart();
    }
}
