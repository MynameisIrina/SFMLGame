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
    for (auto &arrow : arrowPool)
    {
        if (!arrow->isActive)
        {
            if(arrow->recentlyDeactivated)
            {
                continue;
            }
            
            arrow->isActive = true;
            return arrow.get();
        }
    }

    // if all arrows are occupied, create new
    arrowPool.emplace_back(std::make_unique<Arrow>(arrowSprite));
    arrowPool.back()->isActive = true;
    return arrowPool.back().get();
}

void ArrowPool::Update(const std::shared_ptr<Player> &player,const std::shared_ptr<Camera> camera,float dt)
{
    for (const auto &arrow : arrowPool)
    {
        if (arrow->isActive)
            arrow->Update(player,camera, dt);
    }
}

void ArrowPool::Draw(const std::shared_ptr<sf::RenderWindow> &window) const
{
    for (auto &arrow : arrowPool)
    {
        if (arrow->isActive)
        {
            window->draw(arrow->GetSprite());
            window->draw(arrow->GetBoundingBox());
        }
    }
}
