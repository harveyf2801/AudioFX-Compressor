/*
  ==============================================================================

    RandomLookAndFeel.cpp
    Author:  Harvey Fretwell

  ==============================================================================
*/

#include "RandomLookAndFeel.h"

void RandomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                         float sliderPos, float rotaryStartAngle,
                                         float rotaryEndAngle, juce::Slider& Slider)
{
    /* Defining the funtion to draw the rotary sliders. */

    // Calculating dial dimentions ...
    float diameter = juce::jmin(width-20.0f, height-20.0f);
    float radius = diameter / 2;
    float centreX = x + width / 2;
    float centreY = y + height / 2;
    float rX = centreX - radius;
    float rY = centreY - radius;

    // Create dial rectangular area ...
    juce::Rectangle<float> dialArea(rX, rY, diameter, diameter);

    // Draw dial fill ...
    g.setColour(foreground_colour1);
    g.fillEllipse(dialArea);

    // Draw dial tick ...
    g.setColour(foreground_colour2);
    juce::Path dialTick; // create a juce path.
    dialTick.addRoundedRectangle(0.0f, -radius, radius/10.0f, radius * 0.33f, 2.0);
    
    // Calculating dial tick angle ...
    float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));
    
    // Fill in the dial tick path ...
    g.fillPath(dialTick, juce::AffineTransform::rotation(angle).translated(centreX, centreY));

    // Draw outline border ...
    float diameterBorder = juce::jmin(width - 10.0f, height - 10.0f);
    float radiusBorder = diameterBorder / 2;
    float centreXBorder = x + width / 2;
    float centreYBorder = y + height / 2;
    float rXBorder = centreXBorder - radiusBorder;
    float rYBorder = centreYBorder - radiusBorder;
    
    // Create dial border rectangular area ...
    juce::Rectangle<float> dialBorderArea(rXBorder, rYBorder, diameterBorder, diameterBorder);

    g.setColour(foreground_colour2);
    juce::Path border; // create a juce path.
    border.addArc(dialBorderArea.getX(), dialBorderArea.getY(), dialBorderArea.getWidth(),
                  dialBorderArea.getHeight(), rotaryStartAngle, rotaryEndAngle, true);
    juce::PathStrokeType strokeType(2.0f, juce::PathStrokeType::JointStyle::curved,
                                    juce::PathStrokeType::EndCapStyle::rounded); // create path stroke type as curved and rounded
    g.strokePath(border, strokeType); // draw stroke path
}

void RandomLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                                         float sliderPos,float minSliderPos, float maxSliderPos,
                                         const juce::Slider::SliderStyle, juce::Slider& Slider)
{
    /* Defining the funtion to draw the linear sliders. */

    // Draw slider bar ...
    juce::Path sliderBackground; // path for static slider bar background.
    juce::Path sliderForeground; // path for moving slider bar.

    // Calcuating dimentions when bar is vertical ...
    if(Slider.getSliderStyle() == juce::Slider::SliderStyle::LinearVertical)
    {
        sliderBackground.startNewSubPath(x + width / 2, y);
        sliderBackground.lineTo(x + width / 2, y + height);
        
        sliderForeground.startNewSubPath(x + width / 2, minSliderPos);
        sliderForeground.lineTo(x + width / 2, sliderPos);
    }
    
    // Calcuating dimentions when bar is horizontal ...
    else
    {
        sliderBackground.startNewSubPath(x, y + height / 2);
        sliderBackground.lineTo(x + width, y + height / 2);
        
        sliderForeground.startNewSubPath(minSliderPos, y + height / 2);
        sliderForeground.lineTo(sliderPos, y + height / 2);
    }
    
    // Draw both paths with curved and rounded edges ...
    g.setColour(foreground_colour1);
    g.strokePath(sliderBackground, juce::PathStrokeType(6.0f, juce::PathStrokeType::JointStyle::curved,
                                    juce::PathStrokeType::EndCapStyle::rounded));
    g.setColour(foreground_colour2);
    g.strokePath(sliderForeground, juce::PathStrokeType(2.0, juce::PathStrokeType::JointStyle::curved,
                                                        juce::PathStrokeType::EndCapStyle::rounded));
}

void RandomLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                                              bool isMouseOverButton, bool isButtonDown)
{
    auto baseColour = foreground_colour1;
    
    // Set darker colour for button background if toggled off ...
    if (!button.getToggleState()) { baseColour = background_colour; };
    // Set darker text for button if toggled off and a clickling toggle state ...
    if (button.getClickingTogglesState()) { button.setColour(juce::TextButton::textColourOffId, juce::Colours::slategrey); };

    // Set contrasting colours for button background if pressed or hovered ...
    if (isButtonDown || isMouseOverButton) { baseColour = baseColour.contrasting (isButtonDown ? 0.2f : 0.1f); }
    
    // Find if any edges connect to other buttons ...
    auto flatOnLeft = button.isConnectedOnLeft();
    auto flatOnRight = button.isConnectedOnRight();
    auto flatOnTop = button.isConnectedOnTop();
    auto flatOnBottom = button.isConnectedOnBottom();
    
    // Find button width and height to get the best outline thickness ...
    auto width  = (float)button.getWidth()  - 1.0f;
    auto height = (float)button.getHeight() - 1.0f;
    
    if(width > 0 && height > 0)
    {
        auto cornerSize = juce::jmin (15.0f, juce::jmin (width, height) * 0.45f);
        auto lineThickness = cornerSize    * 0.1f;
        auto halfThickness = lineThickness * 0.5f;
        
        juce::Path outline;
        // Add rounded edges only on sides that arent connected to other buttons ...
        outline.addRoundedRectangle (0.5f + halfThickness, 0.5f + halfThickness, width - lineThickness, height - lineThickness,
                                     cornerSize, cornerSize,
                                     ! (flatOnLeft  || flatOnTop),
                                     ! (flatOnRight || flatOnTop),
                                     ! (flatOnLeft  || flatOnBottom),
                                     ! (flatOnRight || flatOnBottom));
        
        // Set button background colour ...
        g.setColour(baseColour);
        g.fillPath(outline);
        
        // Set button outline and colour ...
        g.setColour(foreground_colour2);
        g.strokePath(outline, juce::PathStrokeType(lineThickness));
    }
}
