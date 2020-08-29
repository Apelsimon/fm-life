#include <JuceHeader.h>

#include "AlgorithmComponent.h"
#include "ParameterConfig.h"

//==============================================================================
AlgorithmComponent::AlgorithmComponent(juce::AudioProcessorValueTreeState& parameters) :
	algorithmChoices(parameters, ParameterConfig::Id::AlgorithmChoices)
{
	addAndMakeVisible(algorithmChoices());
	for (auto i = 0; i < ParameterConfig::Values::AlgorithmChoices.size(); ++i)
	{
		algorithmChoices().addItem(ParameterConfig::Values::AlgorithmChoices[i], i + 1);
	}
	algorithmChoices().setSelectedId(1);
}

AlgorithmComponent::~AlgorithmComponent()
{
}

void AlgorithmComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
}

void AlgorithmComponent::resized()
{
	auto bounds = getLocalBounds();
	algorithmChoices().setBounds(bounds);
}
