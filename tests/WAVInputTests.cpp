//
// Created by suttor on 10.12.24.
//
#include "gtest/gtest.h"
#include "WAVInput.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <Eigen/Dense>

TEST(WAVInputTest, ValidateWAVReadPCM8bit) {

    //Read CSV file with data of the same .wav file read using python library
    std::ifstream file("Test_Files/Sine8bit.csv");
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
    int bits_per_sample = std::stoi(properties[2]);

    //Check properties against WAVInput class
    WAVInput AudioInput("Test_Files/Sine8bit.wav");
    AudioInput.readData();
    EXPECT_EQ(AudioInput.getSampleRate(), sampling_rate);
    EXPECT_EQ(AudioInput.getNumChannels(), num_channels);
    EXPECT_EQ(AudioInput.getBitsPerSample(), bits_per_sample);

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



    while (std::getline(file, line)) {
        std::istringstream data_stream(line);
        float sample;
        while (data_stream >> sample) {
            csv_data.push_back(sample);
        }
    }

    //Convert CSV data into Eigen Matrix
    Eigen::MatrixXd True_Audio(num_channels, (csv_data.size() / num_channels));
    for (int i = 0; i < num_channels; ++i) {
        for (int j = 0; j < True_Audio.cols(); ++j) {
            True_Audio(i, j) = csv_data[j * num_channels + i];
        }
    }

    // Compare normalized data stored in WAVInput with data from CSV
    Eigen::MatrixXd Audio = AudioInput.getData();
    ASSERT_EQ(Audio.rows(), True_Audio.rows());
    ASSERT_EQ(Audio.cols(), True_Audio.cols());

    double residual = (Audio - True_Audio).norm();
    ASSERT_LT(residual, 1e-16);
}

TEST(WAVInputTest, ValidateWAVReadPCM16bit) {

    //Read CSV file with data of the same .wav file read using python library
    std::ifstream file("Test_Files/Sine16bit.csv");
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
    int bits_per_sample = std::stoi(properties[2]);

    //Check properties against WAVInput class
    WAVInput AudioInput("Test_Files/Sine16bit.wav");
    AudioInput.readData();
    EXPECT_EQ(AudioInput.getSampleRate(), sampling_rate);
    EXPECT_EQ(AudioInput.getNumChannels(), num_channels);
    EXPECT_EQ(AudioInput.getBitsPerSample(), bits_per_sample);

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

    // Compare normalized data stored in WAVInput with data from CSV
    Eigen::MatrixXd Audio = AudioInput.getData();
    ASSERT_EQ(Audio.rows(), True_Audio.rows());
    ASSERT_EQ(Audio.cols(), True_Audio.cols());

    double residual = (Audio - True_Audio).norm();
    ASSERT_LT(residual, 1e-16);
}

TEST(WAVInputTest, ValidateWAVReadPCM24bit) {

    //Read CSV file with data of the same .wav file read using python library
    std::ifstream file("Test_Files/Sine24bit.csv");
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
    int bits_per_sample = std::stoi(properties[2]);

    //Check properties against WAVInput class
    WAVInput AudioInput("Test_Files/Sine24bit.wav");
    AudioInput.readData();
    EXPECT_EQ(AudioInput.getSampleRate(), sampling_rate);
    EXPECT_EQ(AudioInput.getNumChannels(), num_channels);
    EXPECT_EQ(AudioInput.getBitsPerSample(), bits_per_sample);

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

    // Compare normalized data stored in WAVInput with data from CSV
    Eigen::MatrixXd Audio = AudioInput.getData();
    ASSERT_EQ(Audio.rows(), True_Audio.rows());
    ASSERT_EQ(Audio.cols(), True_Audio.cols());

    double residual = (Audio - True_Audio).norm();
    ASSERT_LT(residual, 1e-16);
}

TEST(WAVInputTest, ValidateWAVReadPCM32bit) {

    //Read CSV file with data of the same .wav file read using python library
    std::ifstream file("Test_Files/Sine32bit.csv");
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
    int bits_per_sample = std::stoi(properties[2]);

    //Check properties against WAVInput class
    WAVInput AudioInput("Test_Files/Sine32bit.wav");
    AudioInput.readData();
    EXPECT_EQ(AudioInput.getSampleRate(), sampling_rate);
    EXPECT_EQ(AudioInput.getNumChannels(), num_channels);
    EXPECT_EQ(AudioInput.getBitsPerSample(), bits_per_sample);

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

    // Compare normalized data stored in WAVInput with data from CSV
    Eigen::MatrixXd Audio = AudioInput.getData();
    ASSERT_EQ(Audio.rows(), True_Audio.rows());
    ASSERT_EQ(Audio.cols(), True_Audio.cols());

    double residual = (Audio - True_Audio).norm();
    ASSERT_LT(residual, 1e-16);
}

TEST(WAVInputTest, ValidateWAVReadPCM32bitStereo) {

    //Read CSV file with data of the same .wav file read using python library
    std::ifstream file("Test_Files/Sine32bitStereo.csv");
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
    int bits_per_sample = std::stoi(properties[2]);

    //Check properties against WAVInput class
    WAVInput AudioInput("Test_Files/Sine32bitStereo.wav");
    AudioInput.readData();
    EXPECT_EQ(AudioInput.getSampleRate(), sampling_rate);
    EXPECT_EQ(AudioInput.getNumChannels(), num_channels);
    EXPECT_EQ(AudioInput.getBitsPerSample(), bits_per_sample);

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

    // Compare normalized data stored in WAVInput with data from CSV
    Eigen::MatrixXd Audio = AudioInput.getData();
    ASSERT_EQ(Audio.rows(), True_Audio.rows());
    ASSERT_EQ(Audio.cols(), True_Audio.cols());

    double residual = (Audio - True_Audio).norm();
    ASSERT_LT(residual, 1e-16);
}

TEST(WAVInputTest, ValidateWAVFormatError) {
    EXPECT_THROW({
        try {
            WAVInput AudioInput("Test_Files/SineWrongFormat.wav");
            AudioInput.readData();
        } catch (const std::exception& e) {
            EXPECT_STREQ(e.what(), "Unsupported WAV file: Compressed Audio is not supported.");
            throw;
        }
    }, std::exception);
}

