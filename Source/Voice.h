#pragma once

#include <JuceHeader.h>


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
	enum
	{
		OscIndex
	};

	bool playNote;
	juce::dsp::ProcessorChain<juce::dsp::Oscillator<float>> processorChain;
	juce::HeapBlock<char> heapBlock;
	juce::dsp::AudioBlock<float> tempBlock;
};

}
