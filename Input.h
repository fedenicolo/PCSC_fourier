//
// Created by nicol on 24/11/2024.
//
#ifndef INPUT_H
#define INPUT_H

#include <string>
#include <vector>

class Input {
protected:
    std::string filepath;

public:
    explicit Input(const std::string& path);

    virtual ~Input() = default;

    virtual void readData() = 0;

    std::string getFilepath() const;

    virtual bool isValidFile() const;

    virtual std::vector<double> getData() const = 0;
};

#endif // INPUT_H
