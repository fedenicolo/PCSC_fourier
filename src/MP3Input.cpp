#include "MP3Input.h"
#include "Input.h"
#include "AudioExceptions.h"
#include <fstream>
#include <iostream>
#include <Eigen/Dense>
#include "mpg123.h"

MP3Input::MP3Input(const std::string& filepath) : Sound(filepath) {
    AudioFormat=0;
    NumChannels=0;
    SampleRate = 0;
}

void MP3Input::readData(){

    //Initialize mpg123 library
    mpg123_init();
    mpg123_handle* mh = mpg123_new(nullptr, nullptr);
    if (!mh) {
        throw INIT_MPG123();
    }

    //Open the MP3 file and check if it was opened correctly
    if (mpg123_open(mh, filepath.c_str()) != MPG123_OK) {
        mpg123_delete(mh);
        mpg123_exit();
        throw INVALID_MP3_FILE_OPEN("Unable to open MP3 file: " + filepath);
    }

    //Get the format details (useful for output class)
    if (mpg123_getformat(mh, &SampleRate, &NumChannels, &AudioFormat) != MPG123_OK) {
        mpg123_close(mh);
        mpg123_delete(mh);
        mpg123_exit();
        throw INVALID_MP3_FORMAT_DETAILS();
    }

    //Read the MP3 file
    size_t DataSize = mpg123_outblock(mh);
    size_t done = 0;
    unsigned char* Databuffer = new unsigned char[DataSize];
    std::vector<float> Data;

    int error;
    while ((error = mpg123_read(mh, Databuffer, DataSize, &done)) == MPG123_OK) {
        switch (AudioFormat) {
            case MPG123_ENC_SIGNED_16: {
                // 16-bit signed PCM
                short* samples = reinterpret_cast<short*>(Databuffer);
                size_t sampleCount = done / sizeof(short);
                for (size_t i = 0; i < sampleCount; ++i) {
                    Data.push_back(std::max(samples[i] / 32767.0,-1.0));
                }
                break;
            }
            case MPG123_ENC_UNSIGNED_16: {
                // 16-bit unsigned PCM
                unsigned short* samples = reinterpret_cast<unsigned short*>(Databuffer);
                size_t sampleCount = done / sizeof(unsigned short);
                for (size_t i = 0; i < sampleCount; ++i) {
                    Data.push_back(std::max((samples[i] - 32768) / 32767.0,-1.0));
                }
                break;
            }
            case MPG123_ENC_FLOAT_32: {
                // 32-bit float PCM
                float* samples = reinterpret_cast<float*>(Databuffer);
                size_t sampleCount = done / sizeof(float);
                for (size_t i = 0; i < sampleCount; ++i) {
                    Data.push_back(samples[i]); //Already normalized
                }
                break;
            }
            default:
                delete[] Databuffer;
                mpg123_close(mh);
                mpg123_delete(mh);
                mpg123_exit();
                throw INVALID_MP3_ENCODING_FORMAT();
            }
    }

    //Check if data has been loaded correctly
    if (error != MPG123_DONE) {
        delete[] Databuffer;
        mpg123_close(mh);
        mpg123_delete(mh);
        mpg123_exit();
        throw INVALID_MP3_READING("Error reading MP3 file :" + filepath +".", mh);
    }

    //Free allocated memory for the Data buffer, close data reader, exit mpg123
    delete[] Databuffer;
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();

    //Store Data in AudioData Eigen Matrix
    size_t NumSamples = Data.size() / NumChannels;
    AudioData.resize(NumChannels, NumSamples);
    double rows = AudioData.rows();
    double cols = AudioData.cols();
    std::cout << "Number of rows: " << rows << std::endl;
    std::cout << "Number of cols: " << cols << std::endl;
    for (size_t i = 0; i < NumSamples; ++i) {
        for (int channel = 0; channel < NumChannels; ++channel) {
            AudioData(channel, i) = Data[i * NumChannels + channel];
        }
    }

}


