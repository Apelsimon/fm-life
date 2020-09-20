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
		juce::String outputId;
		juce::String feedbackId;
		juce::String wavetypeChoicesId;
	};

	static const OperatorParamIds Operator1{
		"Operator1RatioId",
		"Operator1AttackId",
		"Operator1DecayId",
		"Operator1SustainId",
		"Operator1ReleaseId",
		"Operator1OutputId",
		"Operator1FeedbackId",
		"Operator1WaveTypeChoicesId"
	};
	static const OperatorParamIds Operator2{
		"Operator2RatioId",
		"Operator2AttackId",
		"Operator2DecayId",
		"Operator2SustainId",
		"Operator2ReleaseId",
		"Operator2OutputId",
		"Operator2FeedbackId",
		"Operator2WaveTypeChoicesId"
	};
	static const OperatorParamIds Operator3{
		"Operator3RatioId",
		"Operator3AttackId",
		"Operator3DecayId",
		"Operator3SustainId",
		"Operator3ReleaseId",
		"Operator3OutputId",
		"Operator3FeedbackId",
		"Operator3WaveTypeChoicesId"
	};
	static const OperatorParamIds Operator4{
		"Operator4RatioId",
		"Operator4AttackId",
		"Operator4DecayId",
		"Operator4SustainId",
		"Operator4ReleaseId",
		"Operator4OutputId",
		"Operator4FeedbackId",
		"Operator4WaveTypeChoicesId"
	};

	static const juce::String AlgorithmChoices = "AlgorithmChoicesId";
	static const juce::String RandomizeParameterButtons = "RandomizeParameterButtonsId";
}

namespace Values
{
	template<typename T>
	struct ValueLimits
	{
		T min;
		T max;
		T def;
	};

	struct OperatorParamValues
	{
		ValueLimits<float> ratio;
		ValueLimits<float> attack;
		ValueLimits<float> decay;
		ValueLimits<float> sustain;
		ValueLimits<float> release;
		ValueLimits<float> output;
		ValueLimits<float> feedback;
	};

	static const OperatorParamValues Operator{
		{ 0.f, 10.f, 1.f },
		{ 0.f, 5000.f, 100.f },
		{ 0.f, 5000.f, 100.f },
		{ 0.f, 1000.f, 707.f },
		{ 0.f, 10000.f, 100.f },
		{ 0.f, 0.99f, 0.75f },
		{ 0.f, 1.f, 0.f }
	};

	enum AlgorithmType
	{
		I = 1,
		II,
		III,
		IV,
		V,
		VI,
		VII,
		VIII
	};

	static const ValueLimits<int> AlgorithmChocies{ 1, 8, 1 };

	enum WaveType
	{
		Sine = 0,
		Square,
		Triangle,
		Saw
	};

	static const juce::StringArray WaveTypeChoices{
		"Sine",
		"Square",
		"Triangle",
		"Saw",
	};
}

}
