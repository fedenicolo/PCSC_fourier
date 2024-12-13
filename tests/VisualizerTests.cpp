#include <iostream>
#include "Visualizer.h"
#include "WAVInput.h"
#include "Eigen/Dense"


int main(){
    WAVInput wav_file("Test_Files/Sine24Bit.wav");
    wav_file.readData();
    Eigen::MatrixXd audio_data = wav_file.getData();
    
    Visualizer vs = Visualizer();
    vs.plot(audio_data, "histogram", 32, "hist_sine24b.png");
    vs.plot(audio_data, "line", 32, "line_sine24b.png");
    vs.printSummary(audio_data);

    return 0;
}