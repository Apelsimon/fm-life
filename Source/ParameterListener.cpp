#include "ParameterListener.h"

namespace jos
{

	ParameterListener::ParameterListener(juce::AudioProcessorValueTreeState& params) :
		parameters(params)
	{
	}

	ParameterListener::~ParameterListener()
	{
		for (const auto&[key, _] : callbackMap)
		{
			parameters.removeParameterListener(key, this);
		}
	}

	void ParameterListener::registerCallback(const juce::String& parameterId, OnParameterChanged onParameterChanged)
	{
		parameters.addParameterListener(parameterId, this);
		callbackMap[parameterId].push_back(onParameterChanged);
	}

	void ParameterListener::parameterChanged(const juce::String &parameterID, float newValue)
	{
		auto it = callbackMap.find(parameterID);
		if (it != callbackMap.end())
		{
			auto& paramCbs = it->second;
			for (auto& paramCb : paramCbs)
			{
				paramCb(newValue);
			}
		}
	}

} // namespace jos
