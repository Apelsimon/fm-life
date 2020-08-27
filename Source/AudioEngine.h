#pragma once

#include <JuceHeader.h>

namespace jos
{

class AudioEngine : public juce::Synthesiser
{
public:
	AudioEngine();

	void prepare(const juce::dsp::ProcessSpec& spec) noexcept;

private:
	static constexpr auto MaxNumVoices = 4;
};

}
