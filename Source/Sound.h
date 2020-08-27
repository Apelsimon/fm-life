#pragma once

#include <JuceHeader.h>

namespace jos
{

class Sound : public juce::SynthesiserSound
{
public:
	Sound() {}

	bool appliesToNote(int) override { return true; }
	bool appliesToChannel(int) override { return true; }
};

}
