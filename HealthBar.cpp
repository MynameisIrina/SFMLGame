#include "HealthBar.h"
#include <iostream>

HealthBar::HealthBar(std::shared_ptr<TextureLoader> txLoaderRef, std::shared_ptr<Camera> cam) : txLoader(txLoaderRef), camera(cam)
{
}

void HealthBar::Initialize(int amount)
{
    sf::Sprite sprite = txLoader->SetSprite(TextureLoader::TextureType::HealthBar);
    for (int i = 0; i < amount; i++)
    {
        sprite.setScale(scale, scale);
        sf::Vector2f position = sf::Vector2f(camera->GetView().getCenter().x + offsetX, offsetY);
        Heart heart = {sprite, position, true};
        hearts.push_back(heart);
    }

    std::cout << "Initialized hearts with " << hearts.size() << " hearts." << std::endl;
}

void HealthBar::Draw(const std::shared_ptr<sf::RenderTarget> rt) const
{
    for (const auto &heart : hearts)
    {
        rt->draw(heart.sprite);
    }
}

void HealthBar::Update(const std::shared_ptr<Camera> cam)
{
    for (int i = 0; i < hearts.size(); i++)
    {
        sf::Vector2f position = sf::Vector2f(camera->GetView().getCenter().x + offsetX + (i * 32), offsetY);
        hearts[i].position = position;
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

void HealthBar::LoseLife()
{
    for(int i = hearts.size() - 1; i >= 0; i--)
    {
        if(hearts[i].isActive)
        {
            hearts[i].isActive = false;
            hearts[i].sprite.setColor(sf::Color(255,255,255, 0));
            break;
        }
    }

}

void HealthBar::GainLife()
{
    for(int i = 0; i < hearts.size(); i++)
    {
        if(!hearts[i].isActive)
        {
            hearts[i].isActive = true;
            hearts[i].sprite.setColor(sf::Color(255,255,255, 255));
            break;
        }
    }

}
