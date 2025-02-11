#include "ProjectileBar.h"
#include "Utilities.h"

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

    int currentProjectileCount = player->GetProjectilesCount();

    if (currentProjectileCount < previousProjectileCount)
    {
        if (projectileBar.getSize().x > 0)
        {
            ReduceAmount(amountToReduce);
        }
    }

    // Reset bar if the player respawned
    if(currentProjectileCount > previousProjectileCount)
    {
        ResetBar();
    }

    //std::cout << currentProjectileCount << std::endl;

    // Regenerate projectile bar
    if (currentProjectileCount == 0)
    {
        AddVisualEffects(dt);
        IncrementAmount(amountToIncrease, dt);
    }
    else
    {
        projectileBar.setFillColor(color);
        isVisible = true;
    }

    UpdateView();

    previousProjectileCount = currentProjectileCount;
}

void ProjectileBar::UpdateView()
{
    projectileBar.setPosition(position);
    backgroundBar.setPosition(position);
}

void ProjectileBar::ReduceAmount(const float amount)
{
    const float newAmount = projectileBar.getSize().x - amount;
    projectileBar.setSize(sf::Vector2f(newAmount, projectileBar.getSize().y));
}

void ProjectileBar::IncrementAmount(const float amount, const float dt)
{
    const float delta = amount * dt;
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

void ProjectileBar::ResetBar()
{
    projectileBar.setSize(sf::Vector2f(maxAmount, 10));
}