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
std::string active_editor = ""; // id of active TextEditor
std::map<std::string, std::string> inputs_data;

void render(sf::RenderWindow &window, Layout &layout) {
    // render the layout
    sf::Font font;
    font.loadFromFile("../font.ttf");
    int last_y = config::window::button_margin;
    int x = config::window::button_margin;
    for (auto element : layout.build()) {
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
    // show previously rendered layout
    window.clear(config::color::background);
    for (auto button : buttons) {
        button.second->draw(window);
    }
    for (auto input : inputs) {
        input.second->draw(window);
    }
    window.display();
}

void parseInputs() {
    for (auto input : inputs) {
        std::string input_text = "";
        for (std::string* line : input.second->lines) {
            input_text += *line;
            input_text += '\n';
        }
        if (!input_text.empty()) input_text.erase(input_text.end() - 1);
        inputs_data[input.first] = input_text;
    }
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
    layout.addElement(DialogElement("Button", "Run", "run_button"));

    sf::RenderWindow window(sf::VideoMode(400, 600), config::window::title);
    sf::Font font;
    font.loadFromFile("../font.ttf");
    render(window, layout);
    show(window);

    while (window.isOpen()) {
        // event loop
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::Resized) {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
                render(window, layout);
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                std::cout << "MouseButtonPressed\n";
                for (auto button : buttons) {
                    if ((*(button.second)).isMouseOver(window)) {
                        // button click event
                        std::cout << button.first + " button pressed" << std::endl;
                        if (button.first == "run_button") {
                            parseInputs();
                            std::string res = callback_test_dialog(window, inputs_data);

                            Layout layout;
                            layout.addElement(DialogElement("Label", "Result:", "label_1"));
                            layout.addElement(DialogElement("Label", res, "result"));
                            Dialog dialog("Insert substring in line", layout, callback_empty, window);
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
