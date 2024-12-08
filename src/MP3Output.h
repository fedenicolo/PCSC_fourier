//
// Created by nicol on 29/11/2024.
//
#ifndef MP3OUTPUT_H
#define MP3OUTPUT_H

#include "Output.h"
#include "Sound.h"
#include <Eigen/Dense>
#include <string>

class MP3Output : public Output {
public:
    explicit MP3Output(const std::string& path, const Sound& sound);

    void save(const Eigen::MatrixXd& data) override;

private:
     long sampleRate;
     int numChannels;
};

#endif // MP3OUTPUT_H

