# Project name : PCSC_fourier
The goal of the PCSC_fourier project is to provide a set of easy-to-use tools for basic audio and image processing using object-oriented C++.

## Instructions to Build the Project

1. Navigate to the root directory of the project. Update the submodules by running:
   ```bash
   git submodule update --init --recursive
   ```

2. If you have previously built something in this directory, clear out any generated files by running:
   ```bash
   make clean
   ./clean.sh
   ```

3. Generate the Makefile for building by running:
   ```bash
   cmake -DTESTS=ON -DDOCUMENTATION=ON .
   ```

4. To build the tests, run:
   ```bash
   make tests
   ```
   You may also set the `-j<NUM_THREADS>` flag to use multiple threads for faster compilation. Note that compiling from scratch may take a while.

5. Run the tests with the command:
   ```bash
   make -i run_tests
   ```

6. To build the main program, run:
   ```bash
   make main
   ```

7. To generate the Doxygen documentation, run:
   ```bash
   make doc_doxygen
   ```
   The documentation will be created in the `docs` directory.

8. The main program and all test files will be located in the `build` directory.

## Typical program execution and usage

## Features

- **Input Handling**:
  - `BMPInput`, `PNGInput`, `WAVInput`: Classes specialized in reading BMP images, PNG images, and WAV audio files respectively.
  These classes provide easy-to-use methods to read audio and image data from different types of file. They also provide methods
  to extract basic metadata about the data signals such as the sampling rate for audio data and the width and height for images.

- **Output Generation**:
  - `MP3Output`, `PNGOutput`, `CSVOutput`: Classes used to save processed audio or image data in MP3, PNG, or CSV formats.
  These classes provide intuitive methods to encode and save audio and image data to different types of files.

- **Processing**:
  - `Fourier`: This class handles features related to the computation of the Fast Fourier Transform for both audio signals and images.
  It is also used to compute the inverse FFT. Most of the edge cases are treated internally (e. g. padding of the signal before computing its FFT).
  The Fourier class has multiple constructors and provides several templated methods allowing great flexibility in its usage.

  - `SignalProcessor`: This class provides methods to filter FFT data and therefore process audio and image data. The filters that are available to the user
  are easy-to-use low-pass and high-pass filters. The filtering methods require a cutoff value (between 0 and 1 both for image and sound)(2*fcutoff/fsample) in order to work. The class automatically handles the creation of frequency grids tailored to the input type, whether it is a 2D image or a 1D sound signal.

- **Visualization**:
  - `Histogram`: This class provides methods for computing histograms. It supports computing histograms of real-valued data, such as pixel intensity probabilities, and includes specialized functionality for handling complex-valued data. When an FFT matrix is provided, the class computes the power spectrum and generates a histogram of its values.

  - `Visualizer`: This class provides easy-to-use methods for visualizing data using different plot types with Gnuplot integration.

- **Error Handling**:
  - The project uses custom exception classes extensively to ensure robust error reporting and handling. Most exceptions are thrown in situations
  that would lead to unavoidable critical failure of the program, therefore most exceptions are handeled by simply terminating the program.

- **Third-party Libraries**:
  - `LAME`: For MP3 encoding.
  - `stb_image`: For reading and writing PNG images.
  - `GNUplot`: For generating plots.
  - `Eigen`: For matrix and vector computations.
  - `Google Test`: For implementing and managing tests.

## Tests using Google Tests

## Todos and limitations
