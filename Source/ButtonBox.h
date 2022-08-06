/*
  ==============================================================================

    ButtonBox.h
    Author:  Harvey Fretwell

  ==============================================================================
*/

// Tell the preprocessor to only inlcude and compile this source file once ...
#pragma once

// Including the header to the JUCE library, processor and custom look and feel ...
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomButton.h"
#include "CustomLookAndFeel.h"

//==============================================================================

// Create a new class inheriting from the component class ...
class ButtonBox  : public juce::Component
{
    /* Creating a custom component to hold all the buttons in a flex box. */

public:
    // Declaring the constructor and destrutor methods ...
    ButtonBox();
    ~ButtonBox() override;

    // Declaring other public methods defined in .cpp file ...
    void create(CompressorAudioProcessor &processor);
    
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // Create and declare all private objects used in the .cpp file ...
    juce::UndoManager* undoManagerAttachment;

    CustomButton powerButton, undoButton, redoButton;
    
    // Asserts an error if the object is coppied ...
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ButtonBox)
};
