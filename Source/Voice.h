#pragma once

#include <JuceHeader.h>

#include "Operator.h"

namespace jos
{

class Voice : public juce::SynthesiserVoice
{
public:
	Voice();

	void prepare(const juce::dsp::ProcessSpec& spec);

	bool canPlaySound(juce::SynthesiserSound* sound) override;
	void startNote(int midiNoteNumber, float velocity,
		juce::SynthesiserSound*, int currentPitchWheelPosition) override;
	void stopNote(float velocity, bool allowTailOff) override;
	void pitchWheelMoved(int) override {}
	void controllerMoved(int, int) override {}
	void renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;

private:
	jos::Operator op1;
	jos::Operator op2;
};

}
