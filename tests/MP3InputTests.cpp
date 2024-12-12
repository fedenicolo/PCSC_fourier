//
// Created by suttor on 10.12.24.
//
#include "gtest/gtest.h"
#include "MP3Input.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <Eigen/Dense>

TEST(MP3InputTest, ValidateMP3ReadSineMono) {

    //Read CSV file with data of the same .mp3 file read using python library
    std::ifstream file("Test_Files/SineMono.csv");
    ASSERT_TRUE(file.is_open());

    std::string line;
    std::getline(file, line);

    //Read properties of the CSV file
    std::getline(file, line);
    std::istringstream iss(line);
    std::string value;
    std::vector<std::string> properties;
    while (std::getline(iss, value, ',')) {
        properties.push_back(value);
    }

    int sampling_rate = std::stoi(properties[0]);
    int num_channels = std::stoi(properties[1]);

    //Check properties against MP3Input class
    MP3Input AudioInput("Test_Files/SineMono.mp3");
    AudioInput.readData();
    EXPECT_EQ(AudioInput.getSampleRate(), sampling_rate);
    EXPECT_EQ(AudioInput.getNumChannels(), num_channels);

    //Read true audio data from the csv file
    std::getline(file, line); // Skip 'Normalized Data' line
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
    Eigen::MatrixXd True_Audio(num_channels, csv_data.size() / num_channels);
    for (int i = 0; i < num_channels; ++i) {
        for (int j = 0; j < True_Audio.cols(); ++j) {
            True_Audio(i, j) = csv_data[j * num_channels + i];
        }
    }

    // Compare normalized data stored in MP3Input with data from CSV
    Eigen::MatrixXd Audio = AudioInput.getData();
    ASSERT_EQ(Audio.rows(), True_Audio.rows());
    ASSERT_EQ(Audio.cols(), True_Audio.cols());

    double residual = (Audio - True_Audio).norm();
    ASSERT_LT(residual, 1e-15);
}

TEST(MP3InputTest, ValidateMP3ReadSineStereo) {

    //Read CSV file with data of the same .mp3 file read using python library
    std::ifstream file("Test_Files/SineStereo.csv");
    ASSERT_TRUE(file.is_open());

    std::string line;
    std::getline(file, line);

    //Read properties of the CSV file
    std::getline(file, line);
    std::istringstream iss(line);
    std::string value;
    std::vector<std::string> properties;
    while (std::getline(iss, value, ',')) {
        properties.push_back(value);
    }

    int sampling_rate = std::stoi(properties[0]);
    int num_channels = std::stoi(properties[1]);

    //Check properties against MP3Input class
    MP3Input AudioInput("Test_Files/SineStereo.mp3");
    AudioInput.readData();
    EXPECT_EQ(AudioInput.getSampleRate(), sampling_rate);
    EXPECT_EQ(AudioInput.getNumChannels(), num_channels);

    //Read true audio data from the csv file
    std::getline(file, line); // Skip 'Normalized Data' line
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
    Eigen::MatrixXd True_Audio(num_channels, csv_data.size() / num_channels);
    for (int i = 0; i < num_channels; ++i) {
        for (int j = 0; j < True_Audio.cols(); ++j) {
            True_Audio(i, j) = csv_data[j * num_channels + i];
        }
    }

    // Compare normalized data stored in MP3Input with data from CSV
    Eigen::MatrixXd Audio = AudioInput.getData();
    ASSERT_EQ(Audio.rows(), True_Audio.rows());
    ASSERT_EQ(Audio.cols(), True_Audio.cols());

    double residual = (Audio - True_Audio).norm();
    ASSERT_LT(residual, 1e-15);
}
