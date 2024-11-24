//
// Created by nicol on 24/11/2024.
//
#include "BMPInput.h"
#include <fstream>
#include <stdexcept>
#include <vector>
#include <iostream>

// Constructor
BMPInput::BMPInput(const std::string& filepath) : Input(filepath) {}

// Process the BMP file and return grayscale values as a vector
std::vector<double> BMPInput::process() {
    std::vector<unsigned char> pixelData;
    int width = 0, height = 0;

    readBMP(filepath, pixelData, width, height);

    return convertToGrayscale(pixelData, width, height);
}

// Convert pixel data to grayscale
std::vector<double> BMPInput::convertToGrayscale(const std::vector<unsigned char>& pixelData, int width, int height) const {
    std::vector<double> grayscale;
    grayscale.reserve(width * height);

    for (size_t i = 0; i < pixelData.size(); i += 3) { // Assume RGB format
        unsigned char r = pixelData[i];
        unsigned char g = pixelData[i + 1];
        unsigned char b = pixelData[i + 2];

        // Convert to grayscale using standard luminance formula
        double gray = 0.299 * r + 0.587 * g + 0.114 * b;
        grayscale.push_back(gray / 255.0); // Normalize to [0, 1]
    }

    return grayscale;
}

// Read BMP file
void BMPInput::readBMP(const std::string& filepath, std::vector<unsigned char>& pixelData, int& width, int& height) const {
    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Unable to open BMP file: " + filepath);
    }


    unsigned char fileHeader[14];
    unsigned char infoHeader[40];

    // Read file and info headers.
    // We need to use reinterpret_cast because file.read work with char* but fileheader is declared as
    //unsigned char, it is important to user reinterpret and not a normal cast because we need to keep
    //memory region
    file.read(reinterpret_cast<char*>(fileHeader), 14);
    file.read(reinterpret_cast<char*>(infoHeader), 40);

    if (fileHeader[0] != 'B' || fileHeader[1] != 'M') {
        throw std::runtime_error("Not a valid BMP file");
    }

    // Extract image dimensions, we could have used a memcpy but this is fine too and dont use other memory
    width = *(reinterpret_cast<int*>(&infoHeader[4]));
    height = *(reinterpret_cast<int*>(&infoHeader[8]));

    int rowPadded = (width * 3 + 3) & (~3); // Each row is padded to a multiple of 4 bytes
    pixelData.resize(rowPadded * height);

    // Move file pointer to the start of pixel data, offset
    int dataOffset = *(reinterpret_cast<int*>(&fileHeader[10]));
    file.seekg(dataOffset, std::ios::beg);

    // Read pixel data
    for (int i = 0; i < height; ++i) {
        file.read(reinterpret_cast<char*>(&pixelData[i * rowPadded]), rowPadded);
    }

    if (!file) {
        throw std::runtime_error("Error reading BMP pixel data.");
    }

    file.close();
}
