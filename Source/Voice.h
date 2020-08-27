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
		OscIndex,
		EnvIndex
	};

	struct Envelope : public juce::ADSR
	{
		//==============================================================================

		void process(const juce::dsp::ProcessContextReplacing<float>& context) noexcept
		{
			auto&& outBlock = context.getOutputBlock();

			auto numSamples = outBlock.getNumSamples();
			auto numChannels = outBlock.getNumChannels();

			for (auto sample = 0; sample < numSamples; ++sample)
			{
				auto env = getNextSample();

				for (auto channel = 0; channel < numChannels; ++channel)
					outBlock.getChannelPointer(channel)[sample] *= env;
			}
		}

		//==============================================================================
		void prepare(const juce::dsp::ProcessSpec& spec)
		{
			setSampleRate(spec.sampleRate);
		}
	};

	juce::dsp::ProcessorChain<juce::dsp::Oscillator<float>, Envelope> processorChain;
	juce::HeapBlock<char> heapBlock;
	juce::dsp::AudioBlock<float> tempBlock;
};

}
