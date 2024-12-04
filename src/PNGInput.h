//
// Created by nicol on 24/11/2024.
//
#ifndef PNGINPUT_H
#define PNGINPUT_H

#include "Input.h"
#include <Eigen/Dense>
#include <vector>
#include <string>

class PNGInput : public Input {
public:
    explicit PNGInput(const std::string& filepath);
    void readData() override;
    int getWidth() const {return width;}
    int getHeight() const {return height;}
    int getChannels() const {return channels;}
    Eigen::MatrixXd getData() override { return AudioData;}

private:
    int width;
    int height;
    int num_channels;
    Eigen::MatrixXd ImageData;
    Eigen::MatrixXd convertToGrayscale(unsigned char* Image) const;
};

#endif // PNGINPUT_H
