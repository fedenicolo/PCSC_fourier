//
// Created by nicol on 24/11/2024.
//
#ifndef WAVINPUT_H
#define WAVINPUT_H

#include "Sound.h"
#include <Eigen/Dense>
#include <vector>
#include <string>

class WAVInput : public Sound {
public:
    explicit WAVInput(const std::string& filepath);
    Eigen::MatrixXd getData() override {return AudioData;}
    int getSampleRate() const override {return SampleRate;}
    int getNumChannels() const override {return NumChannels;}
    int getBitsPerSample() const {return BitsPerSample;}
    int getAudioFormat() const {return AudioFormat;}
    void readData() override;

private:
    int AudioFormat;
    int NumChannels;
    int SampleRate;
    int BitsPerSample;
    Eigen::MatrixXd AudioData;
    void ReadHeader(std::ifstream& file);
    void ReadFMTChunk(std::ifstream& file);
    void ReadDataChunk(std::ifstream& file);
};

#endif // WAVINPUT_H
