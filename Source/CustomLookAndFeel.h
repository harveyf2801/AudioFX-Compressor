/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 30 Nov 2023 4:28:05pm
    Author:  Harvey Fretwell

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    //==============================================================================
    
    // Constructor / Destructor
    CustomLookAndFeel();
    virtual ~CustomLookAndFeel();

    // Declare public virtual functions to override ...
    // Sliders
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, juce::Slider& Slider) override;

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle,
        juce::Slider& slider) override;

    // Popups
    void drawBubble(juce::Graphics& g, juce::BubbleComponent& bubble, const juce::Point< float >& tip,
        const juce::Rectangle< float >& body) override;

    // Buttons
    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
        bool isMouseOverButton, bool isButtonDown) override;

private:
    
    //==============================================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomLookAndFeel);
};
