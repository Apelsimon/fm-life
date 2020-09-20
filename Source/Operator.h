#pragma once

#include <JuceHeader.h>

#include "ParameterConfig.h"

#include <functional>

namespace jos
{

	class Operator
	{
	public:
		Operator(std::function<void()> envelopeDoneCb);

		void prepare(const juce::dsp::ProcessSpec& spec);
		float processSample();
		void setFrequency(float freq);
		void setPhaseMod(float fmod) { phaseMod = juce::jmap(fmod, -1.f, 1.f, -juce::MathConstants<float>::pi, juce::MathConstants<float>::pi); }
		void start();
		void stop();
		void reset();
		void setRatio(float r) { ratio = r; setFrequency(carrierFrequency); }
		float getRatio() const { return ratio; }
		void setAttack(float attack);
		void setDecay(float decay);
		void setSustain(float sustain);
		void setRelease(float release);
		void setOutputLevel(float level) { outputLevel = level; }
		void setWaveType(ParameterConfig::Values::WaveType type) { waveType = type; }

	private:

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

		void setEnvelopeParameter(Envelope& envelope, juce::ADSR::Parameters& params, float& paramToChange, float newValue);

		float carrierFrequency;
		float phaseMod;
		float ratio;
		float outputLevel;
		juce::dsp::Oscillator<float> osc;
		Envelope envelope;
		std::function<void()> onInactiveEnvelope;
		ParameterConfig::Values::WaveType waveType;
	};
}
