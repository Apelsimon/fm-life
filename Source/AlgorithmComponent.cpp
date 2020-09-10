#include <JuceHeader.h>

#include "AlgorithmComponent.h"
#include "ParameterConfig.h"

//==============================================================================
AlgorithmComponent::AlgorithmComponent(juce::AudioProcessorValueTreeState& parameters) :
	algorithmChoices(parameters, ParameterConfig::Id::AlgorithmChoices),
	operatorBoxes(NumOperatorBoxes),
	boxWidth(0.f),
	boxHeight(0.f),
	widthPadding(0.f),
	heightPadding(0.f)
{
	addAndMakeVisible(algorithmChoices());
	algorithmChoices().setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	algorithmChoices().setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
	algorithmChoices().onValueChange = [this]() { repaint(); };
}

AlgorithmComponent::~AlgorithmComponent()
{
}

void AlgorithmComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
	
	auto currentSelection = static_cast<int>(algorithmChoices().getValue());
	auto bounds = getLocalBounds();

	paintAlgorithms(g, static_cast<ParameterConfig::Values::AlgorithmType>(currentSelection), bounds.removeFromRight(bounds.getWidth() * AlgoritmBoxesWidthRatio).reduced(20.f));
}

void AlgorithmComponent::resized()
{
	auto bounds = getLocalBounds();
	auto nonAlgoBoxBounds = bounds.removeFromLeft(bounds.getWidth() * (1.f - AlgoritmBoxesWidthRatio));
	algorithmChoices().setBounds(nonAlgoBoxBounds.removeFromRight(nonAlgoBoxBounds.getWidth() * 0.1f));
}

static void drawPath(juce::Path& path, juce::Point<float>& position, const juce::Point<float>& positionOffset, bool startNewSubPath = true) 
{
	if (startNewSubPath) path.startNewSubPath(position);
	position += positionOffset;
	path.lineTo(position);
};

static void strokeAndFillPath(juce::Graphics& g, juce::Path& path) 
{
	const auto pathStrokeType = juce::PathStrokeType{ 2.5f };
	g.strokePath(path, pathStrokeType);
	g.fillPath(path); // TODO: use onlye strokepath to simplify paintAlgorithms
};

static void drawOperatorNumber(juce::Graphics& g, const juce::Rectangle<float>& opRect1, const juce::Rectangle<float>& opRect2,
	const juce::Rectangle<float>& opRect3, const juce::Rectangle<float>& opRect4)
{
	g.setColour(juce::Colours::black);
	g.drawText("4", opRect4, juce::Justification::centred);
	g.drawText("3", opRect3, juce::Justification::centred);
	g.drawText("2", opRect2, juce::Justification::centred);
	g.drawText("1", opRect1, juce::Justification::centred);
}

