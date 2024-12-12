//
// Created by suttor on 10.12.24.
//
#include "gtest/gtest.h"
#include "PNGInput.h"
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <Eigen/Dense>

TEST(PNGInputTest, ValidatePNGReadChess) {

    //Read CSV file with data of the same .png file read using python library
    std::ifstream file("Test_Files/chessboard_pixel_values.csv");
    ASSERT_TRUE(file.is_open());

    std::string line;

    //Read true pixel data from the csv file
    std::getline(file, line); // Skip 'Chessboard Pixel Values (Black and White)' line
    std::vector<float> csv_data;

    while (std::getline(file, line)) {
        char* tok = NULL;
        tok = strtok(&line[0], ",");
        float sample = 0.0f;
        while (tok != NULL) {
            csv_data.push_back(atof(tok));
            tok = strtok(NULL, ",");
        }
    }

    //Convert CSV data into Eigen Matrix
    int width = 100;
    int height = 100;

    Eigen::MatrixXd True_Pixel(height,width);
    for (int i = 0; i < True_Pixel.rows(); ++i) {
        for (int j = 0; j < True_Pixel.cols(); ++j) {
            True_Pixel(i, j) = csv_data[i * True_Pixel.cols() + j];
        }
    }

    // Compare normalized data stored in PNGInput with data from CSV
    PNGInput ImageInput("Test_Files/chessboard.png");
    ImageInput.readData();
    Eigen::MatrixXd Pixels = ImageInput.getData();
    ASSERT_EQ(Pixels.rows(), True_Pixel.rows());
    ASSERT_EQ(Pixels.cols(), True_Pixel.cols());

    double residual = (Pixels - True_Pixel).norm();
    ASSERT_NEAR(residual, 0.0, 1e-6);
}

TEST(PNGInputTest, ValidatePNGReadGradient) {

    //Read CSV file with data of the same .png file read using python library
    std::ifstream file("Test_Files/gradient_pixel_values.csv");
    ASSERT_TRUE(file.is_open());

    std::string line;

    //Read true pixel data from the csv file
    std::getline(file, line); // Skip 'Gradient Image Pixel Values (Grayscale)' line
    std::vector<double> csv_data;
    
    while (std::getline(file, line)) {
        char* tok = NULL;
        tok = strtok(&line[0], ",");
        double sample = 0.0f;
        while (tok != NULL) {
            csv_data.push_back(std::stod(tok));
            tok = strtok(NULL, ",");
        }
    } 

    //Convert CSV data into Eigen Matrix
    int width = 100;
    int height = 100;

    Eigen::MatrixXd True_Pixel2(height,width);
    for (int i = 0; i < True_Pixel2.rows(); ++i) {
        for (int j = 0; j < True_Pixel2.cols(); ++j) {
            True_Pixel2(i, j) = csv_data[i * True_Pixel2.cols() + j];
        }
    }

    // Compare normalized data stored in PNGInput with data from CSV
    PNGInput ImageInput("Test_Files/gradient.png");
    ImageInput.readData();
    Eigen::MatrixXd Pixels = ImageInput.getData();

    ASSERT_EQ(Pixels.rows(), True_Pixel2.rows());
    ASSERT_EQ(Pixels.cols(), True_Pixel2.cols());

    double residual = (Pixels - True_Pixel2).norm();
    ASSERT_NEAR(residual, 0.0, 6e-2);
}