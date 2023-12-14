#pragma once
#include <string>
#include <fstream>
#include <vector>

class FileWorker {
public:
    std::string pathToFile;
    FileWorker(std::string pathToFile) : pathToFile(pathToFile) {}

    // function to read file
    std::vector<std::string> read() {
        std::ifstream inputFile(pathToFile);
        std::vector<std::string> fileContent;

        if (inputFile.is_open()) {
            std::string line;
            while (std::getline(inputFile, line)) {
                fileContent.push_back(line);
            }
            inputFile.close();
        }

        return fileContent;
    }

    void write(std::string fileContent) {
        std::ofstream outputFile(pathToFile);
        if (outputFile.is_open()) {
            outputFile << fileContent;
            outputFile.close();
        }
    }
};
