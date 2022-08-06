/*
  ==============================================================================

    SliderBox.cpp
    Author:  Harvey Fretwell

  ==============================================================================
*/

#include "SliderBox.h"

//==============================================================================

SliderBox::SliderBox()
{
}

SliderBox::~SliderBox()
{
}

void SliderBox::create(CompressorAudioProcessor &processor)
{
    /* This is where the component is created. */

    // Creating labeled sliders ...
    ingainSlider.create(juce::Slider::LinearVertical, NAMEs::INGAIN, IDs::INGAIN, processor);
    outgainSlider.create(juce::Slider::LinearVertical, NAMEs::OUTGAIN, IDs::OUTGAIN, processor);
    
    // Making these components visible
    addAndMakeVisible(ingainSlider);
    addAndMakeVisible(outgainSlider);
}

void SliderBox::paint (juce::Graphics& g)
{
    /* This is where you paint any graphics onto the component. */

    // Set background as transparent so a global colour can be set in the editor.
    g.setColour(juce::Colours::transparentWhite);
    g.fillAll();
}

void SliderBox::resized()
{
    /* This is where the positions of any subcomponents are layed out in the component. */

    // Check if the components orientation is portrait ...
    auto isPortrait = getLocalBounds().getHeight() > getLocalBounds().getWidth();
    
    // Creating a flex container to hold all components ...
    juce::FlexBox flexBox;
    
    // Setting flex box configurations ...
    flexBox.flexDirection = isPortrait ? juce::FlexBox::Direction::column : juce::FlexBox::Direction::row;
    flexBox.justifyContent = juce::FlexBox::JustifyContent::spaceAround;    ingainSlider.slider.setSliderStyle(isPortrait ? juce::Slider::SliderStyle::LinearHorizontal : juce::Slider::SliderStyle::LinearVertical);
    
    // Set slider orientation ...
    outgainSlider.slider.setSliderStyle(isPortrait ? juce::Slider::SliderStyle::LinearHorizontal : juce::Slider::SliderStyle::LinearVertical);

    // Add these flex items to a flex box array container ...
    flexBox.items.add(juce::FlexItem(ingainSlider).withFlex(0, 1, isPortrait ? static_cast<float>(getHeight()) / 5.0f : static_cast<float>(getWidth()) / 5.0f)); // using static cast rather than '(float)variable' as is preffered
    flexBox.items.add(juce::FlexItem(outgainSlider).withFlex(0, 1, isPortrait ? static_cast<float>(getHeight()) / 5.0f : static_cast<float>(getWidth()) / 5.0f));

    // Set the flex box area ...
    flexBox.performLayout(getLocalBounds().toFloat());
}
