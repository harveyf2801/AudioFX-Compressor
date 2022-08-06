/*
  ==============================================================================

    LabeledSlider.cpp
    Author:  Harvey Fretwell

  ==============================================================================
*/

#include "LabeledSlider.h"

//==============================================================================
LabeledSlider::LabeledSlider()
{
}

LabeledSlider::~LabeledSlider()
{
}

void LabeledSlider::create(juce::Slider::SliderStyle aStyle, juce::String aName, juce::String aId, CompressorAudioProcessor &processor, juce::String aSuffix)
{
    /* This is where the component is created. */

    // Set slider style ...
    slider.setSliderStyle(aStyle); // ( juce::Slider::SliderStyle )
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 1005); // ( juce::Slider::TextEntryBoxPosition, readOnly, w, h )
    slider.setPopupDisplayEnabled(true, false, nullptr); // ( show on drag, show on hover, parent component pointer )
    
    // Set label text ...
    label.setText(aName, juce::dontSendNotification); // ( juce::String, juce::Notifications )
    // Justify text to center bottom ...
    label.setJustificationType(juce::Justification::centredBottom);
    
    // Apply custom look and feel ...
    slider.setLookAndFeel(&customLookAndFeel); // ( juce::LookAndFeel )
    
    // Set text suffix if given ...
    if(aSuffix != "") { slider.setTextValueSuffix(aSuffix); } // ( juce::String )
    
    // Draw the objects on screen ...
    addAndMakeVisible(&slider);
    addAndMakeVisible(&label);
    
    // Sync value tree parameters to GUI objects using attatchments ...
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
    (processor.valueTree, aId, slider);
}

void LabeledSlider::paint (juce::Graphics& g)
{
    /* This is where you paint any graphics onto the component. */

    // Set background as transparent so a global colour can be set in the editor.
    g.setColour(juce::Colours::transparentWhite);
    g.fillAll();
}

void LabeledSlider::resized()
{
    /* This is where the positions of any subcomponents are layed out in the component. */
    
    auto area = getLocalBounds().toFloat();
    
    // Creating a flex container to hold all components ...
    juce::FlexBox flexBox;
    
    // Setting flex box configurations ...
    flexBox.flexWrap = juce::FlexBox::Wrap::wrap;
    flexBox.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    
    // Add these flex items to a flex box array container ...
    flexBox.items.add(juce::FlexItem(area.getWidth(), area.getHeight()*0.25, label));
    flexBox.items.add(juce::FlexItem(area.getWidth(), area.getHeight()*0.75, slider));
    
    // Set the flex box area ...
    flexBox.performLayout(area);
}

