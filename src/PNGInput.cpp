#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "PNGInput.h"
#include "Input.h"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <Eigen/Dense>
#include "ext_libraries/stb_image.h"

PNGInput::PNGInput(const std::string& filepath) : Input(filepath) {
    width=0;
    height=0;
    num_channels = 0;
}

void PNGInput::readData(){

    //Load Image Data
    unsigned char* Image = stbi_load(filepath.c_str(), &width, &height, &num_channels, 0);
    //Throw runtime_error if image did not load properly
    if (!Image) {
        throw std::runtime_error("Failed to load image: " + std::to_string(stbi_failure_reason()));
    }
    else{
        // Print image information
        std::cout << "Image loaded successfully!" << std::endl;
        std::cout << "Width: " << width << ", Height: " << height;
        std::cout << ", Channels: " << num_channels << std::endl;

        // Allocate memory for the data matrix
        ImageData.resize(height,width)

        // Store gray scale image in ImageData
        ImageData = convertToGrayscale(Image);
    }
    //Free Image memory after use
    stbi_image_free(Image_data);
}


Eigen::MatrixXd PNGInput::convertToGrayscale(unsigned char* Image) const {

    // Create Eigen matrix with correct size
    Eigen::MatrixXd grayscale(height, width);

    // Convert Image to normalized grayscale data
    if (num_channels == 1) { // Image has only one channel, data is only normalized
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                grayscale(y,x) = static_cast<double>(Image[y*width + x]) / 255.0;
            }
        }
    } else if (num_channels == 3) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int pixel_index = (y * width + x) * 3;
                unsigned char red = Image[pixel_index];
                unsigned char green = Image[pixel_index + 1];
                unsigned char blue = Image[pixel_index + 2];

                // Convert to grayscale using standard formula
                unsigned char gray = static_cast<unsigned char>(0.299 * red + 0.587 * green + 0.114 * blue)

                grayscale(y,x) = static_cast<double>(gray) / 255.0;
            }
        }
    } else {
        throw std::runtime_error("Failed to convert image, image must have 1 or 3 channels");
    }

    return grayscale;
}


