#include "Player.h"
#include <iostream>
#include "HealthBar.h"

Player::Player(std::shared_ptr<TextureLoader> txLoaderRef): txLoader(txLoaderRef)
{
}

void Player::Initialize(const sf::Vector2f &pos)
{
    position = pos;

    boundingRecPlayer.setSize(sf::Vector2f(30, 40));
    boundingRecPlayer.setFillColor(sf::Color::Transparent);
    boundingRecPlayer.setOutlineColor(sf::Color::Red);
    boundingRecPlayer.setOutlineThickness(1);

    sprite = txLoader->SetSprite(TextureLoader::TextureType::Player);

    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
    sprite.setScale(AnimationPlayer::SCALE, AnimationPlayer::SCALE);
    sprite.setPosition(pos);
}

void Player::Update(bool moveRight, bool moveLeft, float leftBound, float dt, std::vector<sf::RectangleShape> &boundRecs, std::shared_ptr<HealthBar> healthbar)
{

    CalculateCurrAnimation(dt);

    float offset = 27.f;

    if(canLoseLife && position.y > 800)
    {
        healthbar->LoseLife();
        canLoseLife = false;
    }

    if (collisionTop)
    {
        velocity.y = 0;
    }

    if (!collisionGround)
    {
        velocity.y += gravity * dt;
        collisionTop = false;
    }
    else
    {
        velocity.y = 0;
    }

    if ((collisionGround && !collisionSide) || (!collisionGround && !collisionSide))
    {
        velocity.x = 100.f;
    }

    position.y += velocity.y * dt;

    if (moveRight)
    {
        stopped = false;
        atRespawnPos = false;
        position.x += velocity.x * dt;
    }
    else if (moveLeft)
    {
        stopped = false;
        atRespawnPos = false;
        if (position.x > leftBound + offset)
        {
            position.x -= velocity.x * dt;
        }
    }
    else
    {
        stopped = true;
    }

    CheckCollisionGround(boundRecs);
    CheckCollisionSide(boundRecs);



}

void Player::CheckCollisionSide(std::vector<sf::RectangleShape> &boundRecs)
{
    collisionSide = false;

    bool isMovingRight = sprite.getScale().x > 0;
    bool isMovingLeft = sprite.getScale().x < 0;

    if (isMovingRight)
    {
        if (castRay(boundingRecPlayer, boundRecs, boundingRecPlayer.getSize().x, true))
        {
            collisionSide = true;
            velocity.x = 0;
        }
    }
    else if (isMovingLeft)
    {
        if (castRay(boundingRecPlayer, boundRecs, boundingRecPlayer.getSize().x, false))
        {
            collisionSide = true;
            velocity.x = 0;
        }
    }
}

void Player::drawRay(std::shared_ptr<sf::RenderTarget> window, const sf::Vector2f &start, const sf::Vector2f &end, sf::Color color)
{
    sf::VertexArray ray(sf::Lines, 2);
    ray[0].position = start;
    ray[0].color = color; // Starting point color
    ray[1].position = end;
    ray[1].color = color; // Ending point color

    window->draw(ray);
}

bool Player::castRay(const sf::RectangleShape &player, std::vector<sf::RectangleShape> &platforms, float rayLength, bool isMovingRight)
{
    sf::Vector2f rayStart = boundingRecPlayer.getPosition() + sf::Vector2f(boundingRecPlayer.getSize().x / 2, boundingRecPlayer.getSize().y / 2);

    float direction = isMovingRight ? 1.0f : -1.0f;
    sf::Vector2f rayEnd = rayStart + sf::Vector2f(direction * rayLength, 0);

    sf::VertexArray ray(sf::Lines, 2);
    ray[0].position = rayStart;
    ray[1].position = rayEnd;

    sf::Vector2f rayStart2 = boundingRecPlayer.getPosition() + sf::Vector2f(boundingRecPlayer.getSize().x / 2, 0);

    sf::Vector2f rayEnd2 = rayStart2 + sf::Vector2f(direction * rayLength, 0);

    sf::VertexArray ray2(sf::Lines, 2);
    ray2[0].position = rayStart2;
    ray2[1].position = rayEnd2;

    float epsilon = 3.f;

    for (auto &platform : platforms)
    {
        bool ray1Collision_Right = ray[0].position.x < platform.getGlobalBounds().left + platform.getGlobalBounds().width &&
                                   ray[1].position.x > platform.getGlobalBounds().left &&
                                   ray[0].position.y > platform.getGlobalBounds().top &&
                                   ray[0].position.y < platform.getGlobalBounds().top + platform.getGlobalBounds().height;
        bool ray2Collision_Right = ray2[0].position.x < platform.getGlobalBounds().left + platform.getGlobalBounds().width &&
                                   ray2[1].position.x > platform.getGlobalBounds().left &&
                                   ray2[0].position.y > platform.getGlobalBounds().top &&
                                   ray2[0].position.y < platform.getGlobalBounds().top + platform.getGlobalBounds().height;
        bool ray1Collision_Left = ray[1].position.x < platform.getGlobalBounds().left + platform.getGlobalBounds().width &&
                                  ray[0].position.x > platform.getGlobalBounds().left &&
                                  ray[0].position.y > platform.getGlobalBounds().top &&
                                  ray[0].position.y < platform.getGlobalBounds().top + platform.getGlobalBounds().height;
        bool ray2Collision_Left = ray2[1].position.x < platform.getGlobalBounds().left + platform.getGlobalBounds().width &&
                                  ray2[0].position.x > platform.getGlobalBounds().left &&
                                  ray2[0].position.y > platform.getGlobalBounds().top &&
                                  ray2[0].position.y < platform.getGlobalBounds().top + platform.getGlobalBounds().height;

        if (ray1Collision_Right || ray2Collision_Right)
        {
            platform.setOutlineColor(sf::Color::Red);
            position.x = platform.getGlobalBounds().left - boundingRecPlayer.getGlobalBounds().width + epsilon;
            return true;
        }
        else if (ray1Collision_Left || ray2Collision_Left)
        {
            platform.setOutlineColor(sf::Color::Cyan);
            position.x = platform.getGlobalBounds().left + platform.getGlobalBounds().width + boundingRecPlayer.getGlobalBounds().width - epsilon;
            return true;
        }
    }

    return false;
}

