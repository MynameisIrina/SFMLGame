#pragma once
#include "TextureLoader.h"
#include "Camera.h"

class Player;

class HealthBar
{

public:

struct Heart
{
    sf::Sprite sprite;
    sf::Vector2f position;
    bool isActive;
};

HealthBar(std::shared_ptr<TextureLoader> txLoaderRef, std::shared_ptr<Camera> cam);
void Initialize(const std::shared_ptr<Player> player);
void Draw(const std::shared_ptr<sf::RenderTarget> rt) const;
void Update(const std::shared_ptr<Camera> cam, const std::shared_ptr<Player> player);
void UpdateView();



private:

std::vector<Heart> hearts;
std::shared_ptr<Camera> camera;
static std::shared_ptr<TextureLoader> txLoader;
static constexpr float offsetX = 200.f;
static constexpr float offsetY = 10.f;
static constexpr float scale = 2;

};