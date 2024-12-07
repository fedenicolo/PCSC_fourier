#include "BMPInput.h"
#include "Image.h"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <Eigen/Dense>

BMPInput::BMPInput(const std::string& filepath) : Image(filepath) {
    height = 0;
    width = 0;
}

// Convert pixel data to grayscale and return as a 2D Eigen matrix
Eigen::MatrixXd BMPInput::convertToGrayscale(const std::vector<unsigned char>& pixelData) const{

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
void BMPInput::readData(){

    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Unable to open BMP file: " + filepath);
    }

    unsigned char fileHeader[14];
    unsigned char infoHeader[40];

    // Read file and info headers, here doind the reinterpet cast is necessary cuz we need to convert from int to char*
    file.read(reinterpret_cast<char*>(fileHeader), 14);
    file.read(reinterpret_cast<char*>(infoHeader), 40);

    if (fileHeader[0] != 'B' || fileHeader[1] != 'M') {
        throw std::runtime_error("Not a valid BMP file");
    }

    // Extract image dimensions
    width = *(reinterpret_cast<int*>(&infoHeader[4]));
    height = *(reinterpret_cast<int*>(&infoHeader[8]));

    // Create buffer to store file data
    int rowPadded = (width * 3 + 3) & (~3); // Each row is padded to a multiple of 4 bytes
    std::vector<unsigned char> pixelData;
    pixelData.resize(rowPadded * height);

    // Move file pointer to the start of pixel data
    int dataOffset = *(reinterpret_cast<int*>(&fileHeader[10]));
    file.seekg(dataOffset, std::ios::beg);

    // Read pixel data
    for (int i = 0; i < height; ++i) {
        file.read(reinterpret_cast<char*>(&pixelData[i * rowPadded]), rowPadded);
    }

    std::vector<unsigned char> flippedData;
    flippedData.resize(pixelData.size());
    int rowSize = width * 3; // Assuming 3 channels (RGB)
    for (int i = 0; i < height; ++i) {
        int srcRowIdx = (height - i - 1) * rowPadded;  // Flip the rows
        int dstRowIdx = i * rowPadded;
        std::copy(&pixelData[srcRowIdx], &pixelData[srcRowIdx + rowPadded], &flippedData[dstRowIdx]);
    }
    pixelData = flippedData;

    if (!file) {
        throw std::runtime_error("Error reading BMP pixel data.");
    }

    file.close();

    // Convert to grayscale and save in ImageData Eigen Matrix
    ImageData = convertToGrayscale(pixelData);
}
