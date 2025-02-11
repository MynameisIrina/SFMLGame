
#include "CoinBar.h"

CoinBar::CoinBar(const std::shared_ptr<TextureLoader> &txLoader)
{
    this->sprite = txLoader->SetSprite(TextureLoader::Coin);
    coins = 0;
}

void CoinBar::Initialize(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera> &camera)
{
    const sf::Vector2f position = sf::Vector2f(camera->GetView().getCenter().x + offsetX, offsetY);
    this->position = position;

    sprite.setPosition(position);

    if (!font.loadFromFile("Assets/Fonts/Pixel.ttf")) {
        throw std::runtime_error("Failed to load pixel font");
    }

    coinText.setFont(font);
    coinText.setCharacterSize(24);
    coinText.setFillColor(sf::Color::Black);
    coinText.setString("0");
    coinText.setPosition(position.x + 30, position.y);
}

void CoinBar::Update(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera> &camera)
{
    const sf::Vector2f position = sf::Vector2f(camera->GetView().getCenter().x + offsetX, offsetY);
    this->position = position;
    coins = player->GetCoins();

    UpdateView();
}

void CoinBar::UpdateView()
{
    sprite.setPosition(position);
    coinText.setPosition(position.x + 30, position.y);
    coinText.setString(std::to_string(coins));
}

void CoinBar::Draw(const std::shared_ptr<sf::RenderTarget> &rt) const
{
    rt->draw(sprite);
    rt->draw(coinText);
}