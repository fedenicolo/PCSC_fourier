//
// Created by nicol on 24/11/2024.
//
#ifndef IMAGEEXCEPTIONS_H
#define IMAGEEXCEPTIONS_H

#include <exception>
#include <stdexcept>


/** @class INVALID_BMP_FILE_OPEN
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when a program is unable to open a ".bmp" file.
 *  The custom message is of the form : "Unable to open BMP file : file.bmp"
 */
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

/** @class INVALID_BMP_BM
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when a program is trying to load an invalid BMP file.
 */
class INVALID_BMP_BM : public std::exception{
public:
    const char* what() const noexcept override{
        return "Not a valid BMP file.";
    }
};

/** @class INVALID_BMP_READ
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when an error occurs during the reading of a ".bmp" file.
 */
class INVALID_BMP_READ : public std::exception{
public:
    const char* what() const noexcept override{
        return "Error reading BMP pixel data.";
    }
};

/** @class INVALID_PNG_READ
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when an error occurs while reading a ".png" file.
 */
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

/** @class INVALID_PNG_NUM_CHANNELS
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when a program tries to load a png image with more than 3 channels.
 */
class INVALID_PNG_NUM_CHANNELS : public std::exception{
    public:
    const char* what() const noexcept override{
        return "Failed to convert image, image must have 1 or 3 channels.";
    }
};

/** @class INVALID_PNG_DIMENSIONS
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when a program tries to save a png image with dimensions that don't make sens (e.g. negative height).
 */
class INVALID_PNG_DIMENSIONS : public std::exception{
    public:
    const char* what() const noexcept override{
        return "Invalid image dimensions.";
    }
};

/** @class PNG_SAVE_FAIL
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when the library (stb_image) used to save ".png" images fails to save an image.
 */
class PNG_SAVE_FAIL : public std::exception{
    public:
    const char* what() const noexcept override{
        return "Failed to save PNG file.";
    }
};

/** @class INVALID_CSV_OPEN
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when a program is unable to open a ".csv" file.
 */
class INVALID_CSV_OPEN : public std::exception {
private:
    std::string error_message;

public:
    INVALID_CSV_OPEN(const std::string& custom_message) {
        error_message = custom_message;
    }

    const char* what() const noexcept override {
        return error_message.c_str();
    }
};


#endif // IMAGEEXCEPTIONS_H
