#include "Voice.h"

#include "ParameterListener.h"
#include "Sound.h"

namespace jos
{

Voice::Voice() : stopNoteCb([this]() { stopNote(0.f, false); }), 
	op1(stopNoteCb), op2(stopNoteCb), op3(stopNoteCb), op4(stopNoteCb),
	operator4Feedback(0), algorithmChoice(ParameterConfig::Values::AlgorithmType::I)
{
}

void Voice::prepare(const juce::dsp::ProcessSpec& spec, juce::AudioProcessorValueTreeState& parameters)
{
	op1.prepare(spec);
	op2.prepare(spec);
	op3.prepare(spec);
	op4.prepare(spec);

	initOperatorParameters(op1, parameters, ParameterConfig::Id::Operator1);
	initOperatorParameters(op2, parameters, ParameterConfig::Id::Operator2);
	initOperatorParameters(op3, parameters, ParameterConfig::Id::Operator3);
	initOperatorParameters(op4, parameters, ParameterConfig::Id::Operator4);

	operator4Feedback = *parameters.getRawParameterValue(ParameterConfig::Id::Operator4.feedbackId);

	algorithmChoice = static_cast<ParameterConfig::Values::AlgorithmType>(static_cast<int>(*parameters.getRawParameterValue(ParameterConfig::Id::AlgorithmChoices)));
}

void Voice::registerParameterCallbacks(jos::ParameterListener& paramListener)
{
	registerOperatorCallbacks(paramListener, ParameterConfig::Id::Operator1, op1);
	registerOperatorCallbacks(paramListener, ParameterConfig::Id::Operator2, op2);
	registerOperatorCallbacks(paramListener, ParameterConfig::Id::Operator3, op3);
	registerOperatorCallbacks(paramListener, ParameterConfig::Id::Operator4, op4);

	paramListener.registerCallback(ParameterConfig::Id::Operator4.feedbackId, [this](float newValue) { DBG("op 4 feedback: " << newValue); operator4Feedback = newValue; });

	paramListener.registerCallback(ParameterConfig::Id::AlgorithmChoices, [this](float newValue) { DBG("CHOICE: " << newValue); 
		algorithmChoice = static_cast<ParameterConfig::Values::AlgorithmType>(static_cast<int>(newValue)); });
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

void Voice::initOperatorParameters(jos::Operator& op, juce::AudioProcessorValueTreeState& parameters, const ParameterConfig::Id::OperatorParamIds& paramIds)
{
	op.setRatio(*parameters.getRawParameterValue(paramIds.ratioId));
	op.setAttack(*parameters.getRawParameterValue(paramIds.attckId));
	op.setDecay(*parameters.getRawParameterValue(paramIds.decayId));
	op.setSustain(*parameters.getRawParameterValue(paramIds.sustainId));
	op.setRelease(*parameters.getRawParameterValue(paramIds.releaseId));
	op.setWaveType(static_cast<ParameterConfig::Values::WaveType>(static_cast<int>(*parameters.getRawParameterValue(paramIds.wavetypeChoicesId))));
}

void Voice::registerOperatorCallbacks(jos::ParameterListener& paramListener, const ParameterConfig::Id::OperatorParamIds& paramIds, jos::Operator& op)
{
	paramListener.registerCallback(paramIds.ratioId, [&op](float newValue) { DBG("SET RATIO FOR OP: " << (int)&op);  op.setRatio(newValue); });
	paramListener.registerCallback(paramIds.attckId, [&op](float newValue) { DBG("SET ATTACK FOR OP: " << (int)&op); op.setAttack(newValue); });
	paramListener.registerCallback(paramIds.decayId, [&op](float newValue) { DBG("SET DECAY FOR OP: " << (int)&op); op.setDecay(newValue); });
	paramListener.registerCallback(paramIds.sustainId, [&op](float newValue) { DBG("SET SUSTAIN FOR OP: " << (int)&op); op.setSustain(newValue); });
	paramListener.registerCallback(paramIds.releaseId, [&op](float newValue) { DBG("SET RELEASE FOR OP: " << (int)&op); op.setRelease(newValue); });
	paramListener.registerCallback(paramIds.wavetypeChoicesId, [&op](float newValue) { DBG("SET Wave type FOR OP: " << newValue); op.setWaveType(static_cast<ParameterConfig::Values::WaveType>(static_cast<int>(newValue))); });
}

float Voice::processSample()
{
	switch (algorithmChoice)
	{
	case ParameterConfig::Values::AlgorithmType::I:
	{
		auto out4 = op4.processSample();
		op4.setPhaseMod(operator4Feedback * out4);
		
		op3.setPhaseMod(out4);
		auto out3 = op3.processSample();

		op2.setPhaseMod(out3);
		auto out2 = op2.processSample();

		op1.setPhaseMod(out2);
		return op1.processSample();
	}
	case ParameterConfig::Values::AlgorithmType::II:
	{
		auto out4 = op4.processSample();
		op4.setPhaseMod(operator4Feedback * out4);
		auto out3 = op3.processSample();

		op2.setPhaseMod(0.5f * (out3 + out4));
		auto out2 = op2.processSample();

		op1.setPhaseMod(out2);
		return op1.processSample();
	}
	case ParameterConfig::Values::AlgorithmType::III:
	{
		auto out4 = op4.processSample();
		op4.setPhaseMod(operator4Feedback * out4);
		auto out3 = op3.processSample();

		op2.setPhaseMod(out3);
		auto out2 = op2.processSample();

		op1.setPhaseMod(0.5 * (out2 + out4));
		return op1.processSample();
	}
	case ParameterConfig::Values::AlgorithmType::IV:
	{
		auto out4 = op4.processSample();
		op4.setPhaseMod(operator4Feedback * out4);
		op3.setPhaseMod(out4);
		auto out3 = op3.processSample();

		auto out2 = op2.processSample();

		op1.setPhaseMod(0.5 * (out2 + out3));
		return op1.processSample();
	}
	case ParameterConfig::Values::AlgorithmType::V:
	{
		auto out4 = op4.processSample();
		op4.setPhaseMod(operator4Feedback * out4);
		auto out3 = op3.processSample();

		op2.setPhaseMod(out4);
		op1.setPhaseMod(out3);

		return 0.5 * (op1.processSample() + op2.processSample());
	}
	case ParameterConfig::Values::AlgorithmType::VI:
	{
		auto out4 = op4.processSample();
		op4.setPhaseMod(operator4Feedback * out4);
		op3.setPhaseMod(out4);
		op2.setPhaseMod(out4);
		op1.setPhaseMod(out4);

		return (op1.processSample() + op2.processSample() + op3.processSample()) / 3.f;
	}
	case ParameterConfig::Values::AlgorithmType::VII:
	{
		auto out4 = op4.processSample();
		op4.setPhaseMod(operator4Feedback * out4);
		op3.setPhaseMod(out4);

		return (op1.processSample() + op2.processSample() + op3.processSample()) / 3.f;
	}
	case ParameterConfig::Values::AlgorithmType::VIII:
	{
		auto out4 = op4.processSample();
		op4.setPhaseMod(operator4Feedback * out4);
		return (op1.processSample() + op2.processSample() + op3.processSample() + out4) / 4.f;
	}
	default:
		return 0.f;
	}
}

}
