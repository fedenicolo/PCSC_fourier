//
// Created by suttor on 10.12.24.
//
#include "gtest/gtest.h"
#include "WAVInput.h"
#include "Fourier.h"
#include "SignalProcessor.h"
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "Eigen/Dense"

TEST(FilterTest, ValidateLowPassFilter) {

    //Read CSV file with data of the low frequency (50 hz) sine signal
    std::ifstream file("Test_Files/LowFreq.csv");
    ASSERT_TRUE(file.is_open());

    //Read true audio data from the csv file
    std::string line;
    std::getline(file, line);
    std::vector<double> csv_data;
        while (std::getline(file, line)) {
        char* tok = NULL;
        tok = strtok(&line[0], ",");
        while (tok != NULL) {
            csv_data.push_back(atof(tok));
            tok = strtok(NULL, ",");
        }
    }

    //Convert CSV data into Eigen Matrix
    int num_channels = 1;
    Eigen::MatrixXd True_Audio_LowFreq(num_channels, (csv_data.size() / num_channels));
    for (int i = 0; i < num_channels; ++i) {
        for (int j = 0; j < True_Audio_LowFreq.cols(); ++j) {
            True_Audio_LowFreq(i, j) = csv_data[j * num_channels + i];
        }
    }

    // Extract audio from the audio file with both a low (50 Hz) and high (5000 Hz) frequency sine wave
    WAVInput AudioInput("Test_Files/LowFreqHighFreq.wav");
    AudioInput.readData();

    // Compare normalized data stored in WAVInput with data from CSV
    Eigen::MatrixXd Audio = AudioInput.getData();
    ASSERT_EQ(Audio.rows(), True_Audio_LowFreq.rows());
    ASSERT_EQ(Audio.cols(), True_Audio_LowFreq.cols());

    // Compute the FFT of the mixed audio signal
    Fourier Audio_Fourier(Audio, false);
    Audio_Fourier.transform();
    Eigen::Matrix<std::complex<double>, -1, -1> Audio_FFT = Audio_Fourier.get_fft_result<double>();

    // Filter the FFT of the mixed Audio
    SignalProcessor processor(Audio_FFT, true);
    double Cutoff = 0.01; //Does not exactly correspond to 50  Hz but its lower than 5000 Hz
    Eigen::Matrix<std::complex<double>, -1, -1> Filtered_Audio_FFT = processor.applyLowPassFilter(Cutoff);

    // Compute the inverse FFT of the filtered FFT
    Audio_Fourier.load_transform(Filtered_Audio_FFT, false);
    Audio_Fourier.inverse_transform();
    Eigen::MatrixXd Audio_LowFreq = Audio_Fourier.get_inverse_result<double>();

    // Compare the two results
    ASSERT_EQ(Audio_LowFreq.rows(), True_Audio_LowFreq.rows());
    ASSERT_EQ(Audio_LowFreq.cols(), True_Audio_LowFreq.cols());

    double residual = (Audio_LowFreq - True_Audio_LowFreq).norm();
    ASSERT_LT(residual, 1e-16);

}

TEST(FilterTest, ValidateHighPassFilter) {

    //Read CSV file with data of the low frequency (50 hz) sine signal
    std::ifstream file("Test_Files/HighFreq.csv");
    ASSERT_TRUE(file.is_open());

    //Read true audio data from the csv file
    std::string line;
    std::getline(file, line);
    std::vector<double> csv_data;

    while (std::getline(file, line)) {
        char* tok = NULL;
        tok = strtok(&line[0], ",");
        while (tok != NULL) {
            csv_data.push_back(atof(tok));
            tok = strtok(NULL, ",");
        }
    }

    //Convert CSV data into Eigen Matrix
    int num_channels = 1;
    Eigen::MatrixXd True_Audio_HighFreq(num_channels, (csv_data.size() / num_channels));
    for (int i = 0; i < num_channels; ++i) {
        for (int j = 0; j < True_Audio_HighFreq.cols(); ++j) {
            True_Audio_HighFreq(i, j) = csv_data[j * num_channels + i];
        }
    }

    // Extract audio from the audio file with both a low (50 Hz) and high (5000 Hz) frequency sine wave
    WAVInput AudioInput("Test_Files/LowFreqHighFreq.wav");
    AudioInput.readData();

    // Compare normalized data stored in WAVInput with data from CSV
    Eigen::MatrixXd Audio = AudioInput.getData();
    ASSERT_EQ(Audio.rows(), True_Audio_HighFreq.rows());
    ASSERT_EQ(Audio.cols(), True_Audio_HighFreq.cols());

    // Compute the FFT of the mixed audio signal
    Fourier Audio_Fourier(Audio,0);
    Audio_Fourier.transform();
    Eigen::Matrix<std::complex<double>, -1, -1> Audio_FFT = Audio_Fourier.get_fft_result<double>();

    // Filter the FFT of the mixed Audio
    SignalProcessor processor(Audio_FFT,1);
    double Cutoff = 0.18; //Does not exactly correspond to 5000 Hz but its higher than 50 Hz
    Eigen::Matrix<std::complex<double>, -1, -1> Filtered_Audio_FFT = processor.applyHighPassFilter(Cutoff);

    // Compute the inverse FFT of the filtered FFT
    Audio_Fourier.load_transform(Filtered_Audio_FFT, false);
    Audio_Fourier.inverse_transform();
    Eigen::MatrixXd Audio_HighFreq = Audio_Fourier.get_inverse_result<double>();

    // Compare the two results
    ASSERT_EQ(Audio_HighFreq.rows(), True_Audio_HighFreq.rows());
    ASSERT_EQ(Audio_HighFreq.cols(), True_Audio_HighFreq.cols());

    double residual = (Audio_HighFreq - True_Audio_HighFreq).norm();
    ASSERT_LT(residual, 1e-16);
}


