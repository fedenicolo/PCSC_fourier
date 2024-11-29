//
// Created by nicol on 29/11/2024.
//
#ifndef PNGOUTPUT_H
#define PNGOUTPUT_H

#include "Output.h"
#include <Eigen/Dense>
#include <string>

class PNGOutput : public Output {
public:
    explicit PNGOutput(const std::string& path);

    void save(const Eigen::MatrixXd& data) override;
};

#endif // PNGOUTPUT_H

