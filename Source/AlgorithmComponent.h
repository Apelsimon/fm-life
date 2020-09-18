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

	void randomize();

private:
	class OperatorBoxesComponent : public juce::Component
	{
		static constexpr auto NumOperatorBoxes = 16;
	public:
		OperatorBoxesComponent() : operatorBoxes(NumOperatorBoxes) {}

		void paint(juce::Graphics&) override;
		void resized() override;

		void initOperatorBoxes(juce::Rectangle<int> bounds);

		std::vector<juce::Rectangle<float>> operatorBoxes;
		ParameterConfig::Values::AlgorithmType currentAlgorithmChoice;
		float boxWidth;
		float boxHeight;
		float widthPadding;
		float heightPadding;
	};

	jos::SliderAttachment algorithmChoices;
	OperatorBoxesComponent operatorBoxes;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlgorithmComponent)
};
