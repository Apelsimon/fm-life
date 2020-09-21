/*
  ==============================================================================

    PresetComponent.cpp
    Created: 15 Sep 2020 5:19:55pm
    Author:  Apelsimon

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PresetComponent.h"

//==============================================================================
PresetComponent::PresetComponent(juce::AudioProcessorValueTreeState& parameters) :
	parameters(parameters),
	presetList(),
	saveButton(),
	loadButton(),
	saveAndLoadFileLocation(juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)),
	loadedPresets()
{
	addAndMakeVisible(presetList);
	addAndMakeVisible(saveButton);
	addAndMakeVisible(loadButton);

	for (auto i = 0; i < BinaryData::namedResourceListSize; ++i)
	{
		juce::String presetStr{ BinaryData::originalFilenames[i] };
		presetList.addItem(presetStr.dropLastCharacters(4), i + 1);
	}
	presetList.onChange = [this]() { selectPreset(); };
	presetList.setTextWhenNothingSelected("Presets");

	saveButton.setButtonText("Save");
	saveButton.onClick = [this]() { savePreset(); };
	
	loadButton.setButtonText("Load");
	loadButton.onClick = [this]() { loadPreset(); };
}

PresetComponent::~PresetComponent()
{
}

void PresetComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
}

void PresetComponent::resized()
{
	auto bounds = getLocalBounds();
	auto buttonWidth = bounds.getWidth() / 2.f;
	presetList.setBounds(bounds.removeFromBottom(bounds.getHeight() / 2.f));
	saveButton.setBounds(bounds.removeFromLeft(buttonWidth));
	loadButton.setBounds(bounds.removeFromLeft(buttonWidth));
}

void PresetComponent::selectPreset()
{
	auto presetId = presetList.getSelectedId();

	if (presetId <= BinaryData::namedResourceListSize)
	{
		int presetDataSize;
		juce::String presetDataString{ BinaryData::getNamedResource(BinaryData::namedResourceList[presetId - 1], presetDataSize) };
		auto preset = juce::XmlDocument{ presetDataString }.getDocumentElement();
		activatePreset(*preset);
	}
	else
	{
		presetId -= BinaryData::namedResourceListSize + 1;
		activatePreset(*loadedPresets[presetId]);
	}
}

void PresetComponent::savePreset()
{
	juce::FileChooser fileChooser{ "Save preset", saveAndLoadFileLocation, "*.xml" };
	
	if (fileChooser.browseForFileToSave(true))
	{
		auto state = parameters.copyState();
		auto xml = std::unique_ptr<juce::XmlElement>(state.createXml());
		saveAndLoadFileLocation = fileChooser.getResult().getParentDirectory();
		xml->writeTo(fileChooser.getResult());
	}
}

void PresetComponent::loadPreset()
{
	juce::FileChooser fileChooser{ "Load preset", saveAndLoadFileLocation , "*.xml" };
	if (fileChooser.browseForFileToOpen())
	{
		auto result = fileChooser.getResult();
		saveAndLoadFileLocation = result.getParentDirectory();
		auto preset = juce::XmlDocument{ result }.getDocumentElement();
		if (activatePreset(*preset))
		{
			loadedPresets.push_back(std::move(preset));
			auto itemId = presetList.getNumItems() + 1;
			presetList.addItem(result.getFileName().dropLastCharacters(4), itemId);
			presetList.setSelectedId(itemId);
		}
	}
}

bool PresetComponent::activatePreset(juce::XmlElement& xml)
{
	if (xml.hasTagName(parameters.state.getType()))
	{
		parameters.replaceState(juce::ValueTree::fromXml(xml));
		return true;
	}
	return false;
}
