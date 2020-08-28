#include "AudioEngine.h"

#include "Sound.h"
#include "Voice.h"

namespace jos
{

AudioEngine::AudioEngine()
{
	for (auto i = 0; i < MaxNumVoices; ++i)
	{
		addVoice(new Voice);
	}
	addSound(new Sound);
}

void AudioEngine::prepare(const juce::dsp::ProcessSpec& spec, juce::AudioProcessorValueTreeState& parameters) noexcept
{
	setCurrentPlaybackSampleRate(spec.sampleRate);

	for (auto voice : voices)
	{
		dynamic_cast<Voice*>(voice)->prepare(spec, parameters);
	}
}

void AudioEngine::registerParameterCallbacks(jos::ParameterListener& paramListener)
{
	for (auto voice : voices)
	{
		dynamic_cast<Voice*>(voice)->registerParameterCallbacks(paramListener);
	}
}

}