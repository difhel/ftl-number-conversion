#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <config.h>
#include <map>
#include <button.h>
#include <editor.h>
#include <layout.h>


class Dialog {
    public:
        Dialog(std::string title, Layout layout, std::function<void(sf::RenderWindow&, std::map<std::string, std::string>)> callback_function, sf::RenderWindow &parent_window) : parent_window{parent_window} {
            this->callback_function = callback_function;
            this->inputs = inputs;
            this->title = title;
            // add "Close" button
            DialogElement closebtn("Button", "Close", "close_btn");
            layout.addElement(closebtn);
            this->layout = layout;
            this->dialog_window = new sf::RenderWindow(sf::VideoMode(500, 600), title);
        }
        void run_callback() {
            // put text editors content to this->inputs_data and then call callback function
            for (auto input : inputs) {
                std::string input_text = "";
                for (std::string* line : input.second->lines) {
                    input_text += *line;
                    input_text += '\n';
                }
                if (!input_text.empty()) input_text.erase(input_text.end() - 1);
                this->inputs_data[input.first] = input_text;
            }
            this->callback_function(this->parent_window, this->inputs_data);
        }
        void start_polling() {
            while (this->dialog_window->isOpen()) {
                sf::Event event;
                while (this->dialog_window->pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        this->dialog_window->close();
                        this->run_callback();
                    }

                    if (event.type == sf::Event::Resized) {
                        // update the view to the new size of the window
                        // РЕСАЙЗИТЬ ДИАЛОГИ НЕЛЬЗЯ
                    }

                    if (event.type == sf::Event::MouseButtonPressed) {
                        std::cout << "(dialog) MouseButtonPressed\n";
                        for (auto button : buttons) {
                            if ((*(button.second)).isMouseOver(*(this->dialog_window))) {
                                std::cout << "(dialog) " << button.first + " button pressed" << std::endl;
                                if (button.first == "close_btn") {
                                    this->dialog_window->close();
                                    this->run_callback();
                                }
                            }
                        }
                        for (auto input : inputs) {
                            if ((*(input.second)).isMouseOver(*(this->dialog_window))) {
                                std::cout << "(dialog) " << input.first + " text edit activated" << std::endl;
                                active_editor = input.first;
                            }
                        }
                    }
                    if (event.type == sf::Event::TextEntered || event.type == sf::Event::KeyPressed) {              
                        if (this->active_editor == "") continue;
                        this->inputs[active_editor]->handleInput(event);
                        this->inputs[active_editor]->update();
                    }
                    this->show();
                }
            }
        }
        void generate() {
            sf::Font font;
            font.loadFromFile("../font.ttf");
            int last_y = config::window::button_margin;
            int x = config::window::button_margin;
            for (auto element : this->layout.build()) {
                // std::cout << "Rendering dialog with " << element.type << std::endl;
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
        void show() {
            this->dialog_window->clear(config::color::background);
            for (auto button : buttons) {
                button.second->draw(*(this->dialog_window));
            }
            for (auto input : inputs) {
                // std::cout << "(dialog) input " << input.second << " rerendered" << std::endl;
                input.second->draw(*(this->dialog_window));
            }
            this->dialog_window->display();
        }
    private:
        std::string title;
        Layout layout;
        std::map<std::string, TextEditor*> inputs;
        std::map<std::string, Button*> buttons;
        std::map<std::string, std::string> inputs_data;
        std::function<void(sf::RenderWindow&, std::map<std::string, std::string>)> callback_function;
        sf::RenderWindow* dialog_window;
        sf::RenderWindow &parent_window;
        std::string active_editor = "";
};

