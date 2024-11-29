//
// Created by nicol on 24/11/2024.
//
#include "Input.h"
#include<filesystem>

Input::Input(const std::string& path) : filepath(path) {}

bool Input::isValidFile() const {
    return std::filesystem::exists(filepath);
}
std::string Input::getFilepath() const {
    return filepath;
}


