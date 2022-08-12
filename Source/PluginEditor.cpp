/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.
          The editor provides a GUI and visualisations for the user.

  ==============================================================================
*/

// Including the headers for the editor/GUI and the audio processing ...
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

CompressorAudioProcessorEditor::CompressorAudioProcessorEditor (CompressorAudioProcessor& p)
: AudioProcessorEditor (&p), audioProcessor (p)
{
    /* This is the constructor and will run when this object is created. 
          Initialisation and set up of the window and GUI objects */

    // Creating components and displaying them ...
    buttonBox.create(audioProcessor);
    dialBox.create(audioProcessor);
    sliderBox.create(audioProcessor);
    
    addAndMakeVisible(buttonBox);
    addAndMakeVisible(dialBox);
    addAndMakeVisible(sliderBox);

    // Setting the overal size and limits of the editor window ...
    juce::Rectangle<int> screenSize = juce::Desktop::getInstance().getDisplays().getMainDisplay().userArea;
    int width = screenSize.getWidth();
    int height = screenSize.getHeight();

    setSize(width*0.6, height*0.4); // ( width, height )

    setResizable(true, true); // ( allow host DAW to resize, allow user to resize )
    setResizeLimits(600, 500, width, height); // (min w, min h, max w, max h)
}

CompressorAudioProcessorEditor::~CompressorAudioProcessorEditor()
{
    /* This is the destructor and will run when this object is deleted. */
}

//==============================================================================

void CompressorAudioProcessorEditor::paint (juce::Graphics& g)
{
    /* This is where you paint any graphics onto the editor. */

    // Setting the editor background colour ...
    g.setColour(customLookAndFeel.background_colour);
    g.fillAll();
}

void CompressorAudioProcessorEditor::resized()
{
    /* This is where the positions of any subcomponents are layed out in the editor. */
    
    // Creating a juce grid object ...
    juce::Grid grid;
    
    using Track = juce::Grid::TrackInfo;
    using Px = juce::Grid::Px;
    using Fr = juce::Grid::Fr;
    
    // Creating gaps between each panel
    grid.rowGap = Px(20);
    grid.columnGap = Px(20);
    
    // Creating sizing ratios for each panel
    grid.templateRows = { Track(Fr(1)) };
    grid.templateColumns = { Track(Fr(1)), Track(Fr(2)), Track(Fr(2)) };
    
    // Auto placement algorith ...
    grid.autoRows = Track(Fr(1));
    grid.autoColumns = Track(Fr(3));
    grid.autoFlow = juce::Grid::AutoFlow::row;
    
    // Add each item / panel to grid
    grid.items = { juce::GridItem(buttonBox), juce::GridItem(dialBox), juce::GridItem(sliderBox) };
    
    grid.performLayout (getLocalBounds());
}
