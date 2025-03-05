#include "Camera.h"
#include "Player.h"

Camera::Camera(const std::shared_ptr<sf::RenderWindow> &windowRef) : window(windowRef) {}

void Camera::Initialize()
{
    auto windowSize = window->getSize();
    view.setSize(windowSize.x, windowSize.y);
    view.setCenter(windowSize.x * 0.5f, windowSize.y * 0.5f);
    topBound = 40.f;
    bottomBound = windowSize.y - 40.f;
}

void Camera::Update(const std::shared_ptr<Player> &player, const float dt)
{
    sf::Vector2f prevPos = view.getCenter();

    const sf::Vector2f playerPosition = player->GetPosition();
    float targetX = view.getCenter().x;
    const float viewThreshold = view.getCenter().x;

    bool isMovingRight = player->CalculateDirection() > 0 && player->IfStateActive(Player::State::Moving);
    if (playerPosition.x >= viewThreshold && isMovingRight) {
        targetX = playerPosition.x;
    }

    // Smooth interpolation
    float currentX = view.getCenter().x;
    float newX = currentX + (targetX - currentX) * 5.0f * dt;
    view.setCenter(newX, view.getSize().y * 0.5f);
    window->setView(view);

    camVelocity = (view.getCenter().x - prevPos.x) / dt;
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

int Camera::CalculateTopBound() const
{
    return topBound;
}

int Camera::CalculateBottomBound() const
{
    return bottomBound;
}
