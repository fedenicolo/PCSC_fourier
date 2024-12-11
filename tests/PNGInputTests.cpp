//
// Created by suttor on 10.12.24.
//
#include "gtest/gtest.h"
#include "PNGInput.h"
#include <iostream>
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
        std::istringstream data_stream(line);
        float sample;
        while (data_stream >> sample) {
            csv_data.push_back(sample);
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
    PNGInput ImageInput("Test_Files/chessboard.png")
    Eigen::MatrixXd Pixels = ImageInput.getData();
    ASSERT_EQ(Pixels.rows(), True_Pixel.rows());
    ASSERT_EQ(Pixels.cols(), True_Pixel.cols());

    for (int i = 0; i < Pixels.rows(); ++i) {
        for (int j = 0; j < Pixels.cols(); ++j) {
            EXPECT_NEAR(Pixels(i, j), True_Pixel(i, j), 1e-6);
        }
    }
}

TEST(PNGInputTest, ValidatePNGReadGradient) {

    //Read CSV file with data of the same .png file read using python library
    std::ifstream file("Test_Files/gradient_pixel_values.csv");
    ASSERT_TRUE(file.is_open());

    std::string line;

    //Read true pixel data from the csv file
    std::getline(file, line); // Skip 'Gradient Image Pixel Values (Grayscale)' line
    std::vector<float> csv_data;
    while (std::getline(file, line)) {
        std::istringstream data_stream(line);
        float sample;
        while (data_stream >> sample) {
            csv_data.push_back(sample);
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
    PNGInput ImageInput("Test_Files/gradient.png")
    Eigen::MatrixXd Pixels = ImageInput.getData();
    ASSERT_EQ(Pixels.rows(), True_Pixel.rows());
    ASSERT_EQ(Pixels.cols(), True_Pixel.cols());

    for (int i = 0; i < Pixels.rows(); ++i) {
        for (int j = 0; j < Pixels.cols(); ++j) {
            EXPECT_NEAR(Pixels(i, j), True_Pixel(i, j), 1e-6);
        }
    }
}