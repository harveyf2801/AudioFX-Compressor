/*
  ==============================================================================

    DialBox.cpp
    Author:  Harvey Fretwell

  ==============================================================================
*/

#include "DialBox.h"

//==============================================================================

DialBox::DialBox()
{
}

DialBox::~DialBox()
{
}

void DialBox::create(CompressorAudioProcessor &processor)
{
    /* This is where the component is created. */

    // Creating labeled sliders ...
    attackSlider.create(juce::Slider::RotaryHorizontalVerticalDrag, NAMEs::ATTACK, IDs::ATTACK, processor, " ms");
    releaseSlider.create(juce::Slider::RotaryHorizontalVerticalDrag, NAMEs::RELEASE, IDs::RELEASE, processor, " ms");
    thresholdSlider.create(juce::Slider::RotaryHorizontalVerticalDrag, NAMEs::THRESHOLD, IDs::THRESHOLD, processor);
    ratioSlider.create(juce::Slider::RotaryHorizontalVerticalDrag, NAMEs::RATIO, IDs::RATIO, processor, " : 1");
    kneeSlider.create(juce::Slider::RotaryHorizontalVerticalDrag, NAMEs::KNEE, IDs::KNEE, processor);
    mixSlider.create(juce::Slider::RotaryHorizontalVerticalDrag, NAMEs::MIX, IDs::MIX, processor);
    
    // Make these components visible ...
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(releaseSlider);
    addAndMakeVisible(thresholdSlider);
    addAndMakeVisible(ratioSlider);
    addAndMakeVisible(kneeSlider);
    addAndMakeVisible(mixSlider);
}

void DialBox::paint (juce::Graphics& g)
{
    /* This is where you paint any graphics onto the component. */

    // Set background as transparent so a global colour can be set in the editor.
    g.setColour(juce::Colours::transparentWhite);
    g.fillAll();
}

void DialBox::resized()
{
    /* This is where the positions of any subcomponents are layed out in the component. */

    // Check if the components orientation is portrait ...
    auto isPortrait = getLocalBounds().getHeight() > getLocalBounds().getWidth();

    // Creating a flex container to hold all components ...
    juce::FlexBox flexBox;
    
    // Setting flex box configurations ...
    flexBox.flexDirection = isPortrait ? juce::FlexBox::Direction::column : juce::FlexBox::Direction::row;
    flexBox.flexWrap = juce::FlexBox::Wrap::wrap;
    flexBox.justifyContent = juce::FlexBox::JustifyContent::center;
    
    // Add these flex items to a flex box array container ...
    flexBox.items.add(juce::FlexItem(attackSlider).withMinHeight(50.0f).withMinWidth(50.0f).withFlex(1));
    flexBox.items.add(juce::FlexItem(releaseSlider).withMinHeight(50.0f).withMinWidth(50.0f).withFlex(1));
    flexBox.items.add(juce::FlexItem(thresholdSlider).withMinHeight(50.0f).withMinWidth(50.0f).withFlex(1));
    flexBox.items.add(juce::FlexItem(ratioSlider).withMinHeight(50.0f).withMinWidth(50.0f).withFlex(1));
    flexBox.items.add(juce::FlexItem(kneeSlider).withMinHeight(50.0f).withMinWidth(50.0f).withFlex(1));
    flexBox.items.add(juce::FlexItem(mixSlider).withMinHeight(50.0f).withMinWidth(50.0f).withFlex(1));

    // Set the flex box area ...
    flexBox.performLayout(getLocalBounds().toFloat());
}
