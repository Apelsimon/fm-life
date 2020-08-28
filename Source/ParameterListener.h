#ifndef PARAMETER_LISTENER_H_
#define PARAMETER_LISTENER_H_

#include <JuceHeader.h>

#include <vector>

namespace jos
{

	class ParameterListener : public juce::AudioProcessorValueTreeState::Listener
	{
		using OnParameterChanged = std::function<void(float)>;

	public:
		ParameterListener(juce::AudioProcessorValueTreeState& params);
		~ParameterListener();

		void registerCallback(const juce::String& parameterId, OnParameterChanged onParameterChanged);

	private:
		void parameterChanged(const juce::String &parameterID, float newValue) override;

		juce::AudioProcessorValueTreeState& parameters;
		std::unordered_map<juce::String, std::vector<OnParameterChanged>> callbackMap;
	};

} // namespace jos
#endif
