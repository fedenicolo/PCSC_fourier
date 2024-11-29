#ifndef OUTPUT_H
#define OUTPUT_H

#include <string>
#include <Eigen/Dense>

class Output {
protected:
    std::string outputFilePath; // File path for saving the output
public:
    explicit Output(const std::string& path);

    virtual ~Output() = default;

    // Method to save the data in the desired format
    virtual void save(const Eigen::MatrixXd& data) = 0;

    std::string getOutputFilePath() const;

    void setOutputFilePath(const std::string& path);
};

#endif // OUTPUT_H
