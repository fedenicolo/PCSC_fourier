# PCSC_fourier


## Histogram Class
Purpose
The Histogram class computes and visualizes histograms for numerical data, such as:
- Pixel intensities in images.
- Amplitude values in sound data.

The class uses Gnuplot for graphical visualization.

---

Features
- Numerical Data Support: Handles 1D array (sound and image data).
- Configurable Bins: Allows flexible bin sizes and data ranges.
- Normalized Output: Provides histograms as raw frequency counts or normalized probability distributions.
- Visualization: Uses Gnuplot to generate professional-looking bar charts.

---

Class Documentation

Public Methods
1. Histogram(int numBins, double minRange, double maxRange)
   Initializes the histogram with the specified number of bins and data range.
   Parameters:
   - numBins: Number of bins.
   - minRange: Minimum value in the data range.
   - maxRange: Maximum value in the data range.

2. void compute(const std::vector<double>& data)
   Computes the histogram by populating frequency counts for the input data.
   Parameter: data - Vector of numerical values.

3. std::vector<double> getNormalizedHistogram() const
   Returns the histogram as a normalized probability distribution (values between 0 and 1).

4. const std::vector<int>& getBins() const
   Provides raw frequency counts for each bin.

5. void visualize() const
   Generates a graphical representation of the histogram using Gnuplot.
