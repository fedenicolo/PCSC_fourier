//
// Created by nicol on 24/11/2024.
//
#ifndef MP3INPUT_H
#define MP3INPUT_H

#include "Sound.h"
#include <Eigen/Dense>
#include <vector>
#include <string>

class MP3Input : public Sound {
public:
    explicit MP3Input(const std::string& filepath);
    int getSampleRate() const override {return SampleRate;}
    int getNumChannels() const override {return NumChannels;}
    int getAudioFormat() const{ return AudioFormat;}
    void readData() override;
    Eigen::MatrixXd getData() override { return AudioData;}

private:
    int AudioFormat;
    int NumChannels;
    long SampleRate;
    Eigen::MatrixXd AudioData;
};

#endif // MP3INPUT_H
