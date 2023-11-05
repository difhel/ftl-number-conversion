#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <config.h>
#include <iostream>
#include <locale>
#include <codecvt>
#include <sstream>
#include <iomanip>


class TextEditor {
public:
    std::vector<std::string*> lines;
    TextEditor(
        unsigned int charSize,
        unsigned int width,
        unsigned int height,
        unsigned int x,
        unsigned int y,
        bool fullscreen) {
        rectangle.setSize(sf::Vector2f(500, 1000)); // Set the width and height of the rectangle
        rectangle.setFillColor(config::color::secondary_container); // Set the fill color
        rectangle.setPosition(x, y);
        font.loadFromFile("../font.ttf");
        text.setFont(font);
        text.setCharacterSize(charSize);
        text.setFillColor(config::color::on_secondary_container);
        this->width = width;
        this->height = height;
        this->fullscreen = fullscreen;
        this->x = x;
        this->y = y;
        cursorX = 0;
        cursorY = 0;

        // Create the first line
        lines.push_back(new std::string(""));
        std::cout << "New text editor created" << std::endl;
    }

    bool isMouseOver(const sf::RenderWindow& window) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        return rectangle.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition));
    }

    void handleInput(sf::Event event) {
        // std::cout << event.type << "\n";
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::BackSpace) {
                // Handle backspace
                if (cursorX > 0) {
                    lines[cursorY]->erase(cursorX - 1, 1);
                    cursorX--;
                } else if (cursorY > 0) {
                    // If at the beginning of the line, merge with the previous line
                    cursorX = lines[cursorY - 1]->length();
                    *(lines[cursorY - 1]) += *(lines[cursorY]);
                    lines.erase(lines.begin() + cursorY);
                    cursorY--;
                }
            } else if (event.key.code == sf::Keyboard::Return) {
                // Handle Enter key
                std::string restOfLine = lines[cursorY]->substr(cursorX);
                lines[cursorY]->erase(cursorX);
                cursorX = 0;
                cursorY++;
                lines.insert(lines.begin() + cursorY, new std::string(restOfLine));
            }
        }
        if (event.type == sf::Event::TextEntered) {
                // std::cout << "Text entered\n";
                // std::cout << event.text.unicode << ": ";
                std::wcout << sf::String(event.text.unicode).toWideString() << L"\n";
                if (event.text.unicode >= 32 && event.text.unicode < 128) {
                    auto eee = static_cast<char>(event.text.unicode);
                    *(lines[cursorY]) += eee;
                    cursorX++;
                }
            }
    }

    void update() {
        // Ensure the cursor is within the bounds of the text
        if (cursorY >= static_cast<int>(lines.size())) {
            cursorY = lines.size() - 1;
        }
        if (cursorX > static_cast<int>(lines[cursorY]->length())) {
            cursorX = lines[cursorY]->length();
        }
    }

    void draw(sf::RenderWindow& window) {
        int editor_width = (this->fullscreen ? window.getSize().x - config::window::button_width - 2 * config::window::button_margin : this->width);
        int editor_height = (this->fullscreen ? window.getSize().y : this->height);
        rectangle.setSize(sf::Vector2f(editor_width, editor_height));
        window.draw(rectangle);

        float delta_x = (this->fullscreen ? 10.0f : this->x);
        float delta_y = (this->fullscreen ? 0 : this->y);
        float yOffset = 0;
        for (unsigned int i = 0; i < lines.size(); i++) {
            // auto eee = ws2s(lines[i]);
            std::stringstream ss;
            // ss << std::setfill('0');
            ss << std::setfill('0') << std::setw(2) << i + 1;
            std::string s;
            ss >> s;
            s = fullscreen ? s + " " : "";
            text.setString(s + *(lines[i]));
            // std::cout << "setting text " << *(lines[i]) << std::endl;
            text.setPosition(delta_x, delta_y + yOffset);
            window.draw(text);
            yOffset += text.getCharacterSize();
        }
        // Draw the cursor
        int cursor_x_offset = fullscreen ? 3 : 0;
        float cursorXPos = delta_x + (cursorX + cursor_x_offset) * text.getCharacterSize() / 1.71;
        float cursorYPos = delta_y + cursorY * text.getCharacterSize();
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(cursorXPos, cursorYPos), sf::Color::Black),
            sf::Vertex(sf::Vector2f(cursorXPos, cursorYPos + text.getCharacterSize()), sf::Color::Black)
        };
        window.draw(line, 2, sf::Lines);
    }

private:
    sf::Text text;
    sf::RectangleShape rectangle;
    int cursorX;
    int cursorY;
    unsigned int width, height;
    bool fullscreen;
    unsigned int x, y;
    sf::Font font;
};

// int main() {

//     while (window.isOpen()) {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             if (event.type == sf::Event::Closed) {
//                 window.close();
//             }
//             editor.handleInput(event);
//         }

//         editor.update();

//         window.clear(sf::Color::White);
//         editor.draw(window);
//         window.display();
//     }

//     return 0;
// }
