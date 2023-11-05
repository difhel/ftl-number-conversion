#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <editor.h>
#include <algo.h>
#include <string>


void callback_test_dialog(
    sf::RenderWindow &window,
    std::map<std::string, std::string> input_data,
    TextEditor &editor) {
    std::cout << "Callback function" << std::endl;
    for (auto c : input_data) {
        std::cout << "(callback) Input data: " << c.first << " " << c.second << std::endl;
    }
    // window.close();
}

void callback_empty(
    sf::RenderWindow &window,
    std::map<std::string, std::string> input_data,
    TextEditor &editor) {
    return;
}

void callback_insert_substr_in_line(
    sf::RenderWindow &window,
    std::map<std::string, std::string> input_data,
    TextEditor &editor) {
    int n = std::stoi(input_data["n"]);
    int m = std::stoi(input_data["m"]);
    std::string substr = input_data["substr"];
    insertSubstring(editor.lines, n, m, substr);
}

void callback_remove_leading_zeros(
    sf::RenderWindow &window,
    std::map<std::string, std::string> input_data,
    TextEditor &editor) {
    int n = std::stoi(input_data["n"]);
    int m = std::stoi(input_data["m"]);
    removeLeadingZeros(editor.lines, n, m);
}

void callback_delete_line(
    sf::RenderWindow &window,
    std::map<std::string, std::string> input_data,
    TextEditor &editor) {
    int n = std::stoi(input_data["n"]);
    deleteLine(editor.lines, n);
}

void callback_replace_symbol(
    sf::RenderWindow &window,
    std::map<std::string, std::string> input_data,
    TextEditor &editor) {
    int n = std::stoi(input_data["n"]);
    int m = std::stoi(input_data["m"]);
    char c = input_data["c"][0];
    replaceSymbol(editor.lines, n, m, c);
}

void callback_multiple_insert_after_line(
    sf::RenderWindow &window,
    std::map<std::string, std::string> input_data,
    TextEditor &editor) {
    int n = std::stoi(input_data["n"]);
    std::string l = input_data["lines"];
    std::vector<std::string*> lines_to_insert;
    lines_to_insert.push_back(new std::string(""));
    for (char c : l) {
        if (c == '\n') {
            lines_to_insert.push_back(new std::string(""));
            continue;
        }
        *(lines_to_insert.back()) += c;
    }
    char c = input_data["c"][0];
    multipleInsertAfterLine(editor.lines, n, lines_to_insert);
}

void callback_remove_non_increasing(
    sf::RenderWindow &window,
    std::map<std::string, std::string> input_data,
    TextEditor &editor) {
    removeNonIncreasing(editor.lines);
}

void callback_remove_braces(
    sf::RenderWindow &window,
    std::map<std::string, std::string> input_data,
    TextEditor &editor) {
    int n = std::stoi(input_data["n"]);
    removeBraces(editor.lines, n);
}

void callback_replace(
    sf::RenderWindow &window,
    std::map<std::string, std::string> input_data,
    TextEditor &editor) {
    int n = input_data["n"].empty() ? 1 : std::stoi(input_data["n"]);
    int m = input_data["m"].empty() ? editor.lines.size() : std::stoi(input_data["m"]);
    std::string what_replace = input_data["what"];
    std::string to_replace = input_data["to"];
    replace(editor.lines, n, m, what_replace, to_replace);
}

void callback_replace_stars(
    sf::RenderWindow &window,
    std::map<std::string, std::string> input_data,
    TextEditor &editor) {
    char c1 = input_data["c1"][0];
    char c2 = input_data["c2"][0];
    replaceStarToPlus(editor.lines, c1, c2);
}
