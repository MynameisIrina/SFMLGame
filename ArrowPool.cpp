// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "ArrowPool.h"

ArrowPool::ArrowPool(const std::shared_ptr<TextureLoader> &txLoader, int size)
{
    arrowSprite = txLoader->SetSprite(TextureLoader::Arrow);

    for (int i = 0; i < size; i++)
    {
        arrowPool.emplace_back(std::make_unique<Arrow>(arrowSprite));
    }
}

Arrow *ArrowPool::GetArrow()
{
    for (const auto &arrow : arrowPool)
    {
        if (!arrow->isActive && !arrow->recentlyDeactivated)
        {
            arrow->isActive = true;
            return arrow.get();
        }
    }

    // if all arrows are occupied, create new
    arrowPool.emplace_back(std::make_unique<Arrow>(arrowSprite));
    arrowPool.back()->isActive = true;
    return arrowPool.back().get();
}

void ArrowPool::Update(const std::shared_ptr<Player> &player,const std::shared_ptr<Camera>& camera, const float dt)
{

    for (const auto &arrow : arrowPool)
    {
        if (arrow->isActive)
            arrow->Update(player,camera, dt);
    }
}

void ArrowPool::Draw(const std::shared_ptr<sf::RenderWindow> &window) const
{
    for (const auto &arrow : arrowPool)
    {
        if (arrow->isActive)
        {
            window->draw(arrow->sprite);
            window->draw(arrow->GetBoundingBox());
        }
    }
}

void ArrowPool::Clear()
{
    arrowPool.clear();
}
