//
// Created by nicol on 24/11/2024.
//
#ifndef WAVINPUT_H
#define WAVINPUT_H

#include "Sound.h"
#include "Eigen/Dense"
#include <vector>
#include <string>

/** @class WAVInput
 *  @brief This is a class designed to read a ".wav" audio file.
 *
 * This class allows to easily extract audio data from a ".wav" file. It provides methods to access the audio data,
 * as well as basic information about the audio such as sample rate, number of channels,
 * bits per sample and audio format.
 *
 * This class inherits from the Sound class and overrides its methods to provide specific functionalities for
 * ".wav" files.
 *
 * @see Sound
 */

class WAVInput : public Sound {
public:
    /**
     * @brief Constructs a WAVInput object for the given file.
     * @param filepath The path to the ".wav" file to be read (std::string).
     */
    explicit WAVInput(const std::string& filepath);

    /**
     * @brief Gets the audio data stored locally in the object as an Eigen matrix.
     *
     * This method gets the audio data stored locally in the object in the form of an Eigen matrix of doubles,
     * where each row represents an audio channel and each column represents an audio sample.
     *
     * @return Eigen::MatrixXd The matrix containing the audio data.
     */
    Eigen::MatrixXd getData() override {return AudioData;}

    /**
     * @brief Gets the sampling rate of the audio.
     * @return int The sampling rate of the audio.
     */
    int getSampleRate() const override {return SampleRate;}

    /**
     * @brief Gets the number of audio channels in the audio file.
     * @return int The number of audio channels (e.g., 1 for mono, 2 for stereo).
     */
    int getNumChannels() const override {return NumChannels;}

    /**
     * @brief Gets the bits per sample of the audio.
     * @return int The number of bits used to encode each audio sample in the ".wav" file (e.g., 8bit, 16bit,...).
     */
    int getBitsPerSample() const {return BitsPerSample;}

    /**
     * @brief Gets the audio format of the ".wav" file.
     * @return int The audio format, represented as an integer (e.g., PCM 8bit unsigned = 1).
     */
    int getAudioFormat() const {return AudioFormat;}

    /**
     * @brief Reads the audio data from the file.
     *
     * This method reads audio data from the ".wav" file and stores it internally in an Eigen Matrix of doubles.
     * The audio data is accessed using the getData method.
     *
     * @throws exceptions if anything goes wrong during the reading of the file.
     * @see AudioExceptions.h for more details.
     */
    void readData() override;

private:
    int AudioFormat;          ///< The audio format (e.g., PCM 8bit unsigned).
    Eigen::MatrixXd AudioData;///< Eigen Matrix to store the audio data.

    /**
     * @brief Reads the header chunk of the ".wav"" file.
     * @param file The input file stream used to read the file (std::ifstream).
     * @throws exceptions related to the reading of the header if anything goes wrong.
     * @see AudioExceptions.h for more details.
     */
    void ReadHeader(std::ifstream& file);

    /**
     * @brief Reads the "fmt " chunk of the ".wav"" file.
     * @param file The input file stream used to read the file (std::ifstream).
     * @throws exceptions related to the reading of the fmt chunk if anything goes wrong.
     * @see AudioExceptions.h for more details.
     */
    void ReadFMTChunk(std::ifstream& file);

    /**
     * @brief Reads the "data" chunk of the ".wav" file.
     * @param file The input file stream used to read the file (std::ifstream).
     * @throws exceptions related to the reading of the data chunk if anything goes wrong.
     * @see AudioExceptions.h for more details.
     */
    void ReadDataChunk(std::ifstream& file);
};

#endif // WAVINPUT_H
