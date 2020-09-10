#pragma once

#include <JuceHeader.h>

#include "ComponentAttachment.h"
#include "ParameterConfig.h"

#include <vector>

class AlgorithmComponent  : public juce::Component
{
public:
    AlgorithmComponent(juce::AudioProcessorValueTreeState& parameters);
    ~AlgorithmComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

	void paintAlgorithms(juce::Graphics& g, ParameterConfig::Values::AlgorithmType currentSelection, const juce::Rectangle<int>& bounds);
	void initOperatorBoxes(juce::Rectangle<int> bounds);

	static constexpr auto AlgoritmBoxesWidthRatio = 0.3f;
	static constexpr auto NumOperatorBoxes = 16;

	jos::SliderAttachment algorithmChoices;
	std::vector<juce::Rectangle<float>> operatorBoxes;
	float boxWidth;
	float boxHeight;
	float widthPadding;
	float heightPadding;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlgorithmComponent)
};
