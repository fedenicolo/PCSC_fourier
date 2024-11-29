//
// Created by nicol on 29/11/2024.
//
#ifndef CSVOUTPUT_H
#define CSVOUTPUT_H

#include "Output.h"
#include <fstream>

class CSVOutput : public Output {
public:
    explicit CSVOutput(const std::string& path);

    void save(const Eigen::MatrixXd& data) override;
};

#endif // CSVOUTPUT_H
