#include "BMPInput.h"
#include "Input.h"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <Eigen/Dense>

// Constructor
BMPInput::BMPInput(const std::string& filepath) : Input(filepath) {}

// Process the BMP file and return grayscale values as an Eigen matrix with image dimensions
Eigen::MatrixXd BMPInput::getData() {
    std::vector<unsigned char> pixelData;
    int width = 0, height = 0;

    readBMP(filepath, pixelData, width, height);

    return convertToGrayscale(pixelData, width, height);
}

// Convert pixel data to grayscale and return as a 2D Eigen matrix
Eigen::MatrixXd BMPInput::convertToGrayscale(const std::vector<unsigned char>& pixelData, int width, int height) const {
    Eigen::MatrixXd grayscale(height, width);

    int rowPadded = (width * 3 + 3) & (~3);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int pixelIdx = i * rowPadded + j * 3;

            unsigned char r = pixelData[pixelIdx];
            unsigned char g = pixelData[pixelIdx + 1];
            unsigned char b = pixelData[pixelIdx + 2];

            // Convert to grayscale using the standard luminance formula
            double gray = 0.299 * r + 0.587 * g + 0.114 * b;
            grayscale(i, j) = gray / 255.0; // Normalize to [0, 1]
        }
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

    // Read file and info headers
    file.read(reinterpret_cast<char*>(fileHeader), 14);
    file.read(reinterpret_cast<char*>(infoHeader), 40);

    if (fileHeader[0] != 'B' || fileHeader[1] != 'M') {
        throw std::runtime_error("Not a valid BMP file");
    }

    // Extract image dimensions
    width = *(reinterpret_cast<int*>(&infoHeader[4]));
    height = *(reinterpret_cast<int*>(&infoHeader[8]));

    int rowPadded = (width * 3 + 3) & (~3); // Each row is padded to a multiple of 4 bytes
    pixelData.resize(rowPadded * height);

    // Move file pointer to the start of pixel data
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
