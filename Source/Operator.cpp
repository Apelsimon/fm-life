#include "Operator.h"

namespace jos
{

Operator::Operator(std::function<void()> envelopeDoneCb) : carrierFrequency(0.f), phaseMod(0.f), ratio(0.f), processorChain(), heapBlock(), tempBlock(), onInactiveEnvelope(envelopeDoneCb)
{
	auto& osc = processorChain.get<OscIndex>();
	osc.initialise([this](float input) { return std::sin(input + 4.f * phaseMod); });
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

}
