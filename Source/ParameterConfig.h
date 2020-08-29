#pragma once

#include <JuceHeader.h>

namespace ParameterConfig
{

namespace Id
{
	static const juce::String OperatorRatio1 = "OperatorRatio1Id";
	static const juce::String OperatorRatio2 = "OperatorRatio2Id";
	static const juce::String OperatorRatio3 = "OperatorRatio3Id";
	static const juce::String OperatorRatio4 = "OperatorRatio4Id";
	static const juce::String AlgorithmChoices = "AlgorithmChoicesId";
}

namespace Values
{
	static const juce::StringArray AlgorithmChoices{
		"I",
		"II",
		"III",
		"IV",
		"V",
		"VI",
		"VII",
		"VIII"
	};
}

}
