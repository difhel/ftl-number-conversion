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

void render(sf::RenderWindow &window, sf::Font &font, TextEditor &editor) {
    int last_button_y = config::window::button_margin;
    int buttons_x = window.getSize().x - config::window::button_width - config::window::button_margin;
    buttons.clear();
    for (auto button_config : config::window::buttons_config) {
        buttons[button_config.first] = new Button(
            sf::Vector2f(buttons_x, last_button_y),
            sf::Vector2f(config::window::button_width, 50),
            button_config.second);
        last_button_y = last_button_y + 50 + config::window::button_margin;
    }
    window.clear(config::color::background);
    for (auto button : buttons) (*(button.second)).draw(window);
    editor.draw(window);
    window.display();
}
int main() {
    std::locale::global(std::locale("C.utf8"));
    sf::RenderWindow window(sf::VideoMode(800, 600), config::window::title);
    sf::Font font;
    font.loadFromFile("../font.ttf");
    TextEditor editor(24, 800, 600, 0, 0, true);
    render(window, font, editor);
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
                render(window, font, editor);
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                std::cout << "MouseButtonPressed\n";
                for (auto button : buttons) {
                    if ((*(button.second)).isMouseOver(window)) {
                        // Код, который выполняется при нажатии на кнопку
                        // Здесь можно добавить логику обработки нажатия кнопки
                        std::cout << button.first + " button pressed" << std::endl;
                        if (button.first == "insert_substr_in_line") {
                            Layout layout;
                            layout.addElement(DialogElement("Label", "Line number (N)", "label_1"));
                            layout.addElement(DialogElement("TextEditor", "", "n"));
                            layout.addElement(DialogElement("Label", "Symbol number (M)", "label_2"));
                            layout.addElement(DialogElement("TextEditor", "", "m"));
                            layout.addElement(DialogElement("Label", "Substring to insert", "label_3"));
                            layout.addElement(DialogElement("TextEditor", "", "substr"));
                            Dialog dialog("Insert substring in line", layout, callback_insert_substr_in_line, window, editor);
                            dialog.generate();
                            dialog.show();
                            dialog.start_polling();
                        }
                        if (button.first == "remove_zeros") {
                            Layout layout;
                            layout.addElement(DialogElement("Label", "First line (n)", "label_1"));
                            layout.addElement(DialogElement("TextEditor", "", "n"));
                            layout.addElement(DialogElement("Label", "Last line (m)", "label_2"));
                            layout.addElement(DialogElement("TextEditor", "", "m"));
                            Dialog dialog("Remove leading zeros", layout, callback_remove_leading_zeros, window, editor);
                            dialog.generate();
                            dialog.show();
                            dialog.start_polling();
                        }
                        if (button.first == "replace_symbol") {
                            Layout layout;
                            layout.addElement(DialogElement("Label", "Line number (n)", "label_1"));
                            layout.addElement(DialogElement("TextEditor", "", "n"));
                            layout.addElement(DialogElement("Label", "Symbol number (m)", "label_2"));
                            layout.addElement(DialogElement("TextEditor", "", "m"));
                            layout.addElement(DialogElement("Label", "Symbol to replace", "label_3"));
                            layout.addElement(DialogElement("TextEditor", "", "c"));
                            Dialog dialog("Replace symbol", layout, callback_replace_symbol, window, editor);
                            dialog.generate();
                            dialog.show();
                            dialog.start_polling();
                        }
                        if (button.first == "delete_line") {
                            Layout layout;
                            layout.addElement(DialogElement("Label", "Line to delete (n)", "label_1"));
                            layout.addElement(DialogElement("TextEditor", "", "n"));
                            Dialog dialog("Delete line", layout, callback_delete_line, window, editor);
                            dialog.generate();
                            dialog.show();
                            dialog.start_polling();
                        }
                        if (button.first == "test_dialog") {
                            Layout layout;
                            DialogElement le1("Label", "Test button", "1");
                            layout.addElement(le1);
                            DialogElement le2("TextEditor", "Test button", "2");
                            DialogElement le3("TextEditor", "Test button", "3");
                            layout.addElement(le2);
                            layout.addElement(le3);
                            
                            Dialog dialog("Test dialog", layout, callback_test_dialog, window, editor);
                            dialog.generate();
                            dialog.show();
                            dialog.start_polling();
                        }
                        if (button.first == "insert_after_line") {
                            Layout layout;
                            layout.addElement(DialogElement("Label", "Insert after line (n)", "label_1"));
                            layout.addElement(DialogElement("TextEditor", "", "n"));
                            layout.addElement(DialogElement("Label", "One line to insert", "label_2"));
                            layout.addElement(DialogElement("TextEditor", "", "lines"));
                            Dialog dialog("Insert line", layout, callback_multiple_insert_after_line, window, editor);
                            dialog.generate();
                            dialog.show();
                            dialog.start_polling();
                        }
                        if (button.first == "remove_numbers") {
                            callback_remove_non_increasing(window, {}, editor);
                        }
                        if (button.first == "remove_{}") {
                            Layout layout;
                            layout.addElement(DialogElement("Label", "Line to remove {...} (n)", "label_1"));
                            layout.addElement(DialogElement("TextEditor", "", "n"));
                            Dialog dialog("Remove {...}", layout, callback_remove_braces, window, editor);
                            dialog.generate();
                            dialog.show();
                            dialog.start_polling();
                        }
                        if (button.first == "replace") {
                            Layout layout;
                            layout.addElement(DialogElement("Label", "First line (n) (default 1)", "label_1"));
                            layout.addElement(DialogElement("TextEditor", "", "n"));
                            layout.addElement(DialogElement("Label", "Last line (m) (default MAX_LINE)", "label_2"));
                            layout.addElement(DialogElement("TextEditor", "", "m"));
                            layout.addElement(DialogElement("Label", "Replace ...", "label_3"));
                            layout.addElement(DialogElement("TextEditor", "", "what"));
                            layout.addElement(DialogElement("Label", "Replace to ...", "label_4"));
                            layout.addElement(DialogElement("TextEditor", "", "to"));
                            Dialog dialog("Replace", layout, callback_replace, window, editor);
                            dialog.generate();
                            dialog.show();
                            dialog.start_polling();
                        }
                        if (button.first == "replace_*_+") {
                            Layout layout;
                            layout.addElement(DialogElement("Label", "Char *", "label_1"));
                            layout.addElement(DialogElement("TextEditor", "", "c1"));
                            layout.addElement(DialogElement("Label", "Char +", "label_2"));
                            layout.addElement(DialogElement("TextEditor", "", "c2"));
                            Dialog dialog("Replace * to +", layout, callback_replace_stars, window, editor);
                            dialog.generate();
                            dialog.show();
                            dialog.start_polling();
                        }
                    }
                }
            }

            if (event.type == sf::Event::TextEntered || event.type == sf::Event::KeyPressed) {              
                editor.handleInput(event);
                editor.update();
            }
            render(window, font, editor);
        }
    }

    return 0;
}
