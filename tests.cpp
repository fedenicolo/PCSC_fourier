/*
 * exercise-template.cpp <--- The name of the source file goes here
 *
 * <--- Description of the program goes here.
 *
 * Created on: September 02, 2015 <--- Fill in the date here
 * 	   Author: Davide Forti <davide.forti@epfl.ch> <--- Fill in your name (and e-mail) here
 */

/*
 * Any headers you need to include should be specified on the next lines
 */
#include "WAVInput.h"
#include "Input.h"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <cstdint>
#include <Eigen/Dense>

int main(int argc, char* argv[])
{
    std::string filename = "/home/suttor/Desktop/myfiles/MATH-458_PCSC/math-458-pcsc/work/PCSC_Project/Test_files/Test_24Bit.wav";
    WAVInput AudioInput(filename);
    AudioInput.readData();
    Eigen::MatrixXd AudioData = AudioInput.getData();
    std::cout << "Audio Data: " << AudioData << std::endl;
    std::cout << "Audio Format: " << AudioInput.getAudioFormat() << std::endl;
    std::cout << "Audio Channels: " << AudioInput.getNumChannels() << std::endl;
    std::cout << "Sample Rate: " << AudioInput.getSampleRate() << std::endl;
    std::cout << "Bits Per Sample: " << AudioInput.getBitsPerSample() << std::endl;
    return 0;
}

