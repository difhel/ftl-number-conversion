#include <iostream>
#include <SFML/Graphics.hpp>
#include <config.h>
#include <map>
#include <button.h>
#include <editor.h>
#include <algo.h>
#include <dialog.h>
#include <callbacks.h>
#define MALLOC_CHECK_ 2

std::map<std::string, Button*> buttons;
std::map<std::string, TextEditor*> inputs;
std::string active_editor = "";

void render(sf::RenderWindow &window, Layout &layout) {
    sf::Font font;
    font.loadFromFile("../font.ttf");
    int last_y = config::window::button_margin;
    int x = config::window::button_margin;
    for (auto element : layout.build()) {
        // std::cout << "Rendering window with " << element.type << std::endl;
        int last_element_height = 50;
        if (element.type == "Button") {
            buttons[element.id] = new Button(
                sf::Vector2f(x, last_y),
                sf::Vector2f(config::window::button_width, 50),
                element.text);
        }
        if (element.type == "Label") {
            buttons[element.id] = new Button(
                sf::Vector2f(x, last_y),
                sf::Vector2f(config::window::button_width, 20),
                element.text,
                true);
            last_element_height = 20;
        }
        if (element.type == "TextEditor") {
            inputs[element.id] = new TextEditor(24, config::window::button_width, 50, x, last_y, false);
        }
        last_y += last_element_height + config::window::button_margin;
    }
}

void show(sf::RenderWindow &window) {
    window.clear(config::color::background);
    for (auto button : buttons) {
        button.second->draw(window);
    }
    for (auto input : inputs) {
        // std::cout << "(dialog) input " << input.second << " rerendered" << std::endl;
        input.second->draw(window);
    }
    window.display();
}

int main() {
    std::locale::global(std::locale("C.utf8"));

    Layout layout;
    layout.addElement(DialogElement("Label", "Universal convertor", "label_1"));
    layout.addElement(DialogElement("Label", "Enter number:", "label_2"));
    layout.addElement(DialogElement("TextEditor", "", "n"));
    layout.addElement(DialogElement("Label", "Enter p:", "label_3"));
    layout.addElement(DialogElement("TextEditor", "", "p"));
    layout.addElement(DialogElement("Label", "Enter q:", "label_4"));
    layout.addElement(DialogElement("TextEditor", "", "q"));
    layout.addElement(DialogElement("Button", "Run", "button"));
    // Dialog dialog("Insert substring in line", layout, callback_insert_substr_in_line, window, editor);

    sf::RenderWindow window(sf::VideoMode(800, 600), config::window::title);
    sf::Font font;
    font.loadFromFile("../font.ttf");
    render(window, layout);
    show(window);
    // button.buttonShape.setRadius(30);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::Resized) {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
                // editor.draw(window);
                render(window, layout);
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                std::cout << "MouseButtonPressed\n";
                for (auto button : buttons) {
                    if ((*(button.second)).isMouseOver(window)) {
                        // Код, который выполняется при нажатии на кнопку
                        // Здесь можно добавить логику обработки нажатия кнопки
                        std::cout << button.first + " button pressed" << std::endl;
                        if (button.first == "button") {
                            Layout layout;
                            layout.addElement(DialogElement("Label", "Result:", "label_1"));
                            layout.addElement(DialogElement("TextEditor", "1", "n"));
                            Dialog dialog("Insert substring in line", layout, callback_insert_substr_in_line, window);
                            dialog.generate();
                            dialog.show();
                            dialog.start_polling();
                        }
                    }
                }
                for (auto input : inputs) {
                    if ((*(input.second)).isMouseOver(window)) {
                        std::cout << "(dialog) " << input.first + " text edit activated" << std::endl;
                        active_editor = input.first;
                    }
                }
            }

            if (event.type == sf::Event::TextEntered || event.type == sf::Event::KeyPressed) {              
                if (active_editor == "") continue;
                    inputs[active_editor]->handleInput(event);
                    inputs[active_editor]->update();
            }
            show(window);
        }
    }

    return 0;
}
