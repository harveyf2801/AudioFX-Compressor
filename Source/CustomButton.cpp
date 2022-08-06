/*
  ==============================================================================

    CustomButton.cpp
    Author:  Harvey Fretwell

  ==============================================================================
*/

#include "CustomButton.h"

//==============================================================================
CustomButton::CustomButton()
{
}

CustomButton::~CustomButton()
{
}

void CustomButton::create(juce::String aName, juce::String aId, CompressorAudioProcessor &processor, bool toggle)
{
    /* This is where the component is created. */

    // Set button style ...
    button.setClickingTogglesState(toggle); // ( if button toggles and stays on state on click )
    // Set label text ...
    button.setButtonText(aName);
    // Apply custom look and feel ...
    button.setLookAndFeel(&customLookAndFeel); // ( juce::LookAndFeel )
    
    // Draw the objects on screen ...
    addAndMakeVisible(&button);
    
    if (aId != "")
    {
        // Sync value tree parameters to GUI objects using attatchments ...
        attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>
        (processor.valueTree, aId, button);
    }
}

void CustomButton::paint (juce::Graphics& g)
{
    /* This is where you paint any graphics onto the component. */

    // Set background as transparent so a global colour can be set in the editor.
    g.setColour(juce::Colours::transparentWhite);
    g.fillAll();
}

void CustomButton::resized()
{
    button.setBounds(getLocalBounds());
}
