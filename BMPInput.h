//
// Created by nicol on 24/11/2024.
//
#ifndef BMP_INPUT_H
#define BMP_INPUT_H

#include "Input.h"
#include <vector>
#include <string>

class BMPInput : public Input {
public:
    // Constructor
    explicit BMPInput(const std::string& filepath);

    // Override the method to process BMP and return a vector
    std::vector<double> process() override;

private:
    std::vector<double> convertToGrayscale(const std::vector<unsigned char>& pixelData, int width, int height) const;
    void readBMP(const std::string& filepath, std::vector<unsigned char>& pixelData, int& width, int& height) const;
};

#endif // BMP_INPUT_H
