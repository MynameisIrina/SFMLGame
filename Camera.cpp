// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "Camera.h"
#include "Player.h"

Camera::Camera(const std::shared_ptr<sf::RenderWindow> &windowRef) : window(windowRef) {}

void Camera::Initialize()
{
    auto windowSize = window->getSize();
    view.setSize(windowSize.x, windowSize.y);
    view.setCenter(windowSize.x * 0.5f, windowSize.y * 0.5f);
}

void Camera::Update(const std::shared_ptr<Player> &player)
{
    const sf::Vector2f playerPosition = player->GetPosition();
    const float viewThreshold = view.getCenter().x;

    bool isMovingRight = player->CalculateDirection() > 0 && (player->IfStateActive(Player::State::Moving));
    if (playerPosition.x >= viewThreshold && isMovingRight)
    {
        view.setCenter(playerPosition.x, view.getSize().y * 0.5f);
    }

    window->setView(view);
}

void Camera::Reset()
{
    view.setCenter(window->getSize().x * 0.5f, window->getSize().y * 0.5f);
}

sf::View Camera::GetView() const
{
    return view;
}

int Camera::CalculateLeftBound() const
{
    return view.getCenter().x - view.getSize().x * 0.5f;
}

int Camera::CalculateRightBound() const
{
    return view.getCenter().x + view.getSize().x * 0.5f;
}
