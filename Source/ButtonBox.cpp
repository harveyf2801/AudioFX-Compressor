/*
  ==============================================================================

    ButtonBox.cpp
    Author:  Harvey Fretwell

  ==============================================================================
*/

#include "ButtonBox.h"

//==============================================================================

ButtonBox::ButtonBox()
{
}

ButtonBox::~ButtonBox()
{
}

void ButtonBox::create(CompressorAudioProcessor &processor)
{
    /* This is where the component is created. */
    
    // Storing a pointer to the undomanager ...
    juce::UndoManager* undoManagerAttachment = &processor.undoManager;

    // Create power button using custom class ...
    powerButton.create(NAMEs::POWER, IDs::POWER, processor, true);
    undoButton.create(NAMEs::UNDO, "", processor, false);
    redoButton.create(NAMEs::REDO, "", processor, false);
    
    // Set lambda functions for undo and redo buttons
    undoButton.button.onClick = [this, undoManagerAttachment] { undoManagerAttachment->undo(); };
    redoButton.button.onClick = [this, undoManagerAttachment] { undoManagerAttachment->redo(); };

    // Make all components visible
    addAndMakeVisible(powerButton);
    addAndMakeVisible(undoButton);
    addAndMakeVisible(redoButton);
}

void ButtonBox::paint (juce::Graphics& g)
{
    /* This is where you paint any graphics onto the component. */

    // Set background as transparent so a global colour can be set in the editor.
    g.setColour(juce::Colours::transparentWhite);
    g.fillAll();
}

void ButtonBox::resized()
{
    /* This is where the positions of any subcomponents are layed out in the component. */

    // Laying out components using remove functions ...
    auto area = getLocalBounds();
    
    auto margin = 10;
    auto powerHeight = 50;
    auto undoRedoHeight = 40;
    
    area.removeFromTop(area.getHeight()/2.5);
    
    powerButton.setBounds(area.removeFromTop(powerHeight).reduced(margin));
    undoButton.setBounds(area.removeFromLeft(area.getWidth()/2).removeFromTop(undoRedoHeight).reduced(margin));
    redoButton.setBounds(area.removeFromTop(undoRedoHeight).reduced(margin));
}
