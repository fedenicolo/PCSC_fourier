//
// Created by nicol on 17/11/2024.
//

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <vector>

class Histogram {
public:
    Histogram(int numBins, double minRange, double maxRange);

    void compute(const std::vector<double> &data);

    const std::vector<int>& getBins() const;

    std::vector<double> getNormalizedHistogram() const;

    std::vector<std::tuple<double, int, double>> getHistogramData() const;

    void visualize() const;

private:
    std::vector<int> bins;
    int numBins;
    double minRange;
    double maxRange;
};

#endif // HISTOGRAM_H

