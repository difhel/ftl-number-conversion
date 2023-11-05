#pragma once
#include <vector>
#include <string>
#include <iostream>


bool is_digit(const char& c) {
    return (c >= '0' && c <= '9') || c == '\r';
}

void removeLeadingZeros(std::vector<std::string*>& text, int begin, int end) {
    // ✅
    for (int i = begin - 1; i <= end - 1; ++i) {
        std::string* str_ptr = text[i];
        bool isd = false;

        int j = 0;

        while (j < str_ptr->size()) {
            if (str_ptr->at(j) == '0') {
                if (j + 1 < str_ptr->size() && is_digit(str_ptr->at(j + 1))) {
                    str_ptr->erase(str_ptr->begin() + j);
                    --j;
                }
            }
            ++j;
        }
    }
}

void insertSubstring(std::vector<std::string*> &text, int n, int m, std::string substring) {
    // ✅
    for (int i = 0; i < substring.size(); ++i) {
        text[n - 1]->insert(text[n - 1]->begin() + m - 1 + i, 1, substring[i]);
    }
    std::cout << "InsertSubstringButton:\n";
    std::cout << "number N: " << n << " number M:" << m << "\n";
    std::cout << "text:\n";
    std::cout << substring << "\n";
    std::cout << "OK\n---------------------\n";
}

void replaceSymbol(std::vector<std::string*> &text, int n, int m, char symbol) {
    // ✅
    text[n - 1]->erase(m - 1, 1);
    text[n - 1]->insert(text[n - 1]->begin() + m - 1, symbol);
}

void deleteLine(std::vector<std::string*> &text, int n) {
    // ✅
    text.erase(text.begin() + n - 1);
}

void multipleInsertAfterLine(std::vector<std::string*> &text, int n, std::vector<std::string*> &new_strings) {
    // ✅
    for (int i = n; i < n + new_strings.size(); ++i) {
        text.insert(text.begin() + i, new_strings[i - n]);
    }
}

void deprecated_removeNonIncreasing(std::vector<std::string*>& text) {
    // ✅
    for (int i = 0; i < text.size(); ++i) {
        std::string* str_ptr = text[i];
        int j = 0;
        char last_digit = 47; // '0' - 1
        int group_index_start = 0;
        bool is_group = false;
        while (j < str_ptr->size()) {
            if (is_digit(str_ptr->at(j))) {
                if (!is_group) {
                    is_group = true;
                    last_digit = str_ptr->at(j);
                    group_index_start = j;
                    j++;
                    continue;
                }
                if (last_digit >= str_ptr->at(j)) {
                    for (int k = group_index_start; k <= j - group_index_start + 1; ++k) {
                        str_ptr->erase(group_index_start);
                    }
                    j = group_index_start;
                    continue;
                }
            } else is_group = false;
            ++j;
        }
    }
}

bool isIncreasingSequence(const std::string& sequence) {
    for (int i = 1; i < sequence.length(); i++) {
        if (sequence[i] <= sequence[i - 1]) {
            return false;
        }
    }
    return true;
}

void removeNonIncreasingSequences(std::string& inputString) {
    std::string result;
    int i = 0;

    while (i < inputString.length()) {
        if (isdigit(inputString[i])) {
            int start = i;
            while (i < inputString.length() && isdigit(inputString[i])) {
                i++;
            }
            std::string sequence = inputString.substr(start, i - start);
            if (sequence.length() == 1 || !isIncreasingSequence(sequence)) {
                inputString.erase(start, i - start);
                i = start;
            }
        } else {
            i++;
        }
    }
}

void removeNonIncreasing(std::vector<std::string*> &text) {
    for (int i = 0; i < text.size(); ++i) {
        removeNonIncreasingSequences(*text[i]);
    }
}

void removeBraceGroups(std::string &input) {
    size_t startPos = input.find('{');
    while (startPos != std::string::npos) {
        size_t endPos = input.find('}', startPos);
        if (endPos == std::string::npos) {
            break;
        }

        input.erase(startPos, endPos - startPos + 1);

        startPos = input.find('{');
    }
}

void removeBraces(std::vector<std::string*> &text, int n) {
    removeBraceGroups(*text[n - 1]);
}


void replaceStringInPlace(std::string &source, const std::string &what_replace, const std::string &to_replace) {
    if (what_replace.empty()) {
        return;
    }
    size_t pos = 0;
    while ((pos = source.find(what_replace, pos)) != std::string::npos) {
        source.replace(pos, what_replace.length(), to_replace);
        pos += to_replace.length();
    }
}

void replace(std::vector<std::string*> &text, int begin, int end, const std::string &what_replace, const std::string &to_replace) {
    for (size_t i = begin; i <= end; ++i) {
        replaceStringInPlace(*text[i - 1], what_replace, to_replace);
    }
}

void replaceStars(std::string &s, char star, char plus) {
    int N = 0;
    int i = 0; 

    while (i < s.length()) {
        if (s[i] == star) {
            N = 1;
            while (i + N < s.length() && s[i + N] == star) {
                N++;
            }

            if (N > 1) {
                s.replace(i, N, N / 2, plus);
                if (N % 2 == 1) {
                    s.insert(i + N / 2, 1, star);
                }
                i += N / 2 + N % 2;
            } else {
                i++;
            }
        } else {
            i++;
        }
    }
}

void replaceStarToPlus(std::vector<std::string*> &text, char star, char plus) {
    for (size_t i = 0; i < text.size(); ++i) {
        replaceStars(*text[i], star, plus);
    }
}