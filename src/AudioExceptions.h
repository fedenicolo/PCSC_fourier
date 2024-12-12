//
// Created by nicol on 24/11/2024.
//
#ifndef AUDIOEXCEPTIONS_H
#define AUDIOEXCEPTIONS_H

#include "mpg123.h"
#include <exception>
#include <stdexcept>

/** @class INVALID_WAV_RIFF
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when the RIFF header of the ".wav" file is missing.
 */
class INVALID_WAV_RIFF : public std::exception{
public:
    const char* what() const noexcept override{
        return "Invalid WAV file: Missing RIFF header.";
    }
};

/** @class INVALID_WAV_WAVE
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when the WAVE header of the ".wav" file is missing.
 */
class INVALID_WAV_WAVE : public std::exception{
public:
    const char* what() const noexcept override{
        return "Invalid WAV file : Missing WAVE header.";
    }
};

/** @class INVALID_WAV_fmt
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when the fmt chunk of the ".wav" file is missing.
 */
class INVALID_WAV_fmt : public std::exception{
public:
    const char* what() const noexcept override{
        return "Invalid WAV file: Missing fmt chunk.";
    }
};

/** @class INVALID_WAV_AUDIO_FORMAT
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when the audio format of the ".wav" file is unsupported by the readData method of the WAVInput class.
 */
class INVALID_WAV_AUDIO_FORMAT : public std::exception{
public:
    const char* what() const noexcept override{
        return "Unsupported WAV file: Compressed Audio is not supported.";
    }
};

/** @class INVALID_WAV_NUM_CHANNELS
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when the audio data stored in the ".wav" file is not mono or stereo
 */
class INVALID_WAV_NUM_CHANNELS : public std::exception{
public:
    const char* what() const noexcept override{
        return "Invalid WAV file: Invalid number of channels.";
    }
};

/** @class INVALID_WAV_SAMPLE_RATE
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when the sampling rate of the audio data stored in the ".wav" file does not make sens (e.g. negative number).
 */
class INVALID_WAV_SAMPLE_RATE : public std::exception{
public:
    const char* what() const noexcept override{
        return "Invalid WAV file: Invalid sample rate.";
    }
};

/** @class INVALID_WAV_BITS_SAMPLE
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when the bits per sample of the audio data stored in the ".wav" file is not supported by the readData method of the
 *  WAVInput class.
 */
class INVALID_WAV_BITS_SAMPLE : public std::exception{
public:
    const char* what() const noexcept override{
        return "Invalid WAV file: Invalid bits per sample.";
    }
};

/** @class INVALID_WAV_DATA_CHUNK
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when the data chunk of the ".wav" file is invalid.
 */
class INVALID_WAV_DATA_CHUNK : public std::exception{
public:
    const char* what() const noexcept override{
        return "Invalid WAV file: Invalid data chunk.";
    }
};

/** @class INVALID_WAV_FILE_OPEN
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when a ".wav" file could not be opened.
 */
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

/** @class INVALID_WAV_FILE_EMPTY
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when a ".wav" file is empty.
 */
class INVALID_WAV_FILE_EMPTY : public std::exception{
public:
    const char* what() const noexcept override{
        return "WAV file is empty.";
    }
};

/** @class INIT_MPG123
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when the MPG123 handler could not be initialized properly.
 */
class INIT_MPG123 : public std::exception{
public:
    const char* what() const noexcept override{
        return "Failed to initialize mpg123 handle.";
    }
};

/** @class INVALID_MP3_FILE_OPEN
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when a ".mp3" file could not be opened.
 */
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

/** @class INVALID_MP3_FORMAT_DETAILS
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when the format details of a ".mp3" file are not valid for the MPG123 library.
 */
class INVALID_MP3_FORMAT_DETAILS : public std::exception{
public:
    const char* what() const noexcept override{
        return "Failed to get MP3 format details.";
    }
};

/** @class INVALID_MP3_ENCODING_FORMAT
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when the audio format of the ".mp3" file is unsupported by the readData method of the MP3Input class.
 */
class INVALID_MP3_ENCODING_FORMAT : public std::exception{
public:
    const char* what() const noexcept override{
        return "Unsupported encoding format.";
    }
};

/** @class INVALID_MP3_READING
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when the MPG123 library is not able to read a ".mp3" file properly.
 */
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

/** @class INVALID_LAME_CHANNELS
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when the number of audio channels of the audio data is not supported by the LAME library.
 */
class INVALID_LAME_CHANNELS : public std::exception{
public:
    const char* what() const noexcept override{
        return "Only mono (1) and stereo (2) channels are supported.";
    }
};

/** @class INVALID_LAME_INIT
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when the LAME encoder fails to initialize properly.
 */
class INVALID_LAME_INIT : public std::exception{
public:
    const char* what() const noexcept override{
        return "Failed to initialize LAME encoder.";
    }
};

/** @class LAME_FAILED_ENCODE
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when the LAME encoder fails to encode the audio data to the ".mp3" file.
 */
class LAME_FAILED_ENCODE : public std::exception{
public:
    const char* what() const noexcept override{
        return "Failed to encode MP3 data.";
    }
};


#endif // AUDIOEXCEPTIONS_H
