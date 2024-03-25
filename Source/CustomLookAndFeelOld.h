/*
  ==============================================================================

    CustomLookAndFeelOld.h
    Author:  Harvey Fretwell

  ==============================================================================
*/

// Tell the preprocessor to only inlcude and compile this source file once ...
#pragma once

// Including the header to the JUCE library ...
#include <JuceHeader.h>

// Create a new class inheriting from the look and feel class ...
class CustomLookAndFeelOld : public juce::LookAndFeel_V4
{
    
/* Creating a custom look and feel class for our widgets. */
    
private:

public:
    // Declare public virtual functions to override ...
    // Sliders
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                          float rotaryStartAngle, float rotaryEndAngle, juce::Slider& Slider) override;
    
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                          float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle,
                          juce::Slider& Slider) override;

    // Popups
    void drawBubble(juce::Graphics& g, juce::BubbleComponent& bubble, const juce::Point< float >& tip,
        const juce::Rectangle< float >& body) override;
    
    // Buttons
    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                              bool isMouseOverButton, bool isButtonDown) override;

    // Creating public colour attributes to use on components ...
    juce::Colour foreground_colour1 = juce::Colour(14, 14, 14); // #0e0e0e
    juce::Colour foreground_colour2 = juce::Colour(255, 255, 255); // #ffffff
    juce::Colour background_colour = juce::Colour(45, 45, 47); // #2d2d2f
};