void Player::Jump(bool jump, float dt)
{
    if (jump && !isJumping)
    {
        isJumping = true;
        collisionGround = false;
        velocity.y = -280.f;
    }
}

void Player::ResetAnimation(int animYIndex)
{
    sprite.setTextureRect(sf::IntRect(0, animYIndex * AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE));
}

void Player::CheckCollisionGround(std::vector<sf::RectangleShape> &boundRecs)
{
    collisionGround = false;
    collisionTop = false;

    for (auto &boundRec : boundRecs)
    {
        sf::FloatRect playerBounds = boundingRecPlayer.getGlobalBounds();
        sf::FloatRect tileBounds = boundRec.getGlobalBounds();

        if (Math::CheckRectCollision(boundingRecPlayer.getGlobalBounds(), boundRec.getGlobalBounds()))
        {
            boundRec.setOutlineColor(sf::Color::Green);

            float overlapTop = playerBounds.top - (tileBounds.top + tileBounds.height);
            float overlapBottom = tileBounds.top - (playerBounds.top + playerBounds.height);

            if (overlapBottom <= 0.f && overlapBottom >= -5.f)
            {
                collisionGround = true;
                isJumping = false;
                continue;
            }
            if (overlapTop <= 0.f && overlapTop >= -5.f)
            {
                collisionTop = true;
                continue;
            }
        }
    }
}

void Player::CalculateCurrAnimation(float dt)
{
    // calculate current sprite sheet image
    animationTimer += dt;
    if (animationTimer >= animationInterval)
    {
        currentAnim++;

        // if the player stopped moving, reset its animation
        if ((currentAnim >= 8 || stopped) && !isJumping)
        {
            currentAnim = 0;
            ResetAnimation(AnimationPlayer::STOP_MOVING);
        }
        // if the player is jumping, play only one animation unit
        else if (currentAnim >= 0 && isJumping)
        {
            currentAnim = 0;
            ResetAnimation(AnimationPlayer::STOPP_JUMPING);
        }
        animationTimer = 0.f;
    }
}

void Player::Respawn()
{
    if (!atRespawnPos)
    {
        saveLastPos = position;
        position = respawnPos;
        // stop the player if it was falling down
        velocity.y = 0;
        atRespawnPos = true;
        canLoseLife = true;
    }
    else
    {
        std::cout << "Already at respawn point" << std::endl;
    }
}

sf::Vector2f Player::GetLastSavedPos()
{
    return saveLastPos;
}


void Player::UpdateView(bool moveRight, bool moveLeft)
{
    if (moveRight)
    {
        sprite.setScale(AnimationPlayer::SCALE, AnimationPlayer::SCALE);
        sprite.setTextureRect(sf::IntRect(currentAnim * AnimationPlayer::TILE_SIZE, AnimationPlayer::MOVING_Y * AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE));
    }
    else if (moveLeft)
    {
        sprite.setScale(-1 * AnimationPlayer::SCALE, AnimationPlayer::SCALE);
        sprite.setTextureRect(sf::IntRect(currentAnim * AnimationPlayer::TILE_SIZE, AnimationPlayer::MOVING_Y * AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE));
    }

    sprite.setPosition(position);
    boundingRecPlayer.setPosition(sprite.getPosition().x - 16, sprite.getPosition().y + 10);
}

void Player::Draw(std::shared_ptr<sf::RenderTarget> rt)
{
    rt->draw(sprite);
    rt->draw(boundingRecPlayer);

    sf::Vector2f rayStart = boundingRecPlayer.getPosition() + sf::Vector2f(boundingRecPlayer.getSize().x / 2, boundingRecPlayer.getSize().y / 2);

    float direction = sprite.getScale().x > 0 ? 1.0f : -1.0f;
    sf::Vector2f rayEnd = rayStart + sf::Vector2f(direction * boundingRecPlayer.getSize().x, 0);

    sf::VertexArray ray(sf::Lines, 2);
    ray[0].position = rayStart;
    ray[1].position = rayEnd;

    drawRay(rt, rayStart, rayEnd);

    sf::Vector2f rayStart2 = boundingRecPlayer.getPosition() + sf::Vector2f(boundingRecPlayer.getSize().x / 2, 0);

    sf::Vector2f rayEnd2 = rayStart2 + sf::Vector2f(direction * boundingRecPlayer.getSize().x, 0);

    sf::VertexArray ray2(sf::Lines, 2);
    ray2[0].position = rayStart2;
    ray2[1].position = rayEnd2;

    drawRay(rt, rayStart2, rayEnd2);
}

sf::Vector2f Player::GetPosition() const
{
    return position;
}
