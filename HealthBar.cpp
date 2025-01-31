#include "HealthBar.h"
#include <iostream>
#include "Player.h"
#include "Utilities.h"


HealthBar::HealthBar(const std::shared_ptr<TextureLoader>& txLoader): txLoader(txLoader) {}

void HealthBar::Initialize(const std::shared_ptr<Player>& player, const std::shared_ptr<Camera>& camera)
{
    sf::Sprite sprite = txLoader->SetSprite(TextureLoader::TextureType::HealthBar);

    hearts.reserve(player->GetMaxHealth());
    for (int i = 0; i < player->GetMaxHealth(); i++)
    {
        sprite.setScale(scale, scale);
        const sf::Vector2f position = sf::Vector2f(camera->GetView().getCenter().x + offsetX, offsetY);
        Heart heart = {sprite, position, true};
        hearts.push_back(heart);
    }

}

void HealthBar::Draw(const std::shared_ptr<sf::RenderTarget>& rt) const
{
    for (const auto &heart : hearts)
    {
        rt->draw(heart.sprite);
    }
}

void HealthBar::Update(const std::shared_ptr<Player>& player, const std::shared_ptr<Camera>& camera)
{
    const int currentHealth = player->GetHealth();

    for (int i = 0; i < hearts.size(); i++)
    {
        const sf::Vector2f position = sf::Vector2f(camera->GetView().getCenter().x + offsetX + (i * TILE_SIZE), offsetY);
        hearts[i].position = position;

        if(i < currentHealth)
        {
            hearts[i].isActive = true;
            hearts[i].sprite.setColor(Utilities::GetOpaqueColor(sf::Color(255,255,255)));
        }
        else
        {
            hearts[i].isActive = false;
            hearts[i].sprite.setColor(Utilities::GetTransparentColor(sf::Color(255,255,255)));
        }

    }

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