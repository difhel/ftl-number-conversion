#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <editor.h>
#include <algo.h>
#include <string>


std::string callback_test_dialog(
    sf::RenderWindow &window,
    std::map<std::string, std::string> &input_data) {
    std::cout << "Callback function" << std::endl;
    for (auto c : input_data) {
        std::cout << "(callback) Input data: " << c.first << " " << c.second << std::endl;
    }
    return "NaN :)";
}

void callback_empty(
    sf::RenderWindow &window,
    std::map<std::string, std::string> input_data) {
    std::cout << "Callback function" << std::endl;
    window.close();
}
