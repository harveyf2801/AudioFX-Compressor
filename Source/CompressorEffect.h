/*
  ==============================================================================

    CompressorEffect.h
    Author:  Harvey Fretwell

  ==============================================================================
*/

// Tell the preprocessor to only inlcude and compile this source file once ...
#pragma once

// Including the header to our custom circular buffer ...
#include "CircularBuffer.h"

// Create our compressor class ...
class CompressorEffect
{
public:
    // Declaring the constructor and deconstructor methods ...
    CompressorEffect();
    ~CompressorEffect();
    
    // Declaring other public methods ...
    float processSample(float sample, float threshold, float ratio, float attack, float release, float knee);
    float interpolatePoints(float* xPoints, float* yPoints, float detectedValue);
    
private:
    // Declaring private attributes ...
    CircularBuffer cyclicBuffer; // creating a cyclic buffer for our compressor.
    
    float averageTime, gRMS, gain;
};