void AlgorithmComponent::paintAlgorithms(juce::Graphics& g, ParameterConfig::Values::AlgorithmType currentSelection, const juce::Rectangle<int>& bounds)
{
	static std::once_flag operatorBoxInitFlag;
	std::call_once(operatorBoxInitFlag, [this, bounds]() { initOperatorBoxes(bounds); });

	g.setColour(juce::Colours::wheat);
	for (auto& box : operatorBoxes)
	{
		g.drawRect(box);
		g.fillRect(box);
	}

	g.setColour(juce::Colours::lightblue);

	switch (currentSelection)
	{
	case ParameterConfig::Values::AlgorithmType::I:
	{
		juce::Path path;
		juce::Path feedbackPath;

		auto currentPosition = juce::Point<float>{ operatorBoxes[1].getCentreX(), operatorBoxes[1].getY() - 0.5f * heightPadding };
		drawPath(path, currentPosition, { 0.f, 0.5f * heightPadding });

		currentPosition = {operatorBoxes[1].getCentreX(), operatorBoxes[1].getBottom() };
		drawPath(path, currentPosition, { 0.f, heightPadding });

		currentPosition = { operatorBoxes[5].getCentreX(), operatorBoxes[5].getBottom() };
		drawPath(path, currentPosition, { 0.f, heightPadding });

		currentPosition = { operatorBoxes[9].getCentreX(), operatorBoxes[9].getBottom() };
		drawPath(path, currentPosition, { 0.f, heightPadding });

		currentPosition = { operatorBoxes[13].getCentreX(), operatorBoxes[13].getBottom() };
		drawPath(path, currentPosition, { 0.f, heightPadding });

		currentPosition = { operatorBoxes[1].getCentreX(), operatorBoxes[1].getBottom() + 0.5f * heightPadding};
		drawPath(feedbackPath, currentPosition, { 0.5f * (boxWidth + widthPadding), 0.f });
		drawPath(feedbackPath, currentPosition, { 0.f, -boxHeight - heightPadding }, false);
		drawPath(feedbackPath, currentPosition, { -0.5f * (boxWidth + widthPadding), 0.f }, false);

		strokeAndFillPath(g, path);
		strokeAndFillPath(g, feedbackPath);

		drawOperatorNumber(g, operatorBoxes[13], operatorBoxes[9], operatorBoxes[5],
			operatorBoxes[1]);

		break;
	};
	case ParameterConfig::Values::AlgorithmType::II:
	{
		juce::Path path;

		auto currentPosition = juce::Point<float>{ operatorBoxes[5].getCentreX(), operatorBoxes[5].getBottom() };
		drawPath(path, currentPosition, { 0.f, heightPadding });

		currentPosition = { operatorBoxes[9].getCentreX(), operatorBoxes[9].getBottom() };
		drawPath(path, currentPosition, { 0.f, heightPadding });

		currentPosition = { operatorBoxes[13].getCentreX(), operatorBoxes[13].getBottom() };
		drawPath(path, currentPosition, { 0.f, heightPadding });

		strokeAndFillPath(g, path);

		juce::Path path2;
		currentPosition = { operatorBoxes[6].getCentreX(), operatorBoxes[6].getY() - 0.5f * heightPadding };
		drawPath(path2, currentPosition, { 0.f, 0.5f * heightPadding });

		currentPosition = { operatorBoxes[6].getCentreX(), operatorBoxes[6].getBottom()};
		drawPath(path2, currentPosition, { 0.f, 0.5f * heightPadding });
		drawPath(path2, currentPosition, { -boxWidth - widthPadding, 0.f }, false);

		strokeAndFillPath(g, path2);

		juce::Path feedbackPath;
		currentPosition = { operatorBoxes[6].getCentreX(), operatorBoxes[6].getBottom() + 0.5f * heightPadding };
		drawPath(feedbackPath, currentPosition, { 0.5f * (boxWidth + widthPadding), 0.f});
		drawPath(feedbackPath, currentPosition, { 0.f, -boxHeight - heightPadding }, false);
		drawPath(feedbackPath, currentPosition, { -0.5f * (boxWidth + widthPadding), 0.f }, false);
		strokeAndFillPath(g, feedbackPath);

		drawOperatorNumber(g, operatorBoxes[13], operatorBoxes[9], operatorBoxes[5],
			operatorBoxes[6]);

		break;
	};
	case ParameterConfig::Values::AlgorithmType::III:
	{
		juce::Path path;

		auto currentPosition = juce::Point<float>{ operatorBoxes[5].getCentreX(), operatorBoxes[5].getBottom() };
		drawPath(path, currentPosition, { 0.f, heightPadding });

		currentPosition = { operatorBoxes[9].getCentreX(), operatorBoxes[9].getBottom() };
		drawPath(path, currentPosition, { 0.f, heightPadding });

		currentPosition = { operatorBoxes[13].getCentreX(), operatorBoxes[13].getBottom() };
		drawPath(path, currentPosition, { 0.f, heightPadding });

		strokeAndFillPath(g, path);

		juce::Path path2;

		currentPosition = juce::Point<float>{ operatorBoxes[10].getCentreX(), operatorBoxes[10].getY() - 0.5f * heightPadding };
		drawPath(path2, currentPosition, { 0.f, 0.5f * heightPadding });

		currentPosition = juce::Point<float>{ operatorBoxes[10].getCentreX(), operatorBoxes[10].getBottom()};
		drawPath(path2, currentPosition, { 0.f, 0.5f * heightPadding });
		drawPath(path2, currentPosition, { -boxWidth - widthPadding, 0.f }, false);

		strokeAndFillPath(g, path2);

		juce::Path feedbackPath;

		currentPosition = juce::Point<float>{ operatorBoxes[10].getCentreX(), operatorBoxes[10].getBottom() + 0.5f * heightPadding };
		drawPath(feedbackPath, currentPosition, {0.5f * (boxWidth + widthPadding), 0.f });
		drawPath(feedbackPath, currentPosition, { 0.f, -boxHeight - heightPadding }, false);
		drawPath(feedbackPath, currentPosition, { -0.5f * (boxWidth + widthPadding), 0.f  }, false);

		strokeAndFillPath(g, feedbackPath);

		drawOperatorNumber(g, operatorBoxes[13], operatorBoxes[9], operatorBoxes[5],
			operatorBoxes[10]);

		break;
	};
	case ParameterConfig::Values::AlgorithmType::IV:
	{
		juce::Path path;

		auto currentPosition = juce::Point<float>{ operatorBoxes[9].getCentreX(), operatorBoxes[9].getBottom() };
		drawPath(path, currentPosition, { 0.f, heightPadding });

		currentPosition = { operatorBoxes[13].getCentreX(), operatorBoxes[13].getBottom() };
		drawPath(path, currentPosition, { 0.f, heightPadding });

		strokeAndFillPath(g, path);

		juce::Path path2;

		currentPosition = { operatorBoxes[6].getCentreX(), operatorBoxes[6].getY() - 0.5f * heightPadding };
		drawPath(path2, currentPosition, { 0.f, 0.5f * heightPadding });

		currentPosition = { operatorBoxes[6].getCentreX(), operatorBoxes[6].getBottom()};
		drawPath(path2, currentPosition, { 0.f, heightPadding });

		currentPosition = { operatorBoxes[10].getCentreX(), operatorBoxes[10].getBottom() };
		drawPath(path2, currentPosition, { 0.f, 0.5f * heightPadding });
		drawPath(path2, currentPosition, { -boxWidth - widthPadding, 0.f }, false);

		strokeAndFillPath(g, path2);

		juce::Path feedbackPath;

		currentPosition = { operatorBoxes[6].getCentreX(), operatorBoxes[6].getBottom() + 0.5f * heightPadding};
		drawPath(feedbackPath, currentPosition, { 0.5f * (boxWidth + widthPadding), 0.f });
		drawPath(feedbackPath, currentPosition, { 0.f, -boxHeight - heightPadding }, false);
		drawPath(feedbackPath, currentPosition, { -0.5f * (boxWidth + widthPadding), 0.f }, false);

		strokeAndFillPath(g, feedbackPath);

		drawOperatorNumber(g, operatorBoxes[13], operatorBoxes[9], operatorBoxes[10],
			operatorBoxes[6]);

		break;
	};
	case ParameterConfig::Values::AlgorithmType::V:
	{
		juce::Path path;

		auto currentPosition = juce::Point<float>{ operatorBoxes[9].getCentreX(), operatorBoxes[9].getBottom() };
		drawPath(path, currentPosition, { 0.f, heightPadding });

		currentPosition = { operatorBoxes[13].getCentreX(), operatorBoxes[13].getBottom() };
		drawPath(path, currentPosition, { 0.f, 0.5f * heightPadding });
		drawPath(path, currentPosition, { 0.5f * (boxWidth + widthPadding), 0.f }, false);

		strokeAndFillPath(g, path);

		juce::Path path2;

		currentPosition = { operatorBoxes[10].getCentreX(), operatorBoxes[10].getY() - 0.5f * heightPadding};
		drawPath(path2, currentPosition, { 0.f, 0.5f * heightPadding });

		currentPosition = { operatorBoxes[10].getCentreX(), operatorBoxes[10].getBottom()};
		drawPath(path2, currentPosition, { 0.f, heightPadding });

		currentPosition = { operatorBoxes[14].getCentreX(), operatorBoxes[14].getBottom() };
		drawPath(path2, currentPosition, { 0.f, 0.5f * heightPadding });
		drawPath(path2, currentPosition, { -0.5f * (boxWidth + widthPadding), 0.f }, false);
		drawPath(path2, currentPosition, { 0.f, 0.5f * heightPadding}, false);

		strokeAndFillPath(g, path2);

		juce::Path feedbackPath;

		currentPosition = { operatorBoxes[10].getCentreX(), operatorBoxes[10].getBottom() + 0.5f * heightPadding };
		drawPath(feedbackPath, currentPosition, { 0.5f * (boxWidth + widthPadding), 0.f });
		drawPath(feedbackPath, currentPosition, { 0.f, -boxHeight - heightPadding}, false);
		drawPath(feedbackPath, currentPosition, { -0.5f * (boxWidth + widthPadding), 0.f }, false);

		strokeAndFillPath(g, feedbackPath);

		drawOperatorNumber(g, operatorBoxes[13], operatorBoxes[14], operatorBoxes[9],
			operatorBoxes[10]);

		break;
	};
	case ParameterConfig::Values::AlgorithmType::VI:
	{
		juce::Path path;

		auto currentPosition = juce::Point<float>{ operatorBoxes[9].getCentreX(), operatorBoxes[9].getY() - 0.5f * heightPadding};
		drawPath(path, currentPosition, { 0.f, 0.5f * heightPadding });

		currentPosition = { operatorBoxes[9].getCentreX(), operatorBoxes[9].getBottom()};
		drawPath(path, currentPosition, { 0.f, heightPadding });

		currentPosition = { operatorBoxes[13].getCentreX(), operatorBoxes[13].getBottom() };
		drawPath(path, currentPosition, { 0.f, heightPadding });

		strokeAndFillPath(g, path);

		juce::Path path2;

		currentPosition = { operatorBoxes[9].getCentreX(), operatorBoxes[9].getBottom() + 0.5f * heightPadding};
		drawPath(path2, currentPosition, { -boxWidth - widthPadding, 0.f });
		drawPath(path2, currentPosition, { 0.f, 0.5f * heightPadding }, false);

		currentPosition = { operatorBoxes[12].getCentreX(), operatorBoxes[12].getBottom() };
		drawPath(path2, currentPosition, { 0.f, 0.5f * heightPadding });
		drawPath(path2, currentPosition, { boxWidth + widthPadding, 0.f }, false);

		strokeAndFillPath(g, path2);

		juce::Path path3;

		currentPosition = { operatorBoxes[9].getCentreX(), operatorBoxes[9].getBottom() + 0.5f * heightPadding };
		drawPath(path3, currentPosition, { boxWidth + widthPadding, 0.f });
		drawPath(path3, currentPosition, { 0.f, 0.5f * heightPadding }, false);

		currentPosition = { operatorBoxes[14].getCentreX(), operatorBoxes[14].getBottom() };
		drawPath(path3, currentPosition, { 0.f, 0.5f * heightPadding });
		drawPath(path3, currentPosition, { -boxWidth - widthPadding, 0.f }, false);

		strokeAndFillPath(g, path3);

		juce::Path feedbackPath;

		currentPosition = { operatorBoxes[9].getCentreX(), operatorBoxes[9].getBottom() + 0.2f * heightPadding };
		drawPath(feedbackPath, currentPosition, { 0.5f * (boxWidth + widthPadding), 0.f });
		drawPath(feedbackPath, currentPosition, { 0.f, -boxHeight - 0.7f * heightPadding }, false);
		drawPath(feedbackPath, currentPosition, { -0.5f * (boxWidth + widthPadding), 0.f }, false);

		strokeAndFillPath(g, feedbackPath);

		drawOperatorNumber(g, operatorBoxes[12], operatorBoxes[13], operatorBoxes[14],
			operatorBoxes[9]);

		break;
	};
	case ParameterConfig::Values::AlgorithmType::VII:
	{
		juce::Path path;

		auto currentPosition = juce::Point<float>{ operatorBoxes[10].getCentreX(), operatorBoxes[10].getY() - 0.5f * heightPadding };
		drawPath(path, currentPosition, { 0.f, 0.5f * heightPadding });

		currentPosition = { operatorBoxes[10].getCentreX(), operatorBoxes[10].getBottom() };
		drawPath(path, currentPosition, { 0.f, heightPadding });

		currentPosition = { operatorBoxes[14].getCentreX(), operatorBoxes[14].getBottom() };
		drawPath(path, currentPosition, { 0.f, 0.5f * heightPadding });
		drawPath(path, currentPosition, { -2.f * (boxWidth + widthPadding), 0.f }, false);

		strokeAndFillPath(g, path);

		juce::Path path2;

		currentPosition = { operatorBoxes[12].getCentreX(), operatorBoxes[12].getBottom() };
		drawPath(path2, currentPosition, { 0.f, 0.5f * heightPadding });

		strokeAndFillPath(g, path2);

		juce::Path path3;

		currentPosition = { operatorBoxes[13].getCentreX(), operatorBoxes[13].getBottom() };
		drawPath(path3, currentPosition, { 0.f, heightPadding });

		strokeAndFillPath(g, path3);
		
		juce::Path feedbackPath;

		currentPosition = { operatorBoxes[10].getCentreX(), operatorBoxes[10].getBottom() + 0.5f * heightPadding};
		drawPath(feedbackPath, currentPosition, { 0.5f * (boxWidth + widthPadding), 0.f });
		drawPath(feedbackPath, currentPosition, { 0.f, -boxHeight - heightPadding }, false);
		drawPath(feedbackPath, currentPosition, { -0.5f * (boxWidth + widthPadding), 0.f }, false);

		strokeAndFillPath(g, feedbackPath);

		drawOperatorNumber(g, operatorBoxes[12], operatorBoxes[13], operatorBoxes[14],
			operatorBoxes[10]);

		break;
	};
	case ParameterConfig::Values::AlgorithmType::VIII:
	{
		for (auto i = 12; i < 15; ++i)
		{
			juce::Path path;

			auto currentPosition = juce::Point<float>{ operatorBoxes[i].getCentreX(), operatorBoxes[i].getBottom() };
			drawPath(path, currentPosition, { 0.f, 0.5f * heightPadding });

			strokeAndFillPath(g, path);
		}

		juce::Path path;

		auto currentPosition = juce::Point<float>{ operatorBoxes[15].getCentreX(), operatorBoxes[15].getY() - 0.5f * heightPadding};
		drawPath(path, currentPosition, { 0.f, 0.5f * heightPadding });

		currentPosition = { operatorBoxes[15].getCentreX(), operatorBoxes[15].getBottom()};
		drawPath(path, currentPosition, { 0.f, 0.5f * heightPadding });

		strokeAndFillPath(g, path);

		juce::Path path2;

		currentPosition = { operatorBoxes[12].getCentreX(), operatorBoxes[12].getBottom() + 0.5f * heightPadding};
		drawPath(path2, currentPosition, { 3.f * (boxWidth + widthPadding), 0.f });

		strokeAndFillPath(g, path2);

		juce::Path path3;

		currentPosition = { operatorBoxes[13].getCentreX() + 0.5f * (boxWidth + widthPadding), operatorBoxes[13].getBottom() + 0.5f * heightPadding };
		drawPath(path3, currentPosition, { 0.f, 0.5f * heightPadding });

		strokeAndFillPath(g, path3);

		juce::Path feedbackPath;

		currentPosition = { operatorBoxes[15].getCentreX(), operatorBoxes[15].getBottom() + 0.2f * heightPadding };
		drawPath(feedbackPath, currentPosition, { 0.5f * (boxWidth + widthPadding), 0.f });
		drawPath(feedbackPath, currentPosition, { 0.f, - boxHeight - 0.7f * heightPadding }, false);
		drawPath(feedbackPath, currentPosition, { -0.5f * (boxWidth + widthPadding), 0.f }, false);

		strokeAndFillPath(g, feedbackPath);

		drawOperatorNumber(g, operatorBoxes[12], operatorBoxes[13], operatorBoxes[14],
			operatorBoxes[15]);

		break;
	};
	default:
		break;
	}
}

void AlgorithmComponent::initOperatorBoxes(juce::Rectangle<int> bounds)
{	
	boxWidth = bounds.getWidth() * 0.85f;
	boxHeight = bounds.getHeight() * 0.65f;
	widthPadding = bounds.getWidth() - boxWidth;
	heightPadding = bounds.getHeight() - boxHeight;

	boxWidth /= 4;
	boxHeight /= 4;
	widthPadding /= 3;
	heightPadding /= 3;
	
	auto xOffset = boxWidth + widthPadding;
	auto yOffset = boxHeight + heightPadding;
	auto x = bounds.getX();
	auto y = bounds.getY();

	for (auto i = 0; i < NumOperatorBoxes; ++i)
	{
		operatorBoxes[i].setSize(boxWidth, boxHeight);
		operatorBoxes[i].setPosition(x, y);
		x += xOffset;
		if ((i + 1) % 4 == 0)
		{
			x = bounds.getX();
			y += yOffset;
		}
	}
}
