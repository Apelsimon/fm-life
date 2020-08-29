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
OperatorComponent::OperatorComponent(juce::AudioProcessorValueTreeState& parameters, const ParameterConfig::Id::OperatorParamIds& paramIds) :
	ratioAttachment(parameters, paramIds.ratioId),
	attackAttachment(parameters, paramIds.attckId),
    decayAttachment(parameters, paramIds.decayId),
	sustainAttachment(parameters, paramIds.sustainId),
	releaseAttachment(parameters, paramIds.releaseId)
{
	initAndPublishSlider(ratioAttachment(), " ratio");
	initAndPublishSlider(attackAttachment(), " attack");
	initAndPublishSlider(decayAttachment(), " decay");
	initAndPublishSlider(sustainAttachment(), " sustain");
	initAndPublishSlider(releaseAttachment(), " release");
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
	auto sliderWidth = bounds.getWidth() / 5.f;

	ratioAttachment().setBounds(bounds.removeFromLeft(sliderWidth));
	attackAttachment().setBounds(bounds.removeFromLeft(sliderWidth));
	decayAttachment().setBounds(bounds.removeFromLeft(sliderWidth));
	sustainAttachment().setBounds(bounds.removeFromLeft(sliderWidth));
	releaseAttachment().setBounds(bounds.removeFromLeft(sliderWidth));
}

void OperatorComponent::initAndPublishSlider(juce::Slider& slider, const juce::String& suffix)
{
	addAndMakeVisible(slider);
	slider.setTextValueSuffix(suffix);
	slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
}