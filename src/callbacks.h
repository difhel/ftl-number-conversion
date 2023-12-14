#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <editor.h>
#include <algo.h>
#include <string>
#include "big_frac.h"
#include "files.h"


std::string callback_test_dialog(
    sf::RenderWindow &window,
    std::map<std::string, std::string> &input_data) {
    std::cout << "Callback function" << std::endl;
    for (auto c : input_data) {
        std::cout << "(callback) Input data: " << c.first << " " << c.second << std::endl;
    }
    return "NaN :)";
}

std::string callback_from_inputs(
    sf::RenderWindow &window,
    std::map<std::string, std::string> &input_data,
    FileWorker &fileWorkerOutput) {
    std::string result = "Error";
    try {
        std::string p = input_data["p"];
        int q = std::stoi(input_data["q"]);
        // std::cout << input_data["n"] << "<" << std::endl;
        BigFrac n(input_data["n"] + "_" + p);
        n.translate(q);
        result = n.toString();
    }
    catch (const InvalidNumberException& e) {
        result = e.what();
    }
    catch (const std::invalid_argument& e) {
        result = e.what();
    }
    if (result.size() > 20) {
        fileWorkerOutput.write(result);
        result = "result was so long, so it was saved to file";
    }
    return result;
}

std::string callback_from_file(
    sf::RenderWindow &window,
    std::map<std::string, std::string> &input_data,
    FileWorker &fileWorkerInput,
    FileWorker &fileWorkerOutput) {
    std::string result = "Error";
    try {
        std::string p = input_data["p"];
        int q = std::stoi(input_data["q"]);
        std::string n_num = fileWorkerInput.read()[0];
        BigFrac n(n_num + "_" + p);
        n.translate(q);
        result = n.toString();
    }
    catch (const InvalidNumberException& e) {
        result = e.what();
    }
    catch (const std::invalid_argument& e) {
        result = e.what();
    }
    if (result.size() > 20) {
        fileWorkerOutput.write(result);
        result = "result was so long, so it was saved to file";
    }
    return result;
}

void callback_empty(
    sf::RenderWindow &window,
    std::map<std::string, std::string> input_data) {
    std::cout << "Callback function" << std::endl;
    // window.close();
}
