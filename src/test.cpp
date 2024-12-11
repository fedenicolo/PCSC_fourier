#include "Eigen/Dense"
#include "Fourier.h"
#include "PNGInput.h"
#include "SignalProcessor.h"
#include "PNGOutput.h"


int main(){
    PNGInput img = PNGInput("../build/img.png");
    Fourier fft = Fourier(img);
    fft.transform();
    SignalProcessor sp = SignalProcessor(fft.get_fft_result<double>());
    sp.applyLowPassFilter(3.14159/2);
    Eigen::Matrix<std::complex<double>, -1, -1> filtered_data = sp.getData<double>();    
    fft.load_transform<double>(filtered_data, true);
    fft.inverse_transform();
    PNGOutput output = PNGOutput("../build/output.png");
    output.save(fft.get_fft_result<double>());
    return 0;

}