//
// Created by nicol on 24/11/2024.
//
#ifndef BMPINPUT_H
#define BMPINPUT_H

#include "Image.h"
#include <Eigen/Dense>
#include <vector>
#include <string>

class BMPInput : public Image {
public:
    explicit BMPInput(const std::string& filepath);

    Eigen::MatrixXd getData() override;

private:
    void readBMP(const std::string& filepath, std::vector<unsigned char>& pixelData, int& width, int& height) const;
    
    Eigen::MatrixXd convertToGrayscale(const std::vector<unsigned char>& pixelData, int width, int height) const;
};

#endif // BMPINPUT_H
