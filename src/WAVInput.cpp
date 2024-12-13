#include "WAVInput.h"
#include "AudioExceptions.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdint>
#include <Eigen/Dense>

WAVInput::WAVInput(const std::string& filepath) : Sound(filepath) {
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
    throw INVALID_WAV_RIFF();
  }

  //We might want to add a comparison between chunk size and file size and throw an error
  file.ignore(4);

  // Last four characters of the Header should be "WAVE" for the WAV file to be valid
  char WAVEHeader[4];
  file.read(WAVEHeader, 4);
  if (std::string(WAVEHeader, 4) != "WAVE"){
    throw  INVALID_WAV_WAVE();
  }
}
void WAVInput::ReadFMTChunk(std::ifstream& file){

  // First four characters of the FMT chunk should be "fmt" for the WAV file to be valid
  char fmtHeader[4];
  file.read(fmtHeader, 4);
  if (std::string(fmtHeader, 4) != "fmt "){
    throw INVALID_WAV_fmt();
  }

  // Skip BlocSize
  file.ignore(4);

  // Read AudioFormat if AudioFormat != 1 audio data is compressed and additional steps are required
  char Format[2];
  file.read(Format, 2);
  // Converting the char to int knowing that WAV files are using little endian
  this->AudioFormat  = (Format[0] & 0xFF) | ((Format[1] & 0xFF) << 8); // Filter with 0xFF to make sur its unsigned
  if (this->AudioFormat != 1){
    throw INVALID_WAV_AUDIO_FORMAT();
  }

  // Read Number of channels
  char Channels[2];
  file.read(Channels, 2);
  this->NumChannels  = (Channels[0] & 0xFF) | ((Channels[1] & 0xFF) << 8);
  if (this->NumChannels < 1){
    throw INVALID_WAV_NUM_CHANNELS();
  }

  // Read Sample Rate
  char sampleRate[4];
  file.read(sampleRate, 4);
  this->SampleRate = (sampleRate[0] & 0xFF) |
                     ((sampleRate[1] & 0xFF) << 8) |
                     ((sampleRate[2] & 0xFF) << 16) |
                     ((sampleRate[3] & 0xFF) << 24);
  if (this->SampleRate < 0){
    throw INVALID_WAV_SAMPLE_RATE();
  }

  // Skip Byte Rate and Block Align
  file.ignore(6);

  // Read Bits Per Sample
  char bitsPerSample[2];
  file.read(bitsPerSample, 2);
  this->BitsPerSample = (bitsPerSample[0] & 0xFF) | ((bitsPerSample[1] & 0xFF) << 8);
  if (this->BitsPerSample != 8 && this->BitsPerSample != 16 && this->BitsPerSample != 24 && this->BitsPerSample != 32){
    throw INVALID_WAV_BITS_SAMPLE();
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
    throw INVALID_WAV_DATA_CHUNK();
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
  AudioData.resize(NumChannels,NumSamples);

  // Read Audio Data
  std::vector<char> DataBuffer(DataSize);
  file.read(DataBuffer.data(), DataSize); // Read the whole data chunk
  for (int sample = 0; sample < NumSamples; ++sample) {
    for (int channel = 0; channel < NumChannels; ++channel) {
      // Data_buffer : [Bytes of Sample1 of Channel1, Bytes of Sample1 of Channel2,...]
      int sampleIndex = (sample * NumChannels + channel) * bytesPerSample;

      // From Microsoft Specifications : 8 bits per sample is always unsigned
      if (BitsPerSample == 8) {
        uint8_t DataSample = DataBuffer[sampleIndex];
        AudioData(channel,sample) = static_cast<double>(((double)DataSample - 128)/128);
      // From Microsoft Specifications : 9 bits per sample or more is always signed
      } else if (BitsPerSample == 16) {
        int16_t DataSample = static_cast<int16_t>((static_cast<uint8_t>(DataBuffer[sampleIndex+1]) << 8) |
                                                   static_cast<uint8_t>(DataBuffer[sampleIndex]));
        AudioData(channel,sample) = static_cast<double>((double)DataSample/32768.0);
      } else if (BitsPerSample == 24) {
        int32_t DataSample = static_cast<int32_t> ((0x00 << 24) |
                                                    static_cast<uint8_t>(DataBuffer[sampleIndex+2]) << 16 |
                                                    static_cast<uint8_t>(DataBuffer[sampleIndex+1]) << 8 |
                                                    static_cast<uint8_t>(DataBuffer[sampleIndex]));
        // Sign extension is needed because we cast 3 bits in a int32_t which is 4 bits
        if (DataSample & 0x800000) {  // Check if the sign bit (23rd) is set to 1 using a mask
          // Set the first four bits to 1 because the 24-bit number is encoded in a 32-bit integer
          DataSample = DataSample | 0xFF000000;
        }
        AudioData(channel,sample) = static_cast<double>((double)DataSample/8388608.0);

      } else if (BitsPerSample == 32) {
        int32_t DataSample = static_cast<int32_t> (static_cast<uint8_t>(DataBuffer[sampleIndex+3]) << 24 |
                                                   static_cast<uint8_t>(DataBuffer[sampleIndex+2]) << 16 |
                                                   static_cast<uint8_t>(DataBuffer[sampleIndex+1]) << 8 |
                                                   static_cast<uint8_t>(DataBuffer[sampleIndex]));
        AudioData(channel,sample) = static_cast<double>((double)DataSample/2147483648.0);
      } else {
        throw INVALID_WAV_BITS_SAMPLE();
      }

    }
  }

}

void WAVInput::readData(){
  // Open the stream to the file
  std::ifstream file(filepath, std::ios::binary);
  if (!file.is_open()) {
    throw INVALID_WAV_FILE_OPEN("Unable to open WAV file : " + filepath);
  }

  // Reads the first byte of the file without removing it from the buffer. If empty it returns an End Of File trait
  if (file.peek() == std::ifstream::traits_type::eof()) {
  throw INVALID_WAV_FILE_EMPTY();
  }

  // Read the file
  ReadHeader(file);
  ReadFMTChunk(file);
  ReadDataChunk(file);

  // Close the stream
  file.close();
}


