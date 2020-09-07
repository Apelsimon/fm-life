/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"

#include "ParameterConfig.h"
#include "PluginEditor.h"

//==============================================================================
FmlifeAudioProcessor::FmlifeAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
	parameters(*this, nullptr, "FmLifeParameters", createParameterLayout()),
	parameterListener(parameters),
	audioEngine(),
	midiMiessageCollector()
{
	audioEngine.registerParameterCallbacks(parameterListener);
}

FmlifeAudioProcessor::~FmlifeAudioProcessor()
{
}

//==============================================================================
const juce::String FmlifeAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FmlifeAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FmlifeAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FmlifeAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FmlifeAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FmlifeAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FmlifeAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FmlifeAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FmlifeAudioProcessor::getProgramName (int index)
{
    return {};
}

void FmlifeAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FmlifeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	juce::dsp::ProcessSpec spec{ sampleRate, (juce::uint32) samplesPerBlock, 2};
	audioEngine.prepare(spec, parameters);
	midiMiessageCollector.reset(sampleRate);
}

void FmlifeAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FmlifeAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void FmlifeAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

	midiMiessageCollector.removeNextBlockOfMessages(midiMessages, buffer.getNumSamples());
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    
	audioEngine.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool FmlifeAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FmlifeAudioProcessor::createEditor()
{
    return new FmlifeAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void FmlifeAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FmlifeAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}


void addFloatParametersToLayout(juce::AudioProcessorValueTreeState::ParameterLayout& parameterLayout, const ParameterConfig::Id::OperatorParamIds& paramIds, const ParameterConfig::Values::OperatorParamValues& paramValues)
{
	parameterLayout.add(std::make_unique<juce::AudioParameterFloat>(paramIds.ratioId, "RatioParam", paramValues.ratio.min, paramValues.ratio.max, paramValues.ratio.def));
	parameterLayout.add(std::make_unique<juce::AudioParameterFloat>(paramIds.attckId, "AttackParam", paramValues.attack.min, paramValues.attack.max, paramValues.attack.def));
	parameterLayout.add(std::make_unique<juce::AudioParameterFloat>(paramIds.decayId, "DecayParam", paramValues.decay.min, paramValues.decay.max, paramValues.decay.def));
	parameterLayout.add(std::make_unique<juce::AudioParameterFloat>(paramIds.sustainId, "SustainParam", paramValues.sustain.min, paramValues.sustain.max, paramValues.sustain.def));
	parameterLayout.add(std::make_unique<juce::AudioParameterFloat>(paramIds.releaseId, "ReleaseParam", paramValues.release.min, paramValues.release.max, paramValues.release.def));
	parameterLayout.add(std::make_unique<juce::AudioParameterFloat>(paramIds.feedbackId, "FeedbackParam", paramValues.feedback.min, paramValues.feedback.max, paramValues.feedback.def));
}

juce::AudioProcessorValueTreeState::ParameterLayout FmlifeAudioProcessor::createParameterLayout() const
{
	juce::AudioProcessorValueTreeState::ParameterLayout parameterLayout;
	
	addFloatParametersToLayout(parameterLayout, ParameterConfig::Id::Operator1, ParameterConfig::Values::Operator);
	addFloatParametersToLayout(parameterLayout, ParameterConfig::Id::Operator2, ParameterConfig::Values::Operator);
	addFloatParametersToLayout(parameterLayout, ParameterConfig::Id::Operator3, ParameterConfig::Values::Operator);
	addFloatParametersToLayout(parameterLayout, ParameterConfig::Id::Operator4, ParameterConfig::Values::Operator);

	parameterLayout.add(std::make_unique<juce::AudioParameterChoice>(ParameterConfig::Id::AlgorithmChoices, "AlgorithmChoices", ParameterConfig::Values::AlgorithmChoices, 0));

	return parameterLayout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FmlifeAudioProcessor();
}
