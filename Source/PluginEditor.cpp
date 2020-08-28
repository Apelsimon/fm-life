/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"

#include "ParameterConfig.h"
#include "PluginEditor.h"

//==============================================================================
FmlifeAudioProcessorEditor::FmlifeAudioProcessorEditor (FmlifeAudioProcessor& p, juce::AudioProcessorValueTreeState& parameters)
    : AudioProcessorEditor (&p), audioProcessor (p), parameters(parameters), operatorComponent1(parameters, ParameterConfig::Id::OperatorRatio1), 
	operatorComponent2(parameters, ParameterConfig::Id::OperatorRatio2),
	operatorComponent3(parameters, ParameterConfig::Id::OperatorRatio3),
	operatorComponent4(parameters, ParameterConfig::Id::OperatorRatio4)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
	addAndMakeVisible(operatorComponent1);
	addAndMakeVisible(operatorComponent2);
	addAndMakeVisible(operatorComponent3);
	addAndMakeVisible(operatorComponent4);
}

FmlifeAudioProcessorEditor::~FmlifeAudioProcessorEditor()
{
}

//==============================================================================
void FmlifeAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void FmlifeAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	auto bounds = getLocalBounds();
	auto componentHeight = bounds.getHeight() / 4.f;
	operatorComponent1.setBounds(bounds.removeFromTop(componentHeight));
	operatorComponent2.setBounds(bounds.removeFromTop(componentHeight));
	operatorComponent3.setBounds(bounds.removeFromTop(componentHeight));
	operatorComponent4.setBounds(bounds.removeFromTop(componentHeight));
}
