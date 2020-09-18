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
	saveAndLoadFileLocation(juce::File::getSpecialLocation(juce::File::userDocumentsDirectory))
{
	addAndMakeVisible(presetList);
	addAndMakeVisible(saveButton);
	addAndMakeVisible(loadButton);

	for (auto i = 0; i < BinaryData::namedResourceListSize; ++i)
	{
		juce::String presetStr{ BinaryData::namedResourceList[i] };
		presetList.addItem(presetStr.dropLastCharacters(4), i + 1);
	}
	presetList.setSelectedId(1);
	presetList.onChange = [this]() { selectPreset(); };

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

	int presetDataSize;
	juce::String presetDataString{ BinaryData::getNamedResource(BinaryData::namedResourceList[presetId - 1], presetDataSize) };
	juce::XmlDocument preset{ presetDataString };
	activatePreset(preset);
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
		juce::XmlDocument preset{ fileChooser.getResult() };
		activatePreset(preset);
	}
}

void PresetComponent::activatePreset(juce::XmlDocument& xmlDocument)
{
	auto presetXml = xmlDocument.getDocumentElement();
	if (presetXml && presetXml->hasTagName(parameters.state.getType()))
	{
		parameters.replaceState(juce::ValueTree::fromXml(*presetXml));
	}
}
