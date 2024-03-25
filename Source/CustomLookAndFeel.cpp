/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 30 Nov 2023 4:28:05pm
    Author:  Harvey Fretwell

  ==============================================================================
*/

#include "CustomLookAndFeel.h"
//#include "GraphBackground.h"

//==============================================================================

CustomLookAndFeel::CustomLookAndFeel()
    : juce::LookAndFeel_V4()
{
    setColour(juce::ResizableWindow::backgroundColourId, juce::Colours::dimgrey);

    setColour(juce::Slider::backgroundColourId, juce::Colours::transparentBlack);
    setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::white);
    setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::black);
    setColour(juce::Slider::thumbColourId, juce::Colours::black);
    setColour(juce::Slider::trackColourId, juce::Colours::white);

    /*setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::blue);
    setColour(juce::Slider::textBoxHighlightColourId, juce::Colours::orange);
    setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::purple);
    setColour(juce::Slider::textBoxTextColourId, juce::Colours::green);*/

    setColour(juce::BubbleComponent::backgroundColourId, juce::Colours::black);
    setColour(juce::BubbleComponent::outlineColourId, juce::Colours::white);

    setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    setColour(juce::TextButton::buttonOnColourId, juce::Colours::black);
    setColour(juce::TextButton::textColourOffId, juce::Colours::lightgrey);
    setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    
    /*setColour(GraphBackground::backgroundColourId, juce::Colour (16, 21, 24));
    setColour(GraphBackground::freqLineColourId, juce::Colour (49, 54, 57));
    setColour(GraphBackground::gainLineColourId, juce::Colour (41, 47, 47));
    setColour(GraphBackground::gain0LineColourId, juce::Colour (141, 149, 152));
    setColour(GraphBackground::labelColourId, juce::Colour (141, 149, 152));*/
}

CustomLookAndFeel::~CustomLookAndFeel()
{
}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g,
                                         int x, int y,
                                         int width, int height,
                                         float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                                         juce::Slider& slider)
{
    // Calculating dial tick angle ...
    float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));

    // Draw outline curve ...
    float diameterCurve = juce::jmin(width - 10.0f, height - 10.0f);
    float radiusCurve = diameterCurve / 2;
    float centreXCurve = static_cast<float>(x + width / 2);
    float centreYCurve = static_cast<float>(y + height / 2);
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
    auto thumbColour = findColour(juce::Slider::rotarySliderFillColourId);

    if (slider.isMouseOverOrDragging())
    {
        thumbColour = thumbColour.darker(0.5f);
    }

    g.setColour(findColour(juce::Slider::rotarySliderOutlineColourId));
    g.strokePath(curveBackground, juce::PathStrokeType(6.0f, juce::PathStrokeType::JointStyle::curved,
        juce::PathStrokeType::EndCapStyle::rounded));

    g.setColour(thumbColour);
    g.strokePath(curveForeground, juce::PathStrokeType(2.0f, juce::PathStrokeType::JointStyle::curved,
        juce::PathStrokeType::EndCapStyle::rounded));
}
									 
void CustomLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float /*maxSliderPos*/,
    const juce::Slider::SliderStyle, juce::Slider& slider)
{
    /* Defining the funtion to draw the linear sliders. */

    // Draw slider bar ...
    juce::Path sliderBackground; // path for static slider bar background.
    juce::Path sliderForeground; // path for moving slider bar.
    juce::Path sliderThumb; // path for the thumb

    // Calcuating dimentions when bar is vertical ...
    if (slider.getSliderStyle() == juce::Slider::SliderStyle::LinearVertical)
    {
        sliderBackground.startNewSubPath(static_cast<float>(x + width / 2), static_cast<float>(y));
        sliderBackground.lineTo(static_cast<float>(x + width / 2), static_cast<float>(y + height));

        sliderForeground.startNewSubPath(static_cast<float>(x + width / 2), minSliderPos);
        sliderForeground.lineTo(static_cast<float>(x + width / 2), sliderPos);

        sliderThumb.addEllipse(static_cast<float>(x - 6 + width / 2), sliderPos - 6.f, 12.0f, 12.0f);
    }

    // Calcuating dimentions when bar is horizontal ...
    else
    {
        sliderBackground.startNewSubPath(static_cast<float>(x), static_cast<float>(y + height / 2));
        sliderBackground.lineTo(static_cast<float>(x + width), static_cast<float>(y + height / 2));

        sliderForeground.startNewSubPath(minSliderPos, static_cast<float>(y + height / 2));
        sliderForeground.lineTo(sliderPos, static_cast<float>(y + height / 2));

        sliderThumb.addEllipse(sliderPos - 6.f, static_cast<float>(y - 6 + height / 2), 12.0f, 12.0f);
    }

    // Draw both paths with curved and rounded edges ...
    auto foreGColour = findColour(juce::Slider::trackColourId);

    if (slider.isMouseOverOrDragging())
    {
        foreGColour = foreGColour.darker(0.5f);
    }

    g.setColour(findColour(juce::Slider::thumbColourId));
    g.strokePath(sliderBackground, juce::PathStrokeType(6.0f, juce::PathStrokeType::JointStyle::curved,
        juce::PathStrokeType::EndCapStyle::rounded));

    g.setColour(foreGColour);
    g.strokePath(sliderForeground, juce::PathStrokeType(2.0f, juce::PathStrokeType::JointStyle::curved,
        juce::PathStrokeType::EndCapStyle::rounded));

    g.setColour(findColour(juce::Slider::thumbColourId));
    g.fillPath(sliderThumb);
}

void CustomLookAndFeel::drawBubble(juce::Graphics& g, juce::BubbleComponent& /*bubble*/,
    const juce::Point< float >& /*tip*/, const juce::Rectangle< float >& body)
{
    juce::Path outline;
    outline.addRoundedRectangle(body, 10.0f);

    g.setColour(findColour(juce::BubbleComponent::backgroundColourId));
    g.fillPath(outline);
}

void CustomLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button,
    const juce::Colour& /*backgroundColour*/,
    bool isMouseOverButton, bool isButtonDown)
{
    auto baseColour = findColour(juce::TextButton::buttonOnColourId);
    auto textColour = findColour(juce::TextButton::textColourOnId);

    // Set darker colour for button background if toggled off ...
    if (!button.getToggleState()) { baseColour = findColour(juce::TextButton::buttonColourId); };

    // Set darker text for button if toggled off and a clickling toggle state ...
    if (button.getClickingTogglesState()) { textColour = findColour(juce::TextButton::textColourOffId); };

    // Set contrasting colours for button background if pressed or hovered ...
    if (isButtonDown || isMouseOverButton) { baseColour = baseColour.contrasting(isButtonDown ? 0.2f : 0.1f); }

    // Find if any edges connect to other buttons ...
    auto flatOnLeft = button.isConnectedOnLeft();
    auto flatOnRight = button.isConnectedOnRight();
    auto flatOnTop = button.isConnectedOnTop();
    auto flatOnBottom = button.isConnectedOnBottom();

    // Find button width and height to get the best outline thickness ...
    auto width = (float)button.getWidth() - 1.0f;
    auto height = (float)button.getHeight() - 1.0f;

    if (width > 0 && height > 0)
    {
        auto cornerSize = juce::jmin(15.0f, juce::jmin(width, height) * 0.45f);
        auto lineThickness = cornerSize * 0.1f;
        auto halfThickness = lineThickness * 0.5f;

        juce::Path outline;
        // Add rounded edges only on sides that arent connected to other buttons ...
        outline.addRoundedRectangle(0.5f + halfThickness, 0.5f + halfThickness, width - lineThickness, height - lineThickness,
            cornerSize, cornerSize,
            !(flatOnLeft || flatOnTop),
            !(flatOnRight || flatOnTop),
            !(flatOnLeft || flatOnBottom),
            !(flatOnRight || flatOnBottom));

        // Set button background colour ...
        g.setColour(baseColour);
        g.fillPath(outline);

        // Set button outline and colour ...
        g.setColour(textColour);
        g.strokePath(outline, juce::PathStrokeType(lineThickness));
    }
}
