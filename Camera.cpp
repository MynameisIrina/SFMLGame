#include "Camera.h"

Camera::Camera(sf::RenderWindow &windowRef, const Player &playerRef) : window(windowRef), player(playerRef) {}

void Camera::Initialize()
{
    view.setSize(window.getSize().x, window.getSize().y);
    view.setCenter(window.getSize().x / 2, window.getSize().y / 2);
    bool isCameraFollowing = false;
}

void Camera::Update(bool moveLeft)
{
    sf::Vector2f playerPosition = player.GetPosition();

    float viewThreshold = view.getCenter().x;

    if (playerPosition.x >= viewThreshold)
    {
        isCameraFollowing = true;
    }

    if (moveLeft)
    {
        isCameraFollowing = false;
    }

    if (isCameraFollowing)
    {
        view.setCenter(playerPosition.x, view.getSize().y / 2);
    }

    window.setView(view);
}

sf::View Camera::GetView() const
{
    return view;
}

float Camera::CalculateLeftBound() const
{
    return view.getCenter().x - view.getSize().x / 2.f;
}

float Camera::CalculateRightBound() const
{
    return view.getCenter().x + view.getSize().x / 2.f;
}
