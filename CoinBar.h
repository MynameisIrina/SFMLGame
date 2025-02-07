#pragma once
#include "TextureLoader.h"
#include "Player.h"
#include "Camera.h"

class CoinBar
{

public:
    CoinBar(const std::shared_ptr<TextureLoader> &txLoader);
    ~CoinBar() = default;

    void Initialize(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera> &camera);
    void Update(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera> &camera);
    void UpdateView();
    void Draw(const std::shared_ptr<sf::RenderTarget>& rt) const;
    void ExchangeForLife();

private:
    sf::Sprite sprite;
    sf::Vector2f position;
    int coins;
    sf::Text coinText;
    sf::Font font;
    const float offsetX = 200.f;
    const float offsetY = 40.f;
};