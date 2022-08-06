/*
  ==============================================================================

    CompressorEffect.cpp
    Author:  Harvey Fretwell

  ==============================================================================
*/

#include "CompressorEffect.h"

CompressorEffect::CompressorEffect()
{
    // Defining the variables ...
    cyclicBuffer = CircularBuffer(150, 20);
    averageTime = 0.01;
    gRMS = 0;
    gain = 1;
}

CompressorEffect::~CompressorEffect()
{
    
}

float CompressorEffect::processSample(float sample, float threshold, float ratio, float attack, float release, float knee)

//                                    - COMPRESSOR -

//      x = input signal            c = control signal          T = threshold
//      y = output signal              m = makeup                 R = ratio

//                          for gain -> y[i] = x[i] * c[i] * m
//                          for dB   -> y[i] = x[i] + c[i] + m

{
    // Level detection - estimate level using peak rms detection ...
    gRMS = (1 - averageTime) * gRMS + averageTime * std::pow(sample, 2.0f); // find rms based on preious sample's RMS.
    float dbRMS = 10 * std::log10(gRMS); // convert rms (gain) to rms (dB).
    
    // Create slope if hard knee
    float slope = 1 - (1 / ratio); // calculating slope/gradient from ratio value.
    /* if ratio = 1:1 output-> 0
       if ratio = 1:2 output-> 0.5
       if ratio = 1:4 output-> 0.75
       if ratio = 1:20 output-> 0.95 */
    // stops slope from exeeding 1.
    
    // Calculating the knee width ...
    float kneeBottom = threshold - knee / 2.0;
    float kneeTop = threshold + knee / 2.0;
    
    if (knee > 0 && dbRMS > (kneeBottom) && dbRMS < (kneeTop)) // see if current rms value is within the knee points.
    {
        float xPoints[2]; // holds db values of bottom and top of the knee region.
        float yPoints[2]; // holds the slope values.
        
        xPoints[0] = kneeBottom;
        xPoints[1] = std::fmin(0.0f, kneeTop); // caps knee top at 0
        yPoints[0] = 0.0f;
        yPoints[1] = slope;
        slope = interpolatePoints(&xPoints[0], &yPoints[0], threshold); // interpolate a new slope value
        threshold = kneeBottom;
    }
    
    // Gain computer - static apply input/output curve ...
    float dbGain = std::min(0.0f, (slope * (threshold - dbRMS))); // convert gain(g) to gain(dB)
    float gGain = std::pow(10, dbGain / 20); // calculate new gain level in linear scale.
    
    // Ballistics - smoothing of the gain ...
    float coeff;
    if(gGain < gain) { coeff = attack; } // check if we're in the attack phase ...
    else { coeff = release; }           //          or the release phase.
    gain = (1 - coeff) * gain + coeff * gGain;
    
    // Apply control voltage to the audio signal ...
    float compressedSample = gain * cyclicBuffer.getData(); // compress signal using sample from read pointer.
    cyclicBuffer.setData(sample); // set current sample to write pointer.
    cyclicBuffer.nextSample(); // increment the pointers to rotate them around the cyclic buffer.
    return compressedSample;
}

float CompressorEffect::interpolatePoints(float* xPoints, float* yPoints, float detectedValue)
{
    // Standard interpolation to approximate points ...
    float result = 0.0f;
    int n = 2;
    
    for(auto i = 0; i < n; i++)
    {
        float term = 1.0f;
        for(auto j = 0; j<n; j++)
        {
            if(j != i) { term *= (detectedValue - xPoints[j]) / (xPoints[i] - xPoints[j]); }
        }
        result += term * yPoints[i];
    }
    return result;
}
