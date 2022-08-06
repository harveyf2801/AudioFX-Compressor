/*
  ==============================================================================

    CircularBuffer.h
    Author:  Harvey Fretwell

  ==============================================================================
*/

// Tell the preprocessor to only inlcude and compile this source file once ...
#pragma once

// Including the header to the JUCE library ...
#include <JuceHeader.h>

class CircularBuffer
{
    /* Creating a cyclic / circular buffer class  in a FIFO style queue
     to store samples for peak / rms analysis by the compressor. */

public:
    // Declaring our construtor and deconstructor methods ...
    CircularBuffer();
    CircularBuffer(int buffersize, int delaylength);
    ~CircularBuffer();

    // Declaring our getter/setter functions ...
    float getData(); // return data at read index.
    void setData(float data); // sets data at write index.

    void nextSample(); // increment read/write indexes.

private:
    // Declare class attributes ...
    juce::AudioSampleBuffer buffer;
    int writeIndex;
    int readIndex;
    int delaylength;
};
