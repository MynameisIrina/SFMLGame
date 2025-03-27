#include "ui/HealthBar.h"
#include "player/Player.h"
#include "utilities/Utilities.h"
#include <iostream>

HealthBar::HealthBar(const std::shared_ptr<TextureLoader> &txLoader) : txLoader(txLoader) {}

void HealthBar::Initialize(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera> &camera)
{
    sf::Sprite sprite = txLoader->SetSprite(TextureLoader::TextureType::HealthBar);
    sprite.setOrigin(sprite.getLocalBounds().width * 0.5f, sprite.getLocalBounds().height * 0.5f);
    currentHealth = player->GetHealth();

    hearts.reserve(player->GetMaxHealth());
    for (int i = 0; i < player->GetMaxHealth(); i++)
    {
        sprite.setScale(scale, scale);
        const sf::Vector2f position = sf::Vector2f(camera->GetView().getCenter().x + offsetX, offsetY);
        Heart heart = {sprite, position, true, false};
        hearts.push_back(heart);
    }
}

void HealthBar::Draw(const std::shared_ptr<sf::RenderTarget> &rt) const
{
    for (const auto &heart : hearts)
    {
        rt->draw(heart.sprite);
    }
}

void HealthBar::Update(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera> &camera)
{
    const int previousHealth = currentHealth;
    currentHealth = player->GetHealth();

    bool healthIncreased = previousHealth < currentHealth && !player->IsInRebirth();
    if (healthIncreased)
    {
        hearts[currentHealth - 1].shouldPulse = true;
        animationTimer.restart();
        animationActive = true;
    }

    // update visibility of hearts
    for (int i = 0; i < hearts.size(); i++)
    {
        const sf::Vector2f position = sf::Vector2f(camera->GetView().getCenter().x + offsetX + (i * heartSpriteSize), offsetY);
        hearts[i].position = position;

        if (i < currentHealth)
        {
            hearts[i].isActive = true;
            hearts[i].sprite.setColor(Utilities::GetOpaqueColor(sf::Color(255, 255, 255)));
        }
        else
        {
            hearts[i].isActive = false;
            hearts[i].sprite.setColor(Utilities::GetTransparentColor(sf::Color(255, 255, 255)));
        }
    }

    HandlePulseAnimation(player);
    UpdateView();
}

void HealthBar::UpdateView()
{
    for (auto &heart : hearts)
    {
        sf::Vector2f position = heart.position;
        heart.sprite.setPosition(position);
    }
}

void HealthBar::HandlePulseAnimation(const std::shared_ptr<Player> &player)
{
    if (!animationActive)
        return;

    if (animationTimer.getElapsedTime().asSeconds() >= pulseDuration)
    {
        animationActive = false;
        lastPulseTime = 0.f;
        for (int i = 0; i < hearts.size(); i++)
        {
            if (hearts[i].shouldPulse)
            {
                hearts[i].shouldPulse = false;
                hearts[i].sprite.setScale(scale, scale);
            }
        }
        return;
    }

    const bool timeHasPassed = animationTimer.getElapsedTime().asSeconds() - lastPulseTime >= pulseInterval;

    if (timeHasPassed)
    {
        for (int i = 0; i < hearts.size(); i++)
        {
            if (hearts[i].shouldPulse)
            {
                if (pulsing)
                {
                    hearts[i].sprite.setScale(scale, scale);
                }
                else
                {
                    hearts[i].sprite.setScale(scale * 1.1f, scale * 1.1f);
                }
            }
        }

        pulsing = !pulsing;
        lastPulseTime = animationTimer.getElapsedTime().asSeconds();
    }
}
