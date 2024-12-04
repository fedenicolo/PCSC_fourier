//
// Created by nicol on 24/11/2024.
//
#ifndef PNGINPUT_H
#define PNGINPUT_H

#include "Image.h"
#include <Eigen/Dense>
#include <vector>
#include <string>

class PNGInput : public Image {
public:
    explicit PNGInput(const std::string& filepath);
    void readData() override;
    int getWidth() const {return width;}
    int getHeight() const {return height;}
    int getChannels() const {return num_channels;}
    Eigen::MatrixXd getData() override { return ImageData;}

private:
    int width;
    int height;
    int num_channels;
    Eigen::MatrixXd ImageData;
    Eigen::MatrixXd convertToGrayscale(unsigned char* Image) const;
};

#endif // PNGINPUT_H
