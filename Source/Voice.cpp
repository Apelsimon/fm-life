#include "Voice.h"

#include "ParameterConfig.h"
#include "ParameterListener.h"
#include "Sound.h"

namespace jos
{

Voice::Voice() : stopNoteCb([this]() { stopNote(0.f, false); }), 
	op1(stopNoteCb), op2(stopNoteCb), op3(stopNoteCb), op4(stopNoteCb),
	algorithmChoice(I)
{
}

void Voice::prepare(const juce::dsp::ProcessSpec& spec, juce::AudioProcessorValueTreeState& parameters)
{
	op1.prepare(spec);
	op2.prepare(spec);
	op3.prepare(spec);
	op4.prepare(spec);

	op1.setRatio(*parameters.getRawParameterValue(ParameterConfig::Id::OperatorRatio1));
	op2.setRatio(*parameters.getRawParameterValue(ParameterConfig::Id::OperatorRatio2));
	op3.setRatio(*parameters.getRawParameterValue(ParameterConfig::Id::OperatorRatio3));
	op4.setRatio(*parameters.getRawParameterValue(ParameterConfig::Id::OperatorRatio4));
	algorithmChoice = static_cast<AlgorithmChoice>(static_cast<int>(*parameters.getRawParameterValue(ParameterConfig::Id::AlgorithmChoices)));
}

void Voice::registerParameterCallbacks(jos::ParameterListener& paramListener)
{
	paramListener.registerCallback(ParameterConfig::Id::OperatorRatio1, [this](float newValue) { op1.setRatio(newValue); });
	paramListener.registerCallback(ParameterConfig::Id::OperatorRatio2, [this](float newValue) { op2.setRatio(newValue); });
	paramListener.registerCallback(ParameterConfig::Id::OperatorRatio3, [this](float newValue) { op3.setRatio(newValue); });
	paramListener.registerCallback(ParameterConfig::Id::OperatorRatio4, [this](float newValue) { op4.setRatio(newValue); });
	paramListener.registerCallback(ParameterConfig::Id::AlgorithmChoices, [this](float newValue) { DBG("CHOICE: " << newValue); 
		algorithmChoice = static_cast<AlgorithmChoice>(static_cast<int>(newValue)); });
}

bool Voice::canPlaySound(juce::SynthesiserSound* sound)
{
	return dynamic_cast<Sound*> (sound) != nullptr;
}

void Voice::startNote(int midiNoteNumber, float velocity,
	juce::SynthesiserSound*, int currentPitchWheelPosition)
{
	auto freq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);

	DBG("START NOTE");

	op1.setFrequency(freq);
	op2.setFrequency(freq);
	op3.setFrequency(freq);
	op4.setFrequency(freq);
	op1.start();
	op2.start();
	op3.start();
	op4.start();
}

void Voice::stopNote(float velocity, bool allowTailOff)
{
	if (allowTailOff)
	{
		op1.stop();
		op2.stop();
		op3.stop();
		op4.stop();
		DBG("TAIL OFF");
	}
	else
	{
		clearCurrentNote();
		op1.reset();
		op2.reset();
		op3.reset();
		op4.reset();
		DBG("STOP");
	}
}

void Voice::renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
	auto numChannels = outputBuffer.getNumChannels();

	for (auto sample = startSample; sample < (startSample + numSamples); ++sample)
	{
		const auto out = processSample();

		for (auto channel = 0; channel < numChannels; ++channel)
		{
			outputBuffer.getWritePointer(channel)[sample] += out;
		}
	}	
}

float Voice::processSample()
{
	switch (algorithmChoice)
	{
	case I:
	{
		auto out4 = op4.processSample();
		
		op3.setPhaseMod(out4);
		auto out3 = op3.processSample();

		op2.setPhaseMod(out3);
		auto out2 = op2.processSample();

		op1.setPhaseMod(out2);
		return op1.processSample();
	}
	case II:
	{
		auto out4 = op4.processSample();
		auto out3 = op3.processSample();

		op2.setPhaseMod(0.5f * (out3 + out4));
		auto out2 = op2.processSample();

		op1.setPhaseMod(out2);
		return op1.processSample();
	}
	case III:
	{
		auto out4 = op4.processSample();

		auto out3 = op3.processSample();

		op2.setPhaseMod(out3);
		auto out2 = op2.processSample();

		op1.setPhaseMod(0.5 * (out2 + out4));
		return op1.processSample();
	}
	case IV:
	{
		auto out4 = op4.processSample();

		op3.setPhaseMod(out4);
		auto out3 = op3.processSample();

		auto out2 = op2.processSample();

		op1.setPhaseMod(0.5 * (out2 + out3));
		return op1.processSample();
	}
	case V:
	{
		auto out4 = op4.processSample();
		auto out3 = op3.processSample();

		op2.setPhaseMod(out4);
		op1.setPhaseMod(out3);

		return 0.5 * (op1.processSample() + op2.processSample());
	}
	case VI:
	{
		auto out4 = op4.processSample();

		op3.setPhaseMod(out4);
		op2.setPhaseMod(out4);
		op1.setPhaseMod(out4);

		return (op1.processSample() + op2.processSample() + op3.processSample()) / 3.f;
	}
	case VII:
	{
		auto out4 = op4.processSample();

		op3.setPhaseMod(out4);

		return (op1.processSample() + op2.processSample() + op3.processSample()) / 3.f;
	}
	case VIII:
	{
		return (op1.processSample() + op2.processSample() + op3.processSample() + op4.processSample()) / 4.f;
	}
	default:
		return 0.f;
	}
}

}
