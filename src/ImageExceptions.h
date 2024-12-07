//
// Created by nicol on 24/11/2024.
//
#ifndef IMAGEEXCEPTIONS_H
#define IMAGEEXCEPTIONS_H

#include <exception>
#include <stdexcept>

class INVALID_BMP_FILE_OPEN : public std::exception{
private:
    std::string error_message;

public:
    INVALID_BMP_FILE_OPEN(const std::string& custom_message) {
        error_message = custom_message;
    }

    const char* what() const noexcept override {
        return error_message.c_str();
    }
};

class INVALID_BMP_BM : public std::exception{
public:
    const char* what() const noexcept override{
        return "Not a valid BMP file.";
    }
};

class INVALID_BMP_READ : public std::exception{
public:
    const char* what() const noexcept override{
        return "Error reading BMP pixel data.";
    }
};

class INVALID_PNG_READ : public std::exception {
private:
    std::string error_message;

public:
    INVALID_PNG_READ(const std::string& custom_message) {
        error_message = custom_message;
    }

    const char* what() const noexcept override {
        return error_message.c_str();
    }
};

class INVALID_PNG_NUM_CHANNELS : public std::exception{
    public:
    const char* what() const noexcept override{
        return "Failed to convert image, image must have 1 or 3 channels.";
    }
};


#endif // IMAGEEXCEPTIONS_H
