#pragma once
#include "TextureLoader.h"
#include "Camera.h"


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
void Initialize(int amount);
void Draw(const std::shared_ptr<sf::RenderTarget> rt) const;
void Update(const std::shared_ptr<Camera> cam);
void UpdateView();
void LoseLife();
void GainLife();



private:

std::vector<Heart> hearts;
std::shared_ptr<Camera> camera;
std::shared_ptr<TextureLoader> txLoader;
float offsetX = 200.f;
float offsetY = 10.f;
float scale = 2;

};