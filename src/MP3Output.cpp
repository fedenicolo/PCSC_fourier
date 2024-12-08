//
// Created by nicol on 29/11/2024.
//
#include "MP3Output.h"
#include "AudioExceptions.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include "ext_libraries/lame-3.100/include/lame.h"

MP3Output::MP3Output(const std::string& path, const Sound& sound) : Output(path) {
    sampleRate = sound.getSampleRate();
    numChannels = sound.getNumChannels();
}

void MP3Output::save(const Eigen::MatrixXd& data) {

   if (numChannels != 1 && numChannels != 2) {
        throw INVALID_LAME_CHANNELS();
    }

    //Initialize LAME encoder
    lame_t lame = lame_init();
    if (!lame) {
        throw INVALID_LAME_INIT();
    }

    //Configure LAME parameters
    lame_set_in_samplerate(lame, sampleRate);
    lame_set_num_channels(lame, numChannels);
    lame_set_brate(lame, 128); // Bitrate in kbps
    lame_set_quality(lame, 2); // High-quality VBR
    lame_init_params(lame);

    //Prepare input audio data
    int numSamples = static_cast<int>(data.size());
    std::vector<float> pcmData(numSamples);

    for (int channel = 0; channel < data.rows(); ++channel) {
        for (int sample = 0; sample < data.cols(); ++sample) {
            pcmData[sample * data.rows() + channel] = static_cast<float>(data(channel, sample));
        }
    }

    //Encode PCM data to MP3
    const size_t bufferSize = 1.25 * numSamples + 7200; // Estimate MP3 buffer size (from LAME documentation)
    std::vector<unsigned char> mp3Buffer(bufferSize);

    int writeSize = 0;
    if (numChannels == 1) {
        //Mono
        writeSize = lame_encode_buffer_ieee_float(lame, pcmData.data(), nullptr, numSamples, mp3Buffer.data(), bufferSize);
    } else if (numChannels == 2) {
        //Stereo
        const float* leftChannel = pcmData.data();
        writeSize = lame_encode_buffer_interleaved_ieee_float(lame, leftChannel, numSamples / numChannels, mp3Buffer.data(), bufferSize);
    }

    if (writeSize < 0) {
        lame_close(lame);
        throw LAME_FAILED_ENCODE();
    }

    //Write MP3 data to file
    std::ofstream outFile(outputFilePath, std::ios::binary);
    if (!outFile.is_open()) {
        lame_close(lame);
        throw INVALID_MP3_FILE_OPEN("Failed to open output file: " + outputFilePath);
    }
    outFile.write(reinterpret_cast<const char*>(mp3Buffer.data()), writeSize);

    //Finalize encoding
    int flushSize = lame_encode_flush(lame, mp3Buffer.data(), bufferSize);
    if (flushSize > 0) {
        outFile.write(reinterpret_cast<const char*>(mp3Buffer.data()), flushSize);
    }

    lame_close(lame);
    outFile.close();

    std::cout << "Saved MP3 file to: " << outputFilePath << std::endl;
}

