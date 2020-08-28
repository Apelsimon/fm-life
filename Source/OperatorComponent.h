/*
  ==============================================================================

    OperatorComponent.h
    Created: 28 Aug 2020 4:38:23pm
    Author:  Apelsimon

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "ComponentAttachment.h"

//==============================================================================
/*
*/
class OperatorComponent  : public juce::Component
{
public:
    OperatorComponent(juce::AudioProcessorValueTreeState& parameters, const juce::String& ratioParamId);
    ~OperatorComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

	jos::SliderAttachment ratioAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OperatorComponent)
};
