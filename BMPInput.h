//
// Created by nicol on 24/11/2024.
//
#ifndef BMPINPUT_H
#define BMPINPUT_H

#include "Input.h"
#include <Eigen/Dense>
#include <vector>
#include <string>

class BMPInput : public Input {
public:
    explicit BMPInput(const std::string& filepath);

    // Override the base class method to return the processed data as an Eigen matrix
    Eigen::MatrixXd getData() override;

private:
    // Reads BMP file and extracts pixel data
    void readBMP(const std::string& filepath, std::vector<unsigned char>& pixelData, int& width, int& height) const;

    // Converts pixel data to grayscale values in a 2D Eigen matrix
    Eigen::MatrixXd convertToGrayscale(const std::vector<unsigned char>& pixelData, int width, int height) const;
};

#endif // BMPINPUT_H
