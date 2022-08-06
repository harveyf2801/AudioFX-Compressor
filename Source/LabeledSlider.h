/*
  ==============================================================================

    LabeledSlider.h
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
class LabeledSlider  : public juce::Component
{
    /* Creating a custom component to hold a slider and a label in a flex box. */

public:
    // Create and declare all public objects used in the .cpp file ...
    CustomLookAndFeel customLookAndFeel;
    juce::Slider slider;
    
    // Declaring the constructor and destrutor methods ...
    LabeledSlider();
    ~LabeledSlider() override;

    // Declaring other public methods defined in .cpp file ...
    void create(juce::Slider::SliderStyle aStyle, juce::String aName, juce::String aId, CompressorAudioProcessor &processor, juce::String aSuffix = "");
    
    void paint (juce::Graphics&) override;
    void resized() override;

protected:
    // Create and declare all private objects used in the .cpp file ...
    juce::Label label;
    
    // Create JUCE attachment objects to sync the value tree parameters to the GUI objects ...
    // (when the editor is closed the unique pointer goes out of scope which destorys and dealocates the attachments)
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;

private:
    // Asserts an error if the object is coppied ...
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LabeledSlider)
};
