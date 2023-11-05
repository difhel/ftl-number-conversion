#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <config.h>
#include <map>
#include <button.h>
#include <editor.h>
#include <algo.h>


struct DialogElement {
    DialogElement(std::string type, std::string text, std::string id) : type{type}, text{text}, id{id} {};
    std::string type; // TextEditor, Button or Label (button without background)
    std::string text;
    std::string id;
};

class Layout {
    public:
        Layout() = default;
        void addElement(const DialogElement &element) {
            std::cout << "DialogElement::addElement <" << element.type << ">" << std::endl;
            this->views.push_back(element);
        }
        std::vector<DialogElement>& build() {
            return this->views;
        }
    private:
        std::vector<DialogElement> views;
};
