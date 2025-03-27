#include <SFML/Graphics.hpp>
#include "ui/menu/Button.h"
#include "core/GameState.h"
#include "graphics/TextureLoader.h"

class Menu
{

public:
    Menu(std::shared_ptr<sf::RenderWindow> &window, std::shared_ptr<TextureLoader> &txLoader);

    void Show(GameState &gameState, const float dt);

    void Draw();

    void UpdateCatAnimation(const float dt);
private:
    std::shared_ptr<sf::RenderWindow> window;
    sf::Font font;
    Button startButton;
    Button exitButton;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Text title;
    sf::Sprite cat;
    float animationTimer = 0.f;
    const int maxFrames = 3;
    const float animationInterval = 0.2f;
    int currentAnimationX = 0;
    int currentAnimationY = 810;
    const int minAnimationY = 810;
    const int maxAnimationY = 1000;
    const int diffBetweenAnimation = 32;
    const int frameSize = 32;


};
