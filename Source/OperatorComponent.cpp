/*
  ==============================================================================

    OperatorComponent.cpp
    Created: 28 Aug 2020 4:38:23pm
    Author:  Apelsimon

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OperatorComponent.h"

juce::String getLabelTextFromParameterId(const ParameterConfig::Id::OperatorParamIds& paramIds)
{
	if (paramIds.ratioId == ParameterConfig::Id::Operator1.ratioId) return "Operator1";
	else if (paramIds.ratioId == ParameterConfig::Id::Operator2.ratioId) return "Operator2";
	else if (paramIds.ratioId == ParameterConfig::Id::Operator3.ratioId) return "Operator3";
	else if (paramIds.ratioId == ParameterConfig::Id::Operator4.ratioId) return "Operator4";

	return "";
}

//==============================================================================
OperatorComponent::OperatorComponent(juce::AudioProcessorValueTreeState& parameters, const ParameterConfig::Id::OperatorParamIds& paramIds) :
	operatorLabel("operatorLabel", getLabelTextFromParameterId(paramIds)),
	ratioAttachment(parameters, paramIds.ratioId),
	attackAttachment(parameters, paramIds.attckId),
    decayAttachment(parameters, paramIds.decayId),
	sustainAttachment(parameters, paramIds.sustainId),
	releaseAttachment(parameters, paramIds.releaseId)
{
	addAndMakeVisible(operatorLabel);
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

	auto bounds = getLocalBounds();
	g.setColour(juce::Colours::beige);
	g.drawHorizontalLine(bounds.getY(), bounds.getTopLeft().x, bounds.getTopRight().x);
}

void OperatorComponent::resized()
{
	constexpr auto Padding = 10.f;

	auto bounds = getLocalBounds().reduced(Padding);
	const auto childComponentWidth = bounds.getWidth() / 6.f;

	operatorLabel.setBounds(bounds.removeFromLeft(childComponentWidth));
	ratioAttachment().setBounds(bounds.removeFromLeft(childComponentWidth));
	attackAttachment().setBounds(bounds.removeFromLeft(childComponentWidth));
	decayAttachment().setBounds(bounds.removeFromLeft(childComponentWidth));
	sustainAttachment().setBounds(bounds.removeFromLeft(childComponentWidth));
	releaseAttachment().setBounds(bounds.removeFromLeft(childComponentWidth));
}

void OperatorComponent::initAndPublishSlider(juce::Slider& slider, const juce::String& suffix)
{
	addAndMakeVisible(slider);
	slider.setTextValueSuffix(suffix);
	slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
	slider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 100.f, 20.f);
}