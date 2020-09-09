#pragma once

#include <JuceHeader.h>

#include "ParameterConfig.h"

class WaveTypeComboBoxLookAndFeel : public juce::LookAndFeel_V4
{
public:
	void 	drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle< int > &area, bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu,
		const juce::String &text, const juce::String &shortcutKeyText, const juce::Drawable *icon, const juce::Colour *textColour) override
	{
		constexpr auto StrokeThickness = 2.5f;

		g.setColour(juce::Colours::wheat);
		
		juce::Path wavePath;

		if (text == ParameterConfig::Values::WaveTypeChoices[ParameterConfig::Values::WaveType::Sine])
		{
			auto inc = juce::MathConstants<float>::twoPi / static_cast<float>(area.getWidth());
			auto phase = 0.f;
			for (auto i = 0; i < area.getWidth(); ++i)
			{
				auto x = area.getX() + i;
				auto y = area.getCentreY() + std::sin(phase) * 0.5f * (area.getHeight() - StrokeThickness);
				if (i == 0)
				{
					wavePath.startNewSubPath(x, y);
				}
				else
				{
					wavePath.lineTo(x, y);
				}
				phase += inc;
			}
		}
		else if (text == ParameterConfig::Values::WaveTypeChoices[ParameterConfig::Values::WaveType::Square])
		{
			wavePath.startNewSubPath(area.getX(), area.getBottom());
			wavePath.lineTo(area.getX() + area.getWidth() / 2.f, area.getBottom() - StrokeThickness);
			wavePath.lineTo(area.getX() + area.getWidth() / 2.f, area.getY() + StrokeThickness);
			wavePath.lineTo(area.getX() + area.getWidth(), area.getY());
		}
		else if (text == ParameterConfig::Values::WaveTypeChoices[ParameterConfig::Values::WaveType::Triangle])
		{
			wavePath.startNewSubPath(area.getX(), area.getCentreY());
			wavePath.lineTo(area.getX() + area.getWidth() * 0.25f, area.getY() + StrokeThickness);
			wavePath.lineTo(area.getX() + area.getWidth() * 0.75f, area.getBottom() - StrokeThickness);
			wavePath.lineTo(area.getX() + area.getWidth(), area.getCentreY());
		}
		else if (text == ParameterConfig::Values::WaveTypeChoices[ParameterConfig::Values::WaveType::Saw])
		{
			wavePath.startNewSubPath(area.getX(), area.getCentreY());
			wavePath.lineTo(area.getX() + area.getWidth() * 0.5f, area.getY() + StrokeThickness);
			wavePath.lineTo(area.getX() + area.getWidth() * 0.5f, area.getBottom() - StrokeThickness);
			wavePath.lineTo(area.getX() + area.getWidth(), area.getCentreY());
		}
		else
		{
			g.drawText(text, area, juce::Justification::centred);
		}

		g.strokePath(wavePath, juce::PathStrokeType{ StrokeThickness });
	}

private:

};

