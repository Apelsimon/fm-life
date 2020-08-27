#include "Voice.h"

#include "Sound.h"

namespace jos
{

Voice::Voice() : processorChain(), heapBlock(), tempBlock()
{
	auto& osc = processorChain.get<OscIndex>();
	osc.initialise([](float input) { return std::sin(input); }, 128);
}

void Voice::prepare(const juce::dsp::ProcessSpec& spec)
{
	tempBlock = juce::dsp::AudioBlock<float>{ heapBlock, spec.numChannels, spec.maximumBlockSize };
	processorChain.prepare(spec);
}

bool Voice::canPlaySound(juce::SynthesiserSound* sound)
{
	return dynamic_cast<Sound*> (sound) != nullptr;
}

void Voice::startNote(int midiNoteNumber, float velocity,
	juce::SynthesiserSound*, int currentPitchWheelPosition)
{
	auto freq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);

	auto& osc = processorChain.get<OscIndex>();
	osc.setFrequency(freq, true);

	auto& adsr = processorChain.get<EnvIndex>();
	adsr.noteOn();
}

void Voice::stopNote(float velocity, bool allowTailOff)
{
	auto& adsr = processorChain.get<EnvIndex>();

	if (allowTailOff)
	{
		adsr.noteOff();
	}
	else
	{
		clearCurrentNote();
		adsr.reset();
	}
}

void Voice::renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
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
		
		if (!adsr.isActive()) stopNote(0.f, false);
	}
}

}
