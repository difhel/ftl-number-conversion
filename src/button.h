#pragma once
#include <SFML/Graphics.hpp>
#include <config.h>


class Button {
public:
    Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text, bool is_label=false) {
        buttonShape.setSize(size);
        buttonShape.setPosition(position);
        font.loadFromFile("../font.ttf");
        buttonText.setFont(font);
        buttonText.setString(text);
        buttonText.setCharacterSize(16);
        
        if (!is_label) {
            buttonShape.setFillColor(config::color::primary); // Цвет фона кнопки
            buttonText.setFillColor(config::color::on_primary); // Цвет текста

            // Определение позиции текста, чтобы он был выровнен по центру кнопки
            sf::FloatRect textBounds = buttonText.getLocalBounds();
            float x = position.x + (size.x - textBounds.width) / 2;
            float y = position.y + (size.y - textBounds.height) / 2 - textBounds.top;
            buttonText.setPosition(x, y);
        } else {
            // it is a label - button without background fill and text aligment
            buttonShape.setFillColor(config::color::background); // Цвет фона кнопки
            buttonText.setFillColor(config::color::on_background); // Цвет текста
            sf::FloatRect textBounds = buttonText.getLocalBounds();
            float x = position.x;
            float y = position.y + (size.y - textBounds.height) / 2 - textBounds.top;
            buttonText.setPosition(x, y);
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(buttonShape);
        window.draw(buttonText);
    }

    bool isMouseOver(const sf::RenderWindow& window) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        return buttonShape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition));
    }

private:
    sf::RectangleShape buttonShape;
    sf::Text buttonText;
    sf::Font font;
};
