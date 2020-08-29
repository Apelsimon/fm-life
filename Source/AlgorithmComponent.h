#pragma once

#include <JuceHeader.h>

#include "ComponentAttachment.h"

class AlgorithmComponent  : public juce::Component
{
public:
    AlgorithmComponent(juce::AudioProcessorValueTreeState& parameters);
    ~AlgorithmComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
	jos::ComboBoxAttachment algorithmChoices;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlgorithmComponent)
};
