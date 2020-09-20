#include "Operator.h"

namespace jos
{

Operator::Operator(std::function<void()> envelopeDoneCb) : carrierFrequency(0.f), phaseMod(0.f), ratio(0.f), outputLevel(0.f), osc(), envelope(), onInactiveEnvelope(envelopeDoneCb)
{
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
	osc.prepare(spec);
	envelope.prepare(spec);
}

float Operator::processSample()
{
	auto out = 0.f;

	if (envelope.isActive())
	{
		out = outputLevel * osc.processSample(0.f) * envelope.getNextSample();

		if (!envelope.isActive())
		{
			envelope.reset();
			if(onInactiveEnvelope) onInactiveEnvelope();
		}
			
	}

	return out;
}

void Operator::setFrequency(float freq)
{
	carrierFrequency = freq;
	osc.setFrequency(carrierFrequency * ratio, true);
}

void Operator::start()
{
	envelope.noteOn();
}

void Operator::stop()
{
	envelope.noteOff();
}

void Operator::reset()
{
	envelope.reset();
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

	auto params = envelope.getParameters();
	setEnvelopeParameter(envelope, params, params.attack, attack);
}

void Operator::setDecay(float decay)
{
	decay = decay / 1000.f;

	auto params = envelope.getParameters();
	setEnvelopeParameter(envelope, params, params.decay, decay);
}

void Operator::setSustain(float sustain)
{
	sustain = sustain / 1000.f;

	auto params = envelope.getParameters();
	setEnvelopeParameter(envelope, params, params.sustain, sustain);
}

void Operator::setRelease(float release)
{
	release = release / 1000.f;

	auto params = envelope.getParameters();
	setEnvelopeParameter(envelope, params, params.release, release);
}

}
