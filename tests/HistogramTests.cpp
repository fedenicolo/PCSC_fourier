//
// Created by suttor on 12.12.24.
//
#include "gtest/gtest.h"
#include "PNGInput.h"
#include "Histogram.h"
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <Eigen/Dense>

TEST(HistogramTest, ValidateHistogramChess) {

    //Read CSV file with data of the same .png file read using python library
    std::ifstream file("Test_Files/chessboard_pixel_probabilities.csv");
    ASSERT_TRUE(file.is_open());

    std::string line;

    //Read true pixel data from the csv file
    std::getline(file, line); // Skip 'Header' line
    std::vector<float> pixel_values;
    std::vector<float> pixel_probabilities;

    while (std::getline(file, line)) {
        // Tokenize the line by comma
        char* tok = strtok(&line[0], ",");

        // Parse pixel value (first column)
        if (tok != NULL) {
            float pixel_value = atof(tok);
            pixel_values.push_back(pixel_value);
        }

        // Move to next token (probability)
        tok = strtok(NULL, ",");

        // Parse probability value (second column)
        if (tok != NULL) {
            float pixel_probability = atof(tok);
            pixel_probabilities.push_back(pixel_probability);
        }
    }

    //Convert CSV data into Eigen Matrix
    int col = 2;
    int row = pixel_values.size();

    Eigen::MatrixXd True_Histogram(row, col);

    // Populate the matrix with pixel values and probabilities
    for (int i = 0; i < row; ++i) {
        True_Histogram(i, 0) = pixel_values[i];        // Pixel value in the first column
        True_Histogram(i, 1) = pixel_probabilities[i]; // Probability in the second column
    }

    // Read the test image
    PNGInput ImageInput("Test_Files/chessboard.png");
    ImageInput.readData();
    Eigen::MatrixXd Pixels = ImageInput.getData();

    // Compute histogram data
    Histogram histogram = Histogram(Pixels, row);
    histogram.compute();
    Eigen::VectorXd pixel_p = histogram.getNormalizedHistogram();
    Eigen::MatrixXd histogram_data = histogram.getHistogramData();
    Eigen::VectorXd bincenters = histogram_data.col(0);
    Eigen::MatrixXd Histogram(pixel_p.size(), 2);
    Histogram.col(0) = bincenters;
    Histogram.col(1) = pixel_p;


    double residual = (Histogram - True_Histogram).norm();
    ASSERT_NEAR(residual, 0.0, 1e-6);
    ASSERT_NEAR(pixel_p.sum(), 1.0, 1e-6);
}

TEST(HistogramTest, ValidateHistogramGradient) {

    //Read CSV file with data of the same .png file read using python library
    std::ifstream file("Test_Files/gradient_pixel_probabilities.csv");
    ASSERT_TRUE(file.is_open());

    std::string line;

    //Read true pixel data from the csv file
    std::getline(file, line); // Skip 'Header' line
    std::vector<float> pixel_values;
    std::vector<float> pixel_probabilities;

    while (std::getline(file, line)) {
        // Tokenize the line by comma
        char* tok = strtok(&line[0], ",");

        // Parse pixel value (first column)
        if (tok != NULL) {
            float pixel_value = atof(tok);
            pixel_values.push_back(pixel_value);
        }

        // Move to next token (probability)
        tok = strtok(NULL, ",");

        // Parse probability value (second column)
        if (tok != NULL) {
            float pixel_probability = atof(tok);
            pixel_probabilities.push_back(pixel_probability);
        }
    }

    //Convert CSV data into Eigen Matrix
    int col = 2;
    int row = pixel_values.size();

    Eigen::MatrixXd True_Histogram(row, col);

    // Populate the matrix with pixel values and probabilities
    for (int i = 0; i < row; ++i) {
        True_Histogram(i, 0) = pixel_values[i];        // Pixel value in the first column
        True_Histogram(i, 1) = pixel_probabilities[i]; // Probability in the second column
    }

    // Read the test image
    PNGInput ImageInput("Test_Files/gradient.png");
    ImageInput.readData();
    Eigen::MatrixXd Pixels = ImageInput.getData();

    // Compute histogram data
    Histogram histogram = Histogram(Pixels, row);
    histogram.compute();
    Eigen::VectorXd pixel_p = histogram.getNormalizedHistogram();
    Eigen::MatrixXd histogram_data = histogram.getHistogramData();
    Eigen::VectorXd bincenters = histogram_data.col(0);
    Eigen::MatrixXd Histogram(pixel_p.size(), 2);
    Histogram.col(0) = bincenters;
    Histogram.col(1) = pixel_p;


    double residual = (Histogram - True_Histogram).norm();
    ASSERT_NEAR(residual, 0.05, 1e-3);
    ASSERT_NEAR(pixel_p.sum(), 1.0, 1e-6);
}