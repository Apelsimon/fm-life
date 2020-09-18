/*
  ==============================================================================

    PresetComponent.h
    Created: 15 Sep 2020 5:19:55pm
    Author:  Apelsimon

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PresetComponent  : public juce::Component
{
public:
    PresetComponent(juce::AudioProcessorValueTreeState& parameters);
    ~PresetComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
	void selectPreset();
	void savePreset();
	void loadPreset();
	void activatePreset(juce::XmlDocument& xmlDocument);

	juce::AudioProcessorValueTreeState& parameters;
	juce::ComboBox   presetList;
	juce::TextButton saveButton;
	juce::TextButton loadButton;
	juce::File       saveAndLoadFileLocation;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetComponent)
};
