/*
  ==============================================================================

    OperatorComponent.cpp
    Created: 28 Aug 2020 4:38:23pm
    Author:  Apelsimon

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OperatorComponent.h"

//==============================================================================
OperatorComponent::OperatorComponent(juce::AudioProcessorValueTreeState& parameters, const juce::String& ratioParamId) : ratioAttachment(parameters, ratioParamId)
{
	addAndMakeVisible(ratioAttachment());
	ratioAttachment().setTextValueSuffix(" ratio");
	ratioAttachment().setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
}

OperatorComponent::~OperatorComponent()
{
}

void OperatorComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId)); 
}

void OperatorComponent::resized()
{
	auto bounds = getLocalBounds();
	ratioAttachment().setBounds(bounds);
}
