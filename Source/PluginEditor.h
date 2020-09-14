/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "AlgorithmComponent.h"
#include "ComponentAttachment.h"
#include "OperatorComponent.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class FmlifeAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FmlifeAudioProcessorEditor (FmlifeAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~FmlifeAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
	void randomizeParameters1();
	void randomizeParameters2();

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FmlifeAudioProcessor& audioProcessor;
	juce::AudioProcessorValueTreeState& parameters;

	juce::Component dummyComponent;
	juce::TextButton randomizeParametersButton1;
	juce::TextButton randomizeParametersButton2;
	AlgorithmComponent algorithmComponent;
	OperatorComponent operatorComponent1;
	OperatorComponent operatorComponent2;
	OperatorComponent operatorComponent3;
	OperatorComponent operatorComponent4;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FmlifeAudioProcessorEditor)
};
