#pragma once

#include <JuceHeader.h>

#include "ComponentAttachment.h"
#include "ParameterConfig.h"
#include "WaveTypeComboBoxLookAndFeel.h"

class OperatorComponent  : public juce::Component
{
public:

    OperatorComponent(juce::AudioProcessorValueTreeState& parameters, const ParameterConfig::Id::OperatorParamIds& paramIds);
    ~OperatorComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
	void initAndPublishSlider(juce::Slider& slider, const juce::String& suffix);

	juce::Label operatorLabel;
	jos::SliderAttachment ratioAttachment;
	jos::SliderAttachment attackAttachment;
	jos::SliderAttachment decayAttachment;
	jos::SliderAttachment sustainAttachment;
	jos::SliderAttachment releaseAttachment;
	jos::SliderAttachment feedbackAttachment;

	WaveTypeComboBoxLookAndFeel waveTypeComboBoxLookAndFeel;
	juce::ComboBox waveTypeComboBox;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OperatorComponent)
};
