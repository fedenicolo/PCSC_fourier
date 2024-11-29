#include "Output.h"
#include <iostream>

Output::Output(const std::string& path) : outputFilePath(path) {}

std::string Output::getOutputFilePath() const {
    return outputFilePath;
}

void Output::setOutputFilePath(const std::string& path) {
    outputFilePath = path;
}
