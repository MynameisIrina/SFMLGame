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
             arrow->isActive = true;
            return arrow.get();
        }
    }

    // if all arrows are occupied, create new
    arrowPool.emplace_back(std::make_unique<Arrow>(arrowSprite));
    arrowPool.back()->isActive = true;
    return arrowPool.back().get();
}

void ArrowPool::Update(const std::shared_ptr<Player> &player,float dt)
{
    for (const auto &arrow : arrowPool)
    {
        if (arrow->isActive)
            arrow->Update(player, dt);
    }
}

void ArrowPool::Draw(const std::shared_ptr<sf::RenderWindow> &window) const
{
    for (auto &arrow : arrowPool)
    {
        if (arrow->isActive)
        {
            window->draw(arrow->GetSprite());
        }
    }
}
