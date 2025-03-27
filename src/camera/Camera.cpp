#include "camera/Camera.h"
#include "player/Player.h"

Camera::Camera(const std::shared_ptr<sf::RenderWindow> &windowRef) : window(windowRef) {}

void Camera::Initialize()
{
    auto windowSize = window->getSize();
    view.setSize(windowSize.x, windowSize.y);
    view.setCenter(windowSize.x * 0.5f, windowSize.y * 0.5f);
    topBound = topBoundOffset;
    bottomBound = windowSize.y - bottomBoundOffset;
}

void Camera::Update(const std::shared_ptr<Player> &player, const float dt)
{
    const sf::Vector2f prevPos = view.getCenter();
    const sf::Vector2f playerPosition = player->GetPosition();
    const float viewThreshold = view.getCenter().x;
    float targetX = view.getCenter().x;
    const bool isMovingRight = player->CalculateDirection() == Player::Direction::Right && player->IfStateActive(Player::State::Moving);

    if (playerPosition.x >= viewThreshold && isMovingRight) {
        targetX = playerPosition.x;
    }

    // Smooth interpolation
    float currentX = view.getCenter().x;
    float newX = currentX + (targetX - currentX) * cameraSmoothingFactor * dt;
    view.setCenter(newX, view.getSize().y * 0.5f);
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

int Camera::CalculateTopBound() const
{
    return topBound;
}

int Camera::CalculateBottomBound() const
{
    return bottomBound;
}
