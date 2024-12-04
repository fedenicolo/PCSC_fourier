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
    void readData() override;
    Eigen::MatrixXd getData() override { return ImageData;}

private:
    int width;
    int height;
    Eigen::MatrixXd ImageData;
    Eigen::MatrixXd convertToGrayscale(const std::vector<unsigned char>& pixelData) const;
};

#endif // BMPINPUT_H
