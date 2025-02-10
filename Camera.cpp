#include "Camera.h"
#include "Player.h"

Camera::Camera(const std::shared_ptr<sf::RenderWindow> &windowRef) : window(windowRef) {}

void Camera::Initialize()
{
    view.setSize(window->getSize().x, window->getSize().y);
    view.setCenter(window->getSize().x * 0.5f, window->getSize().y * 0.5f);
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
