#pragma once

#include <JuceHeader.h>

#include "Operator.h"

namespace jos
{

class ParameterListener;

class Voice : public juce::SynthesiserVoice
{
public:
	Voice();

	void prepare(const juce::dsp::ProcessSpec& spec, juce::AudioProcessorValueTreeState& parameters);
	void registerParameterCallbacks(jos::ParameterListener& paramListener);

	bool canPlaySound(juce::SynthesiserSound* sound) override;
	void startNote(int midiNoteNumber, float velocity,
		juce::SynthesiserSound*, int currentPitchWheelPosition) override;
	void stopNote(float velocity, bool allowTailOff) override;
	void pitchWheelMoved(int) override {}
	void controllerMoved(int, int) override {}
	void renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;

private:

	enum AlgorithmChoice
	{
		I = 0,
		II,
		III,
		IV,
		V,
		VI,
		VII,
		VIII
	};

	float processSample();

	std::function<void()> stopNoteCb;
	jos::Operator op1;
	jos::Operator op2;
	jos::Operator op3;
	jos::Operator op4;
	AlgorithmChoice algorithmChoice;
};

}
