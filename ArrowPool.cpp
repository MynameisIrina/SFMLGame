#include "ArrowPool.h"

ArrowPool::ArrowPool(const std::shared_ptr<TextureLoader> &txLoader, int size)
{
    arrowSprite = txLoader->SetSprite(TextureLoader::Arrow);

    for (int i = 0; i < size; ++i)
    {
        arrowPool.emplace_back(std::make_unique<Arrow>(arrowSprite));
    }
}

Arrow *ArrowPool::GetArrow()
{
    for (auto &arrow : arrowPool)
    {
        if (!arrow->isActive && !arrow->recentlyDeactivated)
        {
            ResetArrow(arrow);
            arrow->isActive = true;
            return arrow.get();
        }
    }

    arrowPool.emplace_back(std::make_unique<Arrow>(arrowSprite));
    arrowPool.back()->isActive = true;
    return arrowPool.back().get();
}

void ArrowPool::Update(const std::shared_ptr<Player> &player,const std::shared_ptr<Camera>& camera, const float dt)
{
    
    for (const auto &arrow : arrowPool)
    {
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

void ArrowPool::ResetArrow(std::unique_ptr<Arrow> &arrow)
{
    arrow->isActive = false;
    arrow->recentlyDeactivated = false;
    arrow->sprite.setPosition(0, 0);
    arrow->direction = 0;
    arrow->velocity = 0;
}
