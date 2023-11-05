#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include <map>

namespace config {
    const double EPS = 0.00001;

    namespace window {
        const int size = 500;
        const std::string title = "FTL fourth prac";
        const int button_margin = 20;
        const int button_width = 330;
        std::map<std::string, std::string> buttons_config = {
            // {"test_dialog", "A Test dialog"},
            {"insert_after_line", "Insert after line"},
            {"multiple_insert_after_line", "Multiple insert after line"},
            {"delete_line", "Delete line"},
            {"insert_substr_in_line", "Insert substring in line"},
            {"replace_symbol", "Replace symbol"},
            {"replace", "Replace"},
            {"remove_zeros", "Remove leading zeros"},
            {"remove_numbers", "Remove non-increasing numbers"},
            {"replace_*_+", "Replace group of * with +"},
            {"remove_{}", "Remove everything in curly braces"}
        };
    } // namespace window

    namespace color {
        static const sf::Color background{28, 27, 31};
        static const sf::Color on_background{230, 255, 229};
        static const sf::Color primary{208, 188, 255};
        static const sf::Color on_primary{56, 30, 114};
        static const sf::Color secondary_container{74, 68, 88};
        // static const sf::Color on_secondary_container{232, 222, 248};
        static const sf::Color on_secondary_container{255, 255, 255};

        // static const sf::Color background_color{white};
        // static const sf::Color axis_color{100, 100, 100};
        // static const sf::Color grid_color{dark_grey};
        // static const sf::Color figure_color{black};
        // static const sf::Color fill_color{dark_green};
    } // namespace color

    namespace control {
        static const auto move_up =  sf::Keyboard::Up;
        static const auto move_down =  sf::Keyboard::Down;
        static const auto move_left =  sf::Keyboard::Left;
        static const auto move_right =  sf::Keyboard::Right;
        static const auto zoom_in =  sf::Keyboard::Hyphen;
        static const auto zoom_out =  sf::Keyboard::Equal;
        static const auto fill =  sf::Keyboard::F;
    }
} // namespace config
