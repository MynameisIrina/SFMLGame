#include <SFML/Graphics.hpp>

class Button
{
public:
    Button() = default;
    Button(float x, float y, float width, float height, const std::string &text, sf::Font &font)
    {
        button.setSize(sf::Vector2f(width, height));
        button.setPosition(x, y);
        button.setFillColor(sf::Color::Black);

        label.setFont(font);
        label.setString(text);
        label.setCharacterSize(30);
        label.setFillColor(sf::Color::White);
        label.setPosition(
            x + (width - label.getGlobalBounds().width) / 2,
            y);
    }

    void Draw(std::shared_ptr<sf::RenderWindow> &window)
    {
        window->draw(button);
        window->draw(label);
    }

    bool IsClicked(const sf::Vector2i &mousePos)
    {
        return button.getGlobalBounds().contains(sf::Vector2f(mousePos));
    }

    void SetColor(const sf::Color &color)
    {
        button.setFillColor(color);
    }

private:
    sf::RectangleShape button;
    sf::Text label;
};
