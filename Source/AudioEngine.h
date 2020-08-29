#pragma once

#include <JuceHeader.h>

namespace jos
{

class ParameterListener;

class AudioEngine : public juce::Synthesiser
{
public:
	AudioEngine();

	void prepare(const juce::dsp::ProcessSpec& spec, juce::AudioProcessorValueTreeState& parameters) noexcept;
	void registerParameterCallbacks(jos::ParameterListener& paramListener);

private:
	static constexpr auto MaxNumVoices = 4;
};

}
