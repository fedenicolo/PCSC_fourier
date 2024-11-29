//
// Created by nicol on 29/11/2024.
//
#include "PNGOutput.h"
#include <stdexcept>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "ext_libraries/stb_image_write.h"

PNGOutput::PNGOutput(const std::string& path) : Output(path) {}

void PNGOutput::save(const Eigen::MatrixXd& data) {
    int height = static_cast<int>(data.rows());
    int width = static_cast<int>(data.cols());

    if (height <= 0 || width <= 0) {
        throw std::invalid_argument("Invalid image dimensions.");
    }

    // Prepare image data
    std::vector<unsigned char> imageData(height * width);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            imageData[i * width + j] = static_cast<unsigned char>(data(i, j));
        }
    }

    // Save as PNG
    if (!stbi_write_png(outputFilePath.c_str(), width, height, 1, imageData.data(), width)) {
        throw std::runtime_error("Failed to save PNG file.");
    }
}
