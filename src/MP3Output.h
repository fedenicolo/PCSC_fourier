//
// Created by nicol on 29/11/2024.
//
#ifndef MP3OUTPUT_H
#define MP3OUTPUT_H

#include "Output.h"
#include "Sound.h"
#include <Eigen/Dense>
#include <string>

/**
 * @class MP3Output
 * @brief A class for saving data to a ".mp3" audio file.
 *
 * This class saves data stored in an Eigen Matrix to a ".mp3" audio file.
 * It inherits from the `Output` class.
 *
 * The data matrix provided is interpreted as normalized sound amplitude values. Make sure your data is normalized
 * to [-1,1].
 *
 * @see Output
 */

class MP3Output : public Output {
public:
    /**
     * @brief Constructs an MP3Output object with the specified file path and audio characteristics.
     *
     * This constructor initializes the `MP3Output` object. It sets parameters sampleRate and numChannels to be the
     * same as the corresponding attributes of the Sound class provided in the constructor.
     *
     * @param path The file path of the output file where the audio data will be saved (std::string).
     * @param sound The Sound object from which the audio data comes (Sound).
     */
    explicit MP3Output(const std::string& path, const Sound& sound);

    /**
     * @brief Saves the provided data as a ".mp3" audio file.
     *
     * This method saves normalized sound amplitude values stored in an Eigen Matrix to a ".mp3" audio file.
     *
     * The function overrides the `save` method from the `Output` class.
     *
     * @param data The Eigen Matrix of normalized sound amplitude values to be saved
     * in the ".png" image file (Eigen::MatrixXd).
     * @note Make sure the sound amplitude data is normalized to [-1,1]
     *
     * @throws exceptions if anything goes wrong during the saving process.
     * @see AudioExceptions.h for more details.
     */
    void save(const Eigen::MatrixXd& data) override;

private:
     long sampleRate; ///< Sample rate that will be used when encoding the ".mp3" file
     int numChannels; ///< Number of audio channels that will be used when encoding the ".mp3" file
};

#endif // MP3OUTPUT_H

