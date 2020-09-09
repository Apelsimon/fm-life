#include "Operator.h"

namespace jos
{

Operator::Operator(std::function<void()> envelopeDoneCb) : carrierFrequency(0.f), phaseMod(0.f), ratio(0.f), processorChain(), heapBlock(), tempBlock(), onInactiveEnvelope(envelopeDoneCb)
{
	auto& osc = processorChain.get<OscIndex>();
	osc.initialise([this](float input) { 

		auto phase = input + 4.f * phaseMod;
		while (phase > juce::MathConstants<float>::pi) phase -= juce::MathConstants<float>::twoPi;
		
		switch (waveType)
		{
		case ParameterConfig::Values::WaveType::Sine:
		{
			return std::sin(phase);
		}
		case ParameterConfig::Values::WaveType::Square:
		{
			return phase < 0.f ? -1.f : 1.f;
		}
		case ParameterConfig::Values::WaveType::Triangle:
		{
			if (phase < -juce::MathConstants<float>::halfPi)    return 2.f + phase * 2.f / juce::MathConstants<float>::pi;
			else if(phase < juce::MathConstants<float>::halfPi) return phase * -2.f / juce::MathConstants<float>::pi;
			else                                                return phase * 2.f / juce::MathConstants<float>::pi - 2.f;
		}
		case ParameterConfig::Values::WaveType::Saw:
		{
			return (phase < 0.f ? 1.f : -1.f) + phase / juce::MathConstants<float>::pi;
		}
		default:
			return 0.f;
			break;
		}
	});
}

void Operator::prepare(const juce::dsp::ProcessSpec& spec)
{
	tempBlock = juce::dsp::AudioBlock<float>{ heapBlock, spec.numChannels, spec.maximumBlockSize };
	processorChain.prepare(spec);
}

void Operator::renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
	auto& adsr = processorChain.get<EnvIndex>();

	if (adsr.isActive())
	{
		auto block = tempBlock.getSubBlock(0, (size_t)numSamples);
		block.clear();
		juce::dsp::ProcessContextReplacing<float> context(block);
		processorChain.process(context);

		juce::dsp::AudioBlock<float>(outputBuffer)
			.getSubBlock((size_t)startSample, (size_t)numSamples)
			.add(tempBlock);

		if (!adsr.isActive()) onInactiveEnvelope();
	}
}

float Operator::processSample()
{
	auto& adsr = processorChain.get<EnvIndex>();
	auto& osc = processorChain.get<OscIndex>();

	auto out = 0.f;

	if (adsr.isActive())
	{
		out = osc.processSample(0.f) * adsr.getNextSample();

		if (!adsr.isActive()) onInactiveEnvelope();
	}

	return out;
}

void Operator::setFrequency(float freq)
{
	auto& osc = processorChain.get<OscIndex>();
	carrierFrequency = freq;
	osc.setFrequency(carrierFrequency * ratio, true);
}

void Operator::start()
{
	auto& adsr = processorChain.get<EnvIndex>();
	adsr.noteOn();
}

void Operator::stop()
{
	auto& adsr = processorChain.get<EnvIndex>();
	adsr.noteOff();
}

void Operator::reset()
{
	auto& adsr = processorChain.get<EnvIndex>();
	adsr.reset();
}

void Operator::setEnvelopeParameter(Envelope& envelope, juce::ADSR::Parameters& params, float& paramToChange, float newValue)
{
	if (newValue != paramToChange)
	{
		paramToChange = newValue;
		envelope.setParameters(params);
	}
}

void Operator::setAttack(float attack)
{
	attack = attack / 1000.f;

	auto& adsr = processorChain.get<EnvIndex>();
	auto params = adsr.getParameters();
	setEnvelopeParameter(adsr, params, params.attack, attack);
}

void Operator::setDecay(float decay)
{
	decay = decay / 1000.f;

	auto& adsr = processorChain.get<EnvIndex>();
	auto params = adsr.getParameters();
	setEnvelopeParameter(adsr, params, params.decay, decay);
}

void Operator::setSustain(float sustain)
{
	sustain = sustain / 1000.f;

	auto& adsr = processorChain.get<EnvIndex>();
	auto params = adsr.getParameters();
	setEnvelopeParameter(adsr, params, params.sustain, sustain);
}

void Operator::setRelease(float release)
{
	release = release / 1000.f;

	auto& adsr = processorChain.get<EnvIndex>();
	auto params = adsr.getParameters();
	setEnvelopeParameter(adsr, params, params.release, release);
}

}
