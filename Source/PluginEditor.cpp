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
    : AudioProcessorEditor (&p), audioProcessor (p), parameters(parameters), 
	randomizeParametersButton1(),
	randomizeParametersButton2(),
	presetComponent(parameters),
	algorithmComponent(parameters),
	operatorComponent1(parameters, ParameterConfig::Id::Operator1),
	operatorComponent2(parameters, ParameterConfig::Id::Operator2),
	operatorComponent3(parameters, ParameterConfig::Id::Operator3),
	operatorComponent4(parameters, ParameterConfig::Id::Operator4)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1200, 600);
	addAndMakeVisible(randomizeParametersButton1);
	addAndMakeVisible(randomizeParametersButton2);
	addAndMakeVisible(presetComponent);
	addAndMakeVisible(algorithmComponent);
	addAndMakeVisible(operatorComponent1);
	addAndMakeVisible(operatorComponent2);
	addAndMakeVisible(operatorComponent3);
	addAndMakeVisible(operatorComponent4);

	randomizeParametersButton1.setButtonText("Don't push");
	randomizeParametersButton1.onClick = [this]() { randomizeParameters1(); };
	randomizeParametersButton2.setButtonText("Don't push this either");
	randomizeParametersButton2.onClick = [this]() { randomizeParameters2(); };
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

	auto topPanelBounds = bounds.removeFromTop(bounds.getHeight() * 0.2f);
	randomizeParametersButton1.setBounds(topPanelBounds.removeFromLeft(topPanelBounds.getWidth() * 0.2f));
	randomizeParametersButton2.setBounds(topPanelBounds.removeFromLeft(topPanelBounds.getWidth() * 0.2f));
	presetComponent.setBounds(topPanelBounds.removeFromLeft(topPanelBounds.getWidth() * 0.2f));
	algorithmComponent.setBounds(topPanelBounds);

	auto componentHeight = bounds.getHeight() / 4.f;
	operatorComponent1.setBounds(bounds.removeFromTop(componentHeight));
	operatorComponent2.setBounds(bounds.removeFromTop(componentHeight));
	operatorComponent3.setBounds(bounds.removeFromTop(componentHeight));
	operatorComponent4.setBounds(bounds.removeFromTop(componentHeight));
}

void FmlifeAudioProcessorEditor::randomizeParameters1()
{
	algorithmComponent.randomize();
	operatorComponent1.randomize1();
	operatorComponent2.randomize1();
	operatorComponent3.randomize1();
	operatorComponent4.randomize1();
}

void FmlifeAudioProcessorEditor::randomizeParameters2()
{
	operatorComponent1.randomize2();
	operatorComponent2.randomize2();
	operatorComponent3.randomize2();
	operatorComponent4.randomize2();
}
