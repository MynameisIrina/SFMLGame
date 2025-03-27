#include "ui/ProjectileBar.h"
#include "utilities/Utilities.h"

void ProjectileBar::Initialize(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera> &camera)
{
    const sf::Vector2f position = sf::Vector2f(camera->GetView().getCenter().x - offsetX, offsetY);
    const int maxProjectileCount = player->GetProjectilesCount();
    previousProjectileCount = maxProjectileCount;
    amountToReduce = maxAmount / maxProjectileCount;
    currentAmount = maxAmount;

    projectileBar.setSize(sf::Vector2f(maxAmount, barHeight));
    projectileBar.setFillColor(color);
    projectileBar.setPosition(position);

    backgroundBar.setSize(sf::Vector2f(maxAmount, barHeight));
    backgroundBar.setFillColor(sf::Color(255, 0, 0, 0));
    backgroundBar.setOutlineThickness(2);
    backgroundBar.setOutlineColor(sf::Color::Black);
    backgroundBar.setPosition(position);

    // create segments
    segmentsAmount = maxAmount / maxProjectileCount;

    for(int i = 0; i < segmentsAmount; ++i)
    {
        if(i < maxProjectileCount - 1)
        {
            sf::RectangleShape verticalLine(sf::Vector2f(segmentLineWidth, barHeight));
            verticalLine.setFillColor(sf::Color::Black);
            verticalLine.setPosition(position.x + (i + 1) * segmentsAmount, position.y);
            verticalLinesOnBar.push_back(verticalLine);
        }
    }                                                    
    
}

void ProjectileBar::Update(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera> &camera, const float dt)
{
    position = sf::Vector2f(camera->GetView().getCenter().x - offsetX, offsetY);
    const int currentProjectileCount = player->GetProjectilesCount();


    bool playerShot = currentProjectileCount < previousProjectileCount;
    if (playerShot)
    {
        if (currentAmount > 0)
        {
            ReduceAmount(amountToReduce);
            reloadTimer.restart();
        }
    }

    bool projectileRespawned = currentProjectileCount > previousProjectileCount;
    if (projectileRespawned)
    {
        IncrementAmount(amountToIncrease, dt);
    }

    const bool fullyReloaded = currentAmount >= maxAmount;
    if (fullyReloaded)
    {
        isResetting = false;
        projectileBar.setFillColor(color);
        isVisible = true;
    }

    previousProjectileCount = currentProjectileCount;

    UpdateView();
}

void ProjectileBar::ReduceAmount(const float amount)
{
    if (currentAmount <= 0) return;

    currentAmount -= amount;
}

void ProjectileBar::IncrementAmount(const float amount, const float dt)
{
    if(currentAmount >= maxAmount) return;

    currentAmount += amount;
}

void ProjectileBar::ResetBar()
{
    if(currentAmount == maxAmount) return;
    
    currentAmount = maxAmount;
}

void ProjectileBar::UpdateView()
{
    projectileBar.setSize(sf::Vector2f(currentAmount, barHeight));
    projectileBar.setPosition(position);
    backgroundBar.setPosition(position);

    for(int i = 0; i < verticalLinesOnBar.size(); ++i)
    {
        verticalLinesOnBar[i].setPosition(position.x + (i + 1) * segmentsAmount, position.y);
    }     
}

void ProjectileBar::Draw(const std::shared_ptr<sf::RenderTarget> &rt) const
{
    rt->draw(projectileBar);
    rt->draw(backgroundBar);
    
    for (auto &line : verticalLinesOnBar)
    {
        rt->draw(line);
    }
}