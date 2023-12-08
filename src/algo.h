#pragma once
#include <vector>
#include <string>
#include <iostream>


// todo: this is file for the backend

// exception for invalid data with message "invalid number provided"
class InvalidNumberException : public std::exception {
    const char* what() const throw() {
        return "invalid number provided";
    }
};

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool isNumber(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), isDigit);
}

// char to int
int ctoi(char c) {
    if (isDigit(c)) return c - '0';
    // if c in A...Z return code (A=10, B=11, etc) else raise exception
    if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
    throw InvalidNumberException();
}

std::vector<int> stringToVectorInt(std::string str, int radix=10) {
    // parses std::string to std::vector<int> of digits
    std::vector<int> vec;
    size_t i = 0;
    bool isBlockOpen = false;
    while (i < str.size()) {
        if (str[i] == '[') {
            if (isBlockOpen) throw InvalidNumberException();
            isBlockOpen = true;
            i++;
            std::string block;
            while (i < str.size() && str[i] != ']') {
                block += str[i];
                i++;
            }
            if (i == str.size()) throw InvalidNumberException(); // мы дошли до конца строки, но закрывающей скобки в блоке не было
            if (str[i] == ']') {
                isBlockOpen = false;
                i++;
            }
            vec.push_back(stoi(block));
            if (*(vec.end() - 1) >= radix) throw InvalidNumberException();
            continue;
        }
        vec.push_back(ctoi(str[i]));
        if (*(vec.end() - 1) >= radix) throw InvalidNumberException();
        i++;
    }
    return vec;
}