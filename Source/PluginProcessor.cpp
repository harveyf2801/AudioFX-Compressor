/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.
             The processor handles audio i/o, midi i/o and processing logic.

  ==============================================================================
*/

// Including the headers for the editor/GUI and the audio processing ...
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CompressorAudioProcessor::CompressorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                        #if ! JucePlugin_IsSynth
                            // Compile if the plugin is not a midi effect or a synth ...
                            .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                        #endif
                        // Compile if the plugin is not a midi effect ...
                        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                         // Initialising the JUCE ValueTreeState object ...
                       ), valueTree(*this, &undoManager, "PARAMETERS", createParameters())
                        // ( pointer to this object, undo manager, ID of value tree, parameter layout )
#endif
{
    /* This is the constructor and will run when this object is created. */
    
    // Hold the pointer to each parameter ready to dereference in the process block ...
    powerPtr = valueTree.getRawParameterValue(IDs::POWER);
    ingainPtr = valueTree.getRawParameterValue(IDs::INGAIN);
    outgainPtr = valueTree.getRawParameterValue(IDs::OUTGAIN);
    attackPtr = valueTree.getRawParameterValue(IDs::ATTACK);
    releasePtr = valueTree.getRawParameterValue(IDs::RELEASE);
    thresholdPtr = valueTree.getRawParameterValue(IDs::THRESHOLD);
    ratioPtr = valueTree.getRawParameterValue(IDs::RATIO);
    kneePtr = valueTree.getRawParameterValue(IDs::KNEE);
    mixPtr = valueTree.getRawParameterValue(IDs::MIX);
}

CompressorAudioProcessor::~CompressorAudioProcessor()
{
    /* This is the destructor and will run when this object is deleted. */

}

//==============================================================================

/* Get and Set methods for our AudioProcessor class. */

const juce::String CompressorAudioProcessor::getName() const
{
    /* Get method: Returns the name of the plugin. */
    return JucePlugin_Name;
}

bool CompressorAudioProcessor::acceptsMidi() const
{
    /* Get method: Returns boolean true/false if the plugin needs midi input. */
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CompressorAudioProcessor::producesMidi() const
{
    /* Get method: Returns boolean true/false if the plugin needs midi output. */
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CompressorAudioProcessor::isMidiEffect() const
{
    /* Get method: Returns boolean true/false if the plugin is a midi effect. */
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CompressorAudioProcessor::getTailLengthSeconds() const
{
    /* Get method: Returns the length of the processors tail in seconds. */
    return 0.0;
}

//==============================================================================

/* Handling preset functions in the DAW. */

int CompressorAudioProcessor::getNumPrograms()
{
    /* Get method: Returns number of preset programs the processor supports. */
    // ( shouldn't change over it's lifetime or be less than 1 )

    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CompressorAudioProcessor::getCurrentProgram()
{
    /* Get method: Returns the number of currently active program. */
    return 0;
}

void CompressorAudioProcessor::setCurrentProgram (int index)
{
    /* Set method: Called by the host to change the current program. */

}

const juce::String CompressorAudioProcessor::getProgramName (int index)
{
    /* Get method: Returns the name of a given program. */
    return {};
}

void CompressorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    /* Set/Replace method: Called by the host to rename a program. */

}

//==============================================================================

void CompressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    /* This is where any pre-playback initialisation is done. */
    
    // Adding the correct number of compressors needed to match the number of channels ...
    for (auto channel = 0; channel < getTotalNumOutputChannels(); channel++)
    {
        multichannelCompressor.add(CompressorEffect());
    }
}

void CompressorAudioProcessor::releaseResources()
{
    /* When playback stops, this as an opportunity to free up any spare memory, etc. */

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CompressorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    /* Checking the channel configurations are compatible. */

  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // Check if layout/channel configuration is either stereo or mono ...
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    // Check if the input layout matches the output layout ...
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void CompressorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // Where each block/buffer of audio and midi is processed ...

    // Protect against denormalized / 'close-to-zero' numbers and underflow errors ...
    juce::ScopedNoDenormals noDenormals;

    // Getting integer number of i/o channels ...
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Dereference the pointer for each parameter ...
    bool powerValue = *powerPtr;
    float ingainValue = *ingainPtr;
    float outgainValue = *outgainPtr;
    float attackValue = *attackPtr;
    float releaseValue = *releasePtr;
    float thresholdValue = *thresholdPtr;
    float ratioValue = *ratioPtr;
    float kneeValue = *kneePtr;
    float mixValue = *mixPtr;
    
    // Convert attack and release values ...
    auto newAttackValue = 1 - std::exp(((1/getSampleRate()) * -2.2f) / (attackValue / 1000.0f));
    auto newReleaseValue = 1 - std::exp(((1/getSampleRate()) * -2.2f) / (releaseValue / 1000.0f));
    

    // Clear any output channels that didn't contain input data ...
    // ( in case we have more outputs than inputs and avoid feedback however, 
    // if we always overwrite all the output channels, this doesn't need to be included )
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Itterate through each block/buffer of audio samples ...
    // ( make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state )
    
    // Itterate through each channel ...
    for (auto channel = 0; channel < totalNumInputChannels; ++channel)
    {
        // Sync both buffers together ...
        auto* channelData = buffer.getWritePointer(channel);
        CompressorEffect* comp = &multichannelCompressor.getReference(channel);
        
        // Itterate through each sample ...
        for (auto i = 0; i < buffer.getNumSamples(); ++i)
        {
            
            auto dryOut = channelData[i];
            
            // Compress signal if power is on ...
            if (powerValue == true)
            {
                
                // Setting dry signal and applying pre gain ...
                dryOut = dryOut * ingainValue;
                
                // Send the sample to the compressor to be compressed ...
                auto wetOut = comp->processSample(dryOut, thresholdValue, ratioValue, newAttackValue, newReleaseValue, kneeValue);

                // Apply post gain ...
                dryOut *= outgainValue;
                wetOut *= outgainValue;
                
                // Apply mix value using linear equation ...
                channelData[i] = (wetOut * mixValue) + (dryOut * (1 - mixValue));
            }
            // Bypass signal if power is off ...
            else { channelData[i] = dryOut; }
        }
    }
}

//==============================================================================

bool CompressorAudioProcessor::hasEditor() const
{
    /* Get method: Returns boolean true/false if the editor is used in this plugin. */
    return true; // (change to false if a GUI/editor is not used in the plugin)
}

juce::AudioProcessorEditor* CompressorAudioProcessor::createEditor()
{
    /* Creates and returns a pointer to the editor object. */
    return new CompressorAudioProcessorEditor (*this);
}

//==============================================================================

void CompressorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    /* Get method (with no return): Used to store parameters in the memory block.
        Could be done either as raw data, or using the XML or ValueTree classes
            as intermediaries to make it easy to save and load complex data. */
    
    // Store value state tree parameters as XML file ...
    auto stateTree = valueTree.copyState();
    std::unique_ptr<juce::XmlElement> xml(stateTree.createXml());
    copyXmlToBinary(*xml, destData);
}

void CompressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    /* Used to to restore parameters from the memory block,
        whose contents will have been created by the getStateInformation() call. */
    
    // Load in previously saved parameters from XML file ...
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr && xmlState->hasTagName(valueTree.state.getType())) {
        valueTree.replaceState(juce::ValueTree::fromXml(*xmlState));
    }
}

//==============================================================================

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    /* Creates new instances of the plugin. */
    return new CompressorAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout CompressorAudioProcessor::createParameters()
{
    /* Creates a vector to hold all the parameter names and ranges */

    // Create a vector of unique pointers to JUCE RangedAudioParameter objects ...
    // ( unique_ptr from the standard library disposes of objects and deallocates memory when they go out of scope )
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;

    // Adding/pushing parameters onto the vector (ID, Name, min, max, default) ...
    
    // Boolean parameters ...
    parameters.push_back(std::make_unique<juce::AudioParameterBool>(IDs::POWER, NAMEs::POWER, false));

    // Gain to dB parameters ...
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(IDs::INGAIN, NAMEs::INGAIN,
        juce::NormalisableRange<float>(0.00f, 1.00f, 0.01f), 1.0f, // min = -100dB, max = 0dB.
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) {return juce::String(juce::Decibels::gainToDecibels(value), 1) + " dB"; },
        [](juce::String text) {return juce::Decibels::decibelsToGain(text.getFloatValue()); }));
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(IDs::OUTGAIN, NAMEs::OUTGAIN,
        juce::NormalisableRange<float>(0.00f, 1.00f, 0.01f), 1.0f, // min = -100dB, max = 0dB.
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) {return juce::String(juce::Decibels::gainToDecibels(value), 1) + " dB"; },
        [](juce::String text) {return juce::Decibels::decibelsToGain(text.getFloatValue()); }));

    // ( ID, Name, Range, Default, Label, Category, 
    //      Lambda function to set GUI display value as string in decibels with suffix dB,
    //          Lambda function to get normalised gain conversion to set as value tree parameter value )

    // Float parametes
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(IDs::THRESHOLD, NAMEs::THRESHOLD,
        juce::NormalisableRange<float>(-60.0f, 0.0f, 0.01f), 0.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(IDs::ATTACK, NAMEs::ATTACK, juce::NormalisableRange<float>(0.01f, 500.0, 0.01f), 100.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(IDs::RELEASE, NAMEs::RELEASE, juce::NormalisableRange<float>(0.01f, 2000.0f, 0.01f), 500.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(IDs::RATIO, NAMEs::RATIO, juce::NormalisableRange<float>(1.0f, 20.0f, 0.01f), 2.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(IDs::KNEE, NAMEs::KNEE, juce::NormalisableRange<float>(0.0f, 30.0f, 0.01f), 0.0f));

    // Mix to percentage parameters ...
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(IDs::MIX, NAMEs::MIX,
        juce::NormalisableRange<float>(0.00f, 1.00f, 0.0001f), 1.0f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) {return juce::String(value * 100.00f, 1) + "%"; },
        [](juce::String text) {return text.getFloatValue() / 100.00f; }));
    //      Lambda function to set GUI display value as string in percentage with suffix %,
    //          Lambda function to get percentage conversion to set as value tree parameter value )

    return { parameters.begin(), parameters.end() };
}
