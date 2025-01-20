#include "HealthBar.h"
#include <iostream>
#include "Player.h"

std::shared_ptr<TextureLoader> HealthBar::txLoader = nullptr; 

HealthBar::HealthBar(std::shared_ptr<TextureLoader> txLoaderRef, std::shared_ptr<Camera> cam) : camera(cam)
{
    txLoader = txLoaderRef;
}

void HealthBar::Initialize(const std::shared_ptr<Player> player)
{
    sf::Sprite sprite = txLoader->SetSprite(TextureLoader::TextureType::HealthBar);

    hearts.reserve(player->GetMaxHealth());
    for (int i = 0; i < player->GetMaxHealth(); i++)
    {
        sprite.setScale(scale, scale);
        sf::Vector2f position = sf::Vector2f(camera->GetView().getCenter().x + offsetX, offsetY);
        Heart heart = {sprite, position, true};
        hearts.push_back(heart);
    }

}

void HealthBar::Draw(const std::shared_ptr<sf::RenderTarget> rt) const
{
    for (const auto &heart : hearts)
    {
        rt->draw(heart.sprite);
    }
}

void HealthBar::Update(const std::shared_ptr<Camera> cam, const std::shared_ptr<Player> player)
{
    int currentHealth = player->GetHealth();

    for (int i = 0; i < hearts.size(); i++)
    {
        // update position
        sf::Vector2f position = sf::Vector2f(camera->GetView().getCenter().x + offsetX + (i * 32), offsetY);
        hearts[i].position = position;

        if(i < currentHealth)
        {
            hearts[i].isActive = true;
            hearts[i].sprite.setColor(sf::Color(255,255,255, 255));
        }
        else
        {
            hearts[i].isActive = false;
            hearts[i].sprite.setColor(sf::Color(255,255,255, 0));
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