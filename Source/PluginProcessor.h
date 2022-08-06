/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.
         The processor handles audio i/o, midi i/o and processing logic.

                (parent) PluginProcessor -> (child) PluginEditor

  ==============================================================================
*/

// Tell the preprocessor to only inlcude and compile this source file once ...
#pragma once

// Including the header to the JUCE, cmath and our custom compressor ...
#include <JuceHeader.h>
#include <cmath>
#include "CompressorEffect.h"

// Defining our constants as a global preprocessor ...

namespace IDs {
    const juce::String POWER = { "POWER" };
    const juce::String INGAIN = { "INGAIN" };
    const juce::String OUTGAIN = { "OUTGAIN" };
    const juce::String ATTACK = { "ATTACK" };
    const juce::String RELEASE = { "RELEASE" };
    const juce::String THRESHOLD = { "THRESHOLD" };
    const juce::String RATIO = { "RATIO" };
    const juce::String KNEE = { "KNEE" };
    const juce::String MIX = { "MIX" };
    const juce::String DRIVE = { "DRIVE" };
}

namespace NAMEs {
    const juce::String POWER = { "Power" };
    const juce::String INGAIN = { "Input Gain" };
    const juce::String OUTGAIN = { "Output Gain" };
    const juce::String ATTACK = { "Attack" };
    const juce::String RELEASE = { "Release" };
    const juce::String THRESHOLD = { "Threshold" };
    const juce::String RATIO = { "Ratio" };
    const juce::String KNEE = { "Knee" };
    const juce::String MIX = { "Mix" };
    const juce::String DRIVE = { "Drive" };
    const juce::String UNDO = { "<" };
    const juce::String REDO = { ">" };
}

//==============================================================================

class CompressorAudioProcessor  : public juce::AudioProcessor
{
    /* This class inherits from the JUCE AudioProcessor class. */

public:

    //==============================================================================

    // Declaring the constructor and destrutor methods ...
    // ( override keyword is used to replace inherited version of the method )
    CompressorAudioProcessor();
    ~CompressorAudioProcessor() override;

    //==============================================================================

    // Declaring public methods defined in .cpp file ...
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    // Compile if the channel configurations are compatible ...
   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================

    juce::AudioProcessorEditor* createEditor() override;

    bool hasEditor() const override;

    //==============================================================================

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // Create and declare all public objects used in the .cpp file ...
    juce::UndoManager undoManager;
    juce::AudioProcessorValueTreeState valueTree;

private:
    
    /* ! OBJECTS CONSTRUCTED TOP DOWN BUT DESTRUCTED BOTTOM UP ! */

    // Declaring private methods defined in .cpp file ...
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    // Create and declare all private objects used in the .cpp file ...
    juce::Array<CompressorEffect> multichannelCompressor; // creating an array to hold multiple compressor objects.
    
    // Declaring null pointers, ready to be assigned in the constructor ...
    std::atomic<float>* powerPtr = nullptr;
    std::atomic<float>* ingainPtr = nullptr;
    std::atomic<float>* outgainPtr = nullptr;
    std::atomic<float>* attackPtr = nullptr;
    std::atomic<float>* releasePtr = nullptr;
    std::atomic<float>* thresholdPtr = nullptr;
    std::atomic<float>* ratioPtr = nullptr;
    std::atomic<float>* kneePtr = nullptr;
    std::atomic<float>* mixPtr = nullptr;
    std::atomic<float>* drivePtr = nullptr;

    //==============================================================================

    // Asserts an error if the object is coppied ...
    // ( used when a class doesn't use 'copy-by-value' semantics )
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CompressorAudioProcessor)
};
