#ifndef COMPONENT_ATTACHMENT_H_
#define COMPONENT_ATTACHMENT_H_

#include <JuceHeader.h>

namespace jos
{

	template<typename Component, typename Attachment>
	class ComponentAttachment
	{
	public:
		ComponentAttachment(juce::AudioProcessorValueTreeState& parameters, const juce::String& parameterId) :
			component(),
			attachment(std::make_unique<Attachment>(parameters, parameterId, component))
		{
		}

		Component& getComponent() { return component; }
		Component& operator()() { return component; }

	private:
		Component component;
		std::unique_ptr<Attachment> attachment;
	};

	using SliderAttachment = ComponentAttachment<juce::Slider, juce::AudioProcessorValueTreeState::SliderAttachment>;
} // namespace jos


#endif
