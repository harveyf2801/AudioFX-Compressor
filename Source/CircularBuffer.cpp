/*
  ==============================================================================

    CircularBuffer.cpp
    Author:  Harvey Fretwell

  ==============================================================================
*/

#include "CircularBuffer.h"

CircularBuffer::CircularBuffer()
{
    // Initialise all variables and set to 0 if values aren't given ...
    buffer = juce::AudioSampleBuffer(); // empty buffer - 0 channels, 0 length.
    writeIndex = 0;
    readIndex = 0;
    delaylength = 0;
}

CircularBuffer::CircularBuffer(int buffersize, int delaylength)
{
    // Initialise all variables with given buffer size and delay length ...
    buffer = juce::AudioSampleBuffer(1, buffersize); // create a 1 channel buffer with given size.
    buffer.clear(); // makes sure the buffer is empty.

    // Creating a FIFO style queue ...
    writeIndex = delaylength; // set write index to the buffers end.
    readIndex = 0; // set read index to the buffers start.
    this->delaylength = delaylength; // set delay length parameter as circular buffer delay length attribute.
}

CircularBuffer::~CircularBuffer()
{

}

float CircularBuffer::getData()
{
    return buffer.getSample(0, readIndex); // return sample at the read index.
}

void CircularBuffer::setData(float data)
{
    buffer.setSample(0, writeIndex, data); // set sample at the write index.
}

void CircularBuffer::nextSample()
{
    int bufferlength = buffer.getNumSamples(); // get number of samples needed to fill each channel in the buffer.

    // Rotating the head and tail indexes of the circular buffer to the next sample ...
    readIndex = ((bufferlength + writeIndex) - delaylength) % bufferlength;
    writeIndex = (writeIndex + 1) % bufferlength;
}
