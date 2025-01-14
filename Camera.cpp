#include "Camera.h"
#include "Player.h"

Camera::Camera(std::shared_ptr<sf::RenderWindow> windowRef) : window(windowRef){}

void Camera::Initialize()
{
    view.setSize(window->getSize().x, window->getSize().y);
    view.setCenter(window->getSize().x / 2, window->getSize().y / 2);
    bool isCameraFollowing = false;
}

void Camera::Update(const std::shared_ptr<Player> player, bool moveLeft, bool respawn)
{
    sf::Vector2f playerPosition = player->GetPosition();
    float viewThreshold = view.getCenter().x;

    if (respawn)
    {
        view.setCenter(window->getSize().x / 2, window->getSize().y / 2);
        isCameraFollowing = false;
    }
    else
    {
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
    }

    window->setView(view);
}

sf::View Camera::GetView() const
{
    return view;
}

int Camera::CalculateLeftBound() const
{
    return view.getCenter().x - view.getSize().x / 2.f;
}

