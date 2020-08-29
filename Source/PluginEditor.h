/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "AlgorithmComponent.h"
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
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FmlifeAudioProcessor& audioProcessor;
	juce::AudioProcessorValueTreeState& parameters;

	AlgorithmComponent algorithmComponent;
	OperatorComponent operatorComponent1;
	OperatorComponent operatorComponent2;
	OperatorComponent operatorComponent3;
	OperatorComponent operatorComponent4;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FmlifeAudioProcessorEditor)
};
