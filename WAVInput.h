//
// Created by nicol on 24/11/2024.
//
#ifndef WAVINPUT_H
#define WAVINPUT_H

#include "Input.h"
#include <Eigen/Dense>
#include <vector>
#include <string>

class WAVInput : public Input {
public:
    explicit WAVInput(const std::string& filepath);
    Eigen::MatrixXd getData() override { return AudioData; }
    int getSampleRate() const{return SampleRate;}
    int getNumChannels() const{return NumChannels;}
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
