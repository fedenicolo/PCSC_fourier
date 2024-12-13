#include "gtest/gtest.h"
#include "WAVInput.h"
#include "Eigen/Dense"
#include "MP3Output.h"

TEST(MP3OutputTest, MP3SaveFile){
    WAVInput wavfile("Test_Files/Sine16bit.wav");
    wavfile.readData();
    Eigen::MatrixXd audio_data = wavfile.getData();

    MP3Output mp3file("./Sine16bitOutput.mp3", wavfile);
    mp3file.save(audio_data);
}