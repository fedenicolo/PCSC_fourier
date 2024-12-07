//
// Created by nicol on 24/11/2024.
//
#ifndef AUDIOEXCEPTIONS_H
#define AUDIOEXCEPTIONS_H

#include "ext_libraries/mpg123-1.32.8/local/include/mpg123.h"
#include <exception>
#include <stdexcept>

class INVALID_WAV_RIFF : public std::exception{
public:
    const char* what() const noexcept override{
        return "Invalid WAV file: Missing RIFF header.";
    }
};

class INVALID_WAV_WAVE : public std::exception{
public:
    const char* what() const noexcept override{
        return "Invalid WAV file : Missing WAVE header.";
    }
};

class INVALID_WAV_fmt : public std::exception{
public:
    const char* what() const noexcept override{
        return "Invalid WAV file: Missing fmt chunk.";
    }
};

class INVALID_WAV_AUDIO_FORMAT : public std::exception{
public:
    const char* what() const noexcept override{
        return "Unsupported WAV file: Compressed Audio is not supported.";
    }
};

class INVALID_WAV_NUM_CHANNELS : public std::exception{
public:
    const char* what() const noexcept override{
        return "Invalid WAV file: Invalid number of channels.";
    }
};

class INVALID_WAV_SAMPLE_RATE : public std::exception{
public:
    const char* what() const noexcept override{
        return "Invalid WAV file: Invalid sample rate.";
    }
};

class INVALID_WAV_BITS_SAMPLE : public std::exception{
public:
    const char* what() const noexcept override{
        return "Invalid WAV file: Invalid bits per sample.";
    }
};

class INVALID_WAV_DATA_CHUNK : public std::exception{
public:
    const char* what() const noexcept override{
        return "Invalid WAV file: Invalid data chunk.";
    }
};

class INVALID_WAV_FILE_OPEN : public std::exception{
private:
    std::string error_message;

public:
    INVALID_WAV_FILE_OPEN(const std::string& custom_message) {
        error_message = custom_message;
    }

    const char* what() const noexcept override {
        return error_message.c_str();
    }
};

class INVALID_WAV_FILE_EMPTY : public std::exception{
public:
    const char* what() const noexcept override{
        return "WAV file is empty.";
    }
};

class INIT_MPG123 : public std::exception{
public:
    const char* what() const noexcept override{
        return "Failed to initialize mpg123 handle.";
    }
};

class INVALID_MP3_FILE_OPEN : public std::exception{
private:
    std::string error_message;

public:
    INVALID_MP3_FILE_OPEN(const std::string& custom_message) {
        error_message = custom_message;
    }

    const char* what() const noexcept override {
        return error_message.c_str();
    }
};

class INVALID_MP3_FORMAT_DETAILS : public std::exception{
public:
    const char* what() const noexcept override{
        return "Failed to get MP3 format details.";
    }
};

class INVALID_MP3_ENCODING_FORMAT : public std::exception{
public:
    const char* what() const noexcept override{
        return "Unsupported encoding format.";
    }
};

class INVALID_MP3_READING : public std::exception {
private:
    std::string error_message;

public:
    INVALID_MP3_READING(const std::string& custom_message, mpg123_handle* mh) {
        error_message = custom_message + ": " + mpg123_strerror(mh);
    }

    const char* what() const noexcept override {
        return error_message.c_str();
    }
};


#endif // AUDIOEXCEPTIONS_H
