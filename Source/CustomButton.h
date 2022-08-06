/*
  ==============================================================================

    CustomButton.h
    Author:  Harvey Fretwell

  ==============================================================================
*/

// Tell the preprocessor to only inlcude and compile this source file once ...
#pragma once

// Including the header to the JUCE library, processor and custom look and feel ...
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"

//==============================================================================

// Create a new class inheriting from the component class ...
class CustomButton  : public juce::Component
{
    /* Creating a custom component to hold a slider and a label in a flex box. */

public:
    // Create and declare all public objects used in the .cpp file ...
    CustomLookAndFeel customLookAndFeel;
    juce::TextButton button;
    
    // Declaring the constructor and destrutor methods ...
    CustomButton();
    ~CustomButton() override;

    // Declaring other public methods defined in .cpp file ...
    void create(juce::String aName, juce::String aId, CompressorAudioProcessor &processor, bool toggle);
    
    void paint (juce::Graphics&) override;
    void resized() override;

protected:
    // Create JUCE attachment objects to sync the value tree parameters to the GUI objects ...
    // (when the editor is closed the unique pointer goes out of scope which destorys and dealocates the attachments)
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> attachment;

private:
    // Asserts an error if the object is coppied ...
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomButton)
};
