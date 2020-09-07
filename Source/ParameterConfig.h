#pragma once

#include <JuceHeader.h>

namespace ParameterConfig
{

namespace Id
{
	struct OperatorParamIds
	{
		juce::String ratioId;
		juce::String attckId;
		juce::String decayId;
		juce::String sustainId;
		juce::String releaseId;
		juce::String feedbackId;
	};

	static const OperatorParamIds Operator1{
		"Operator1RatioId",
		"Operator1AttackId",
		"Operator1DecayId",
		"Operator1SustainId",
		"Operator1ReleaseId",
		"Operator1FeedbackId"
	};
	static const OperatorParamIds Operator2{
		"Operator2RatioId",
		"Operator2AttackId",
		"Operator2DecayId",
		"Operator2SustainId",
		"Operator2ReleaseId",
		"Operator2FeedbackId"
	};
	static const OperatorParamIds Operator3{
		"Operator3RatioId",
		"Operator3AttackId",
		"Operator3DecayId",
		"Operator3SustainId",
		"Operator3ReleaseId",
		"Operator3FeedbackId"
	};
	static const OperatorParamIds Operator4{
		"Operator4RatioId",
		"Operator4AttackId",
		"Operator4DecayId",
		"Operator4SustainId",
		"Operator4ReleaseId",
		"Operator4FeedbackId"
	};

	static const juce::String AlgorithmChoices = "AlgorithmChoicesId";
}

namespace Values
{
	struct OperatorParamValues
	{
		struct Values
		{
			float min;
			float max;
			float def;
		};

		Values ratio;
		Values attack;
		Values decay;
		Values sustain;
		Values release;
		Values feedback;
	};

	static const OperatorParamValues Operator{
		{ 0.f, 10.f, 1.f },
		{ 0.f, 5000.f, 100.f },
		{ 0.f, 5000.f, 100.f },
		{ 0.f, 1000.f, 707.f },
		{ 0.f, 10000.f, 100.f },
		{ 0.f, 1.f, 0.f }
	};

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
