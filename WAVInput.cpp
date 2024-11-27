#include "WAVInput.h"
#include "Input.h"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <cstdint>
#include <Eigen/Dense>

WAVInput::WAVInput(const std::string& filepath) : Input(filepath) {
  AudioFormat=0;
  NumChannels=0;
  SampleRate = 0;
  BitsPerSample = 0;
}

void WAVInput::ReadHeader (std::ifstream& file){
  // First four characters of the Header should be "RIFF" for the WAV file to be valid
  char Riff[4];
  file.read(Riff, 4);
  if (std::string(Riff,4) != "RIFF"){
    throw std::runtime_error("Invalid WAV file: Missing RIFF header");
  }

  //We might want to add a comparison between chunk size and file size and throw an error
  file.ignore(4);

  // Last four characters of the Header should be "WAVE" for the WAV file to be valid
  char WAVEHeader[4];
  file.read(WAVEHeader, 4);
  if (std::string(WAVEHeader, 4) != "WAVE"){
    throw  std::runtime_error("Invalid WAV file : Missing WAVE header");
  }
}
void WAVInput::ReadFMTChunk(std::ifstream& file){

  // First four characters of the FMT chunk should be "fmt" for the WAV file to be valid
  char fmtHeader[4];
  file.read(fmtHeader, 4);
  if (std::string(fmtHeader, 4) != "fmt "){
    throw std::runtime_error("Invalid WAV file: Missing fmt chunk");
  }

  // Skip BlocSize
  file.ignore(4);

  // Read AudioFormat if AudioFormat != 1 audio data is compressed and additional steps are required
  char Format[2];
  file.read(Format, 2);
  // Converting the char to int knowing that WAV files are using little endian
  this->AudioFormat  = (Format[0] & 0xFF) | ((Format[1] & 0xFF) << 8); // Filter with 0xFF to make sur its unsigned
  if (this->AudioFormat != 1){
    throw std::runtime_error("Unsupported WAV file: Compressed Audio is not supported");
  }

  // Read Number of channels
  char Channels[2];
  file.read(Channels, 2);
  this->NumChannels  = (Channels[0] & 0xFF) | ((Channels[1] & 0xFF) << 8);
  if (this->NumChannels < 1){
    throw std::runtime_error("Invalid WAV file: Invalid number of channels");
  }

  // Read Sample Rate
  char sampleRate[4];
  file.read(sampleRate, 4);
  this->SampleRate = (sampleRate[0] & 0xFF) |
                     ((sampleRate[1] & 0xFF) << 8) |
                     ((sampleRate[2] & 0xFF) << 16) |
                     ((sampleRate[3] & 0xFF) << 24);
  if (this->SampleRate < 0){
    throw std::runtime_error("Invalid WAV file: Invalid sample rate");
  }

  // Skip Byte Rate and Block Align
  file.ignore(6);

  // Read Bits Per Sample
  char bitsPerSample[2];
  file.read(bitsPerSample, 2);
  this->BitsPerSample = (bitsPerSample[0] & 0xFF) | ((bitsPerSample[1] & 0xFF) << 8);
  if (this->BitsPerSample != 8 && this->BitsPerSample != 16 && this->BitsPerSample != 24){
    throw std::runtime_error("Invalid WAV file: Invalid bits per sample");
  }
}

void WAVInput::ReadDataChunk(std::ifstream& file){

  // Read Data chunk header to make sure the file is valid
  char DataChunkHeader[4];
  file.read(DataChunkHeader, 4);
  if (std::string(DataChunkHeader, 4) == "JUNK"){
    // Skip JUNK chunk if there is one
    file.ignore(32);
  }
  if (std::string(DataChunkHeader, 4) != "data"){
    throw std::runtime_error("Invalid WAV file: Invalid data chunk");
  }

  // Read Data Size chunk
  char DataChunkSize[4];
  file.read(DataChunkSize, 4);
  int DataSize = (DataChunkSize[0] & 0xFF) |
                 ((DataChunkSize[1] & 0xFF) << 8) |
                 ((DataChunkSize[2] & 0xFF) << 16) |
                 ((DataChunkSize[3] & 0xFF) << 24);

  // Calculate the number of samples
  int bytesPerSample = BitsPerSample / 8; // Can only result in an integer because BitsPerSample is in {8,16,24}
  int totalSamples = DataSize / bytesPerSample;

  // Allocate memory for the data matrix
  int NumSamples = totalSamples / NumChannels; // Samples per channel, if the file is valid this should be an integer
  AudioData.resize(NumSamples,NumChannels);

  // Read Audio Data
  std::vector<char> DataBuffer(DataSize);
  file.read(DataBuffer.data(), DataSize); // Read the whole data chunk
  for (int sample = 0; sample < NumSamples; ++sample) {
    for (int channel = 0; channel < NumChannels; ++channel) {
      // Data_buffer : [Bytes of Sample1 of Channel1, Bytes of Sample1 of Channel2,...]
      int sampleIndex = (sample * NumChannels + channel) * bytesPerSample;

      // Convert bytes to integer sample using the same logic as before
      int DataSample = 0;
      // From Microsoft Specifications : 8 bits per sample is always unsigned
      if (BitsPerSample == 8) {
        DataSample = (DataBuffer[sampleIndex] & 0xFF) - 128;
      // From Microsoft Specifications : 9 bits per sample or more is always signed
      } else if (BitsPerSample == 16) {
        DataSample = (DataBuffer[sampleIndex] & 0xFF) | ((DataBuffer[sampleIndex + 1] & 0xFF) << 8) - 32768;
      } else if (BitsPerSample == 24) {
        DataSample = ((DataBuffer[sampleIndex] & 0xFF) |
             		      (DataBuffer[sampleIndex + 1] & 0xFF) << 8 |
             		      (DataBuffer[sampleIndex + 2] & 0xFF) << 16) - 8388607;
      } else {
        throw std::runtime_error("Unsupported BitsPerSample");
      }

      // Store sample in the matrix
      AudioData(sample, channel) = static_cast<double>(DataSample);
    }
  }

}

void WAVInput::readData(){
  // Open the stream to the file
  std::ifstream file(filepath, std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("Unable to open WAV file: " + filepath);
  }

  // Reads the first byte of the file without removing it from the buffer. If empty it returns an End Of File trait
  if (file.peek() == std::ifstream::traits_type::eof()) {
  throw std::runtime_error("WAV file " + filepath + " is empty") ;
}
  try{
    // Read the file
    ReadHeader(file);
    ReadFMTChunk(file);
    ReadDataChunk(file);
  }  catch (const std::runtime_error& ErrorMessage) {
    std::cerr << "Runtime error: " << ErrorMessage.what() << std::endl;
  }

  // Close the stream
  file.close();
}

Eigen::MatrixXd WAVInput::getData_Normalized() {
  // Extract maximum values column wise to normalize data
  Eigen::MatrixXd absAudioData = AudioData.cwiseAbs();
  Eigen::VectorXd max_values = absAudioData.colwise().maxCoeff();

  // Copy Audio Data
  Eigen::MatrixXd NormalizedData = AudioData;

  // Normalize Data
  for (int i = 0; i < NumChannels; i++) {
    NormalizedData.col(i) /= max_values(i);
  }
  return NormalizedData;
}

