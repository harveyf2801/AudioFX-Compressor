/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.
          The editor provides a GUI and visualisations for the user.

             . Editor class can have multiple objects
             . Editors job is to set/get info on processor thread

  ==============================================================================
*/

// Tell the preprocessor to only inlcude and compile this source file once ...
#pragma once

// Including the headers to the JUCE library and the audio processing, as well as other custom components ...
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"
#include "ButtonBox.h"
#include "DialBox.h"
#include "SliderBox.h"

//==============================================================================

class CompressorAudioProcessorEditor  : public juce::AudioProcessorEditor
{
    /* This class inherits from the JUCE AudioProcessorEditor class. */

public:
    // Declaring the constructor and destrutor methods ...
    CompressorAudioProcessorEditor (CompressorAudioProcessor&);
    ~CompressorAudioProcessorEditor() override;

    //==============================================================================

    // Declaring other public methods defined in .cpp file ...
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    //==============================================================================

    /* ! OBJECTS CONSTRUCTED TOP DOWN BUT DESTRUCTED BOTTOM UP ! */

    // This is a reference for the editor to access the processor object that created it ...
    CompressorAudioProcessor& audioProcessor;
    
    // Create and declare all private objects used in the .cpp file ...
    ButtonBox buttonBox;
    DialBox dialBox;
    SliderBox sliderBox;
    
    CustomLookAndFeel customLookAndFeel;

    //==============================================================================

    // Asserts an error if the object is coppied ...
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorAudioProcessorEditor)
};
