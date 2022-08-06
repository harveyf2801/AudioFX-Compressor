/*
  ==============================================================================

    CustomLookAndFeel.h
    Author:  Harvey Fretwell

  ==============================================================================
*/

// Tell the preprocessor to only inlcude and compile this source file once ...
#pragma once

// Including the header to the JUCE library ...
#include <JuceHeader.h>

// Create a new class inheriting from the look and feel class ...
class CustomLookAndFeel : public juce::LookAndFeel_V4
{
    
/* Creating a custom look and feel class for our widgets. */
    
private:
    // Create a random integer number generator ...
    juce::Random randomInt = juce::Random::getSystemRandom();

public:
    // Declare public virtual functions to override ...
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                          float rotaryStartAngle, float rotaryEndAngle, juce::Slider& Slider) override;
    
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                          float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle,
                          juce::Slider& Slider) override;
    
    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                                             bool isMouseOverButton, bool isButtonDown) override;
    
    // Creating public colour attributes to use on components ...
    juce::Colour foreground_colour1 = juce::Colour (randomInt.nextInt(256), // selects a random colour
                                         randomInt.nextInt(256), randomInt.nextInt(256)).brighter();
    
    juce::Colour foreground_colour2 = foreground_colour1.contrasting(1.0f); // find a colour which contrasts best with foreground colour 1.
    // ( colour will be either black if light, or white if dark )
    
    juce::Colour background_colour = juce::Colour::contrasting(foreground_colour1, foreground_colour2); // find a colour which is contrasting with foreground colour 1 and 2.
};
