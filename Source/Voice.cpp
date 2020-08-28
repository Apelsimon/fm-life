#include "Voice.h"

#include "Sound.h"

namespace jos
{

	Voice::Voice() : op1([this]() { stopNote(0.f, false); }), op2([this]() { stopNote(0.f, false); })
{
		op1.setRatio(1.f);
		op2.setRatio(0.2f);
}

void Voice::prepare(const juce::dsp::ProcessSpec& spec)
{
	op1.prepare(spec);
	op2.prepare(spec);
}

bool Voice::canPlaySound(juce::SynthesiserSound* sound)
{
	return dynamic_cast<Sound*> (sound) != nullptr;
}

void Voice::startNote(int midiNoteNumber, float velocity,
	juce::SynthesiserSound*, int currentPitchWheelPosition)
{
	auto freq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);

	op1.setFrequency(freq);
	op2.setFrequency(freq);
	op1.start();
	op2.start();
	
	DBG("START NOTE");
}

void Voice::stopNote(float velocity, bool allowTailOff)
{
	if (allowTailOff)
	{
		op1.stop();
		op2.stop();
		DBG("TAIL OFF");
	}
	else
	{
		clearCurrentNote();
		op1.reset();
		op2.reset();
		DBG("STOP");
	}
}

void Voice::renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
	auto numChannels = outputBuffer.getNumChannels();

	for (auto sample = startSample; sample < (startSample + numSamples); ++sample)
	{
		auto out2 = op2.processSample();
		op1.setPhaseMod(out2);
		auto out = op1.processSample();

		for (auto channel = 0; channel < numChannels; ++channel)
		{
			outputBuffer.getWritePointer(channel)[sample] += out;
		}
	}
	
}

}
