/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Author:  Harvey Fretwell

  ==============================================================================
*/

#include "CustomLookAndFeel.h"

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                         float sliderPos, float rotaryStartAngle,
                                         float rotaryEndAngle, juce::Slider& Slider)
{
    /* Defining the funtion to draw the rotary sliders. */
    
    // Calculating dial tick angle ...
    float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));

    // Draw outline curve ...
    float diameterCurve = juce::jmin(width - 10.0f, height - 10.0f);
    float radiusCurve = diameterCurve / 2;
    float centreXCurve = x + width / 2;
    float centreYCurve = y + height / 2;
    float rXCurve = centreXCurve - radiusCurve;
    float rYCurve = centreYCurve - radiusCurve;
    
    // Create dial rectangular area ...
    juce::Rectangle<float> dialCurveArea(rXCurve, rYCurve, diameterCurve, diameterCurve);

    juce::Path curveBackground; // create a juce path.
    juce::Path curveForeground; // create a juce path.

    curveBackground.addArc(dialCurveArea.getX(), dialCurveArea.getY(), dialCurveArea.getWidth(),
                  dialCurveArea.getHeight(), rotaryStartAngle, rotaryEndAngle, true);

    curveForeground.addArc(dialCurveArea.getX(), dialCurveArea.getY(), dialCurveArea.getWidth(),
        dialCurveArea.getHeight(), rotaryStartAngle, angle, true);

    // Draw both paths with curved and rounded edges ...
    g.setColour(foreground_colour1);
    g.strokePath(curveBackground, juce::PathStrokeType(6.0f, juce::PathStrokeType::JointStyle::curved,
                                                             juce::PathStrokeType::EndCapStyle::rounded));
    g.setColour(foreground_colour2);
    g.strokePath(curveForeground, juce::PathStrokeType(2.0f, juce::PathStrokeType::JointStyle::curved,
                                                            juce::PathStrokeType::EndCapStyle::rounded));
}

void CustomLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                                         float sliderPos, float minSliderPos, float maxSliderPos,
                                         const juce::Slider::SliderStyle, juce::Slider& Slider)
{
    /* Defining the funtion to draw the linear sliders. */

    // Draw slider bar ...
    juce::Path sliderBackground; // path for static slider bar background.
    juce::Path sliderForeground; // path for moving slider bar.
    juce::Path sliderThumb; // path for the thumb

    // Calcuating dimentions when bar is vertical ...
    if (Slider.getSliderStyle() == juce::Slider::SliderStyle::LinearVertical)
    {
        sliderBackground.startNewSubPath(x + width / 2, y);
        sliderBackground.lineTo(x + width / 2, y + height);

        sliderForeground.startNewSubPath(x + width / 2, minSliderPos);
        sliderForeground.lineTo(x + width / 2, sliderPos);

        sliderThumb.addEllipse(x-6 + width / 2, sliderPos-6, 12.0f, 12.0f);
    }

    // Calcuating dimentions when bar is horizontal ...
    else
    {
        sliderBackground.startNewSubPath(x, y + height / 2);
        sliderBackground.lineTo(x + width, y + height / 2);

        sliderForeground.startNewSubPath(minSliderPos, y + height / 2);
        sliderForeground.lineTo(sliderPos, y + height / 2);

        sliderThumb.addEllipse(sliderPos-6, y-6 + height / 2, 12.0f, 12.0f);
    }

    // Draw both paths with curved and rounded edges ...
    g.setColour(foreground_colour1);
    g.strokePath(sliderBackground, juce::PathStrokeType(6.0f, juce::PathStrokeType::JointStyle::curved,
        juce::PathStrokeType::EndCapStyle::rounded));

    g.setColour(foreground_colour2);
    g.strokePath(sliderForeground, juce::PathStrokeType(2.0f, juce::PathStrokeType::JointStyle::curved,
        juce::PathStrokeType::EndCapStyle::rounded));

    g.setColour(foreground_colour1);
    g.fillPath(sliderThumb);
}

void CustomLookAndFeel::drawBubble(juce::Graphics& g, juce::BubbleComponent& bubble,
                                   const juce::Point< float >& tip, const juce::Rectangle< float >& body)
{
    juce::Path outline;
    outline.addRoundedRectangle(body, 10.0f);

    g.setColour(foreground_colour1);
    g.fillPath(outline);
}

void CustomLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button,
                                             const juce::Colour& backgroundColour,
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
