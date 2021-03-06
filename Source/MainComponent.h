#pragma once

#include "maximilian.h"
#include "scaleBank.hpp"
#include "../JuceLibraryCode/JuceHeader.h"

// Custom Juce Look and Feel class for the Rotary Dials:
class OtherLookAndFeel : public LookAndFeel_V4
{
public:
	OtherLookAndFeel()
	{
		setColour(Slider::thumbColourId, Colours::darkslategrey); // 0x1001300
		setColour(Slider::backgroundColourId, Colours::blue); // 0x1001200, defined below
		setColour(Slider::rotarySliderFillColourId, Colours::lightseagreen); // covered values
		setColour(Slider::rotarySliderOutlineColourId, Colours::darkslategrey); // outline colour when not covered
	}

	void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
	{
		auto outline = slider.findColour(Slider::rotarySliderOutlineColourId);
		auto fill = slider.findColour(Slider::rotarySliderFillColourId);

		//auto bounds = Juce::Rectangle<int>(x, y, width, height).toFloat().reduced (10);
		auto radius = jmin(width, height) / 2.0f;
		auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
		float rad = radius * 0.5;
		auto lineW = jmin(8.0f, rad);
		auto arcRadius = radius - lineW * 0.5f;

		// Big circle:
		float centreX = x + width * 0.5f;
		float centreY = y + height * 0.5f;
		float rx = centreX - radius;
		float ry = centreY - radius;
		float rw = radius * 2.0f;

		g.setColour(Colours::whitesmoke);
		g.fillEllipse(rx, ry, rw, rw);

		// Smaller filled circle:
		float rx2 = centreX - (radius / 2);
		float ry2 = centreY - (radius / 2);
		// fill:
		g.setColour(Colours::darkslategrey);
		g.fillEllipse(rx2, ry2, (rw / 2), (rw / 2));

		Path backgroundArc;
		backgroundArc.addCentredArc(width / 2,
			height / 2,
			arcRadius,
			arcRadius,
			0.0f,
			rotaryStartAngle,
			rotaryEndAngle,
			true);

		g.setColour(outline);
		g.strokePath(backgroundArc, PathStrokeType(lineW, PathStrokeType::beveled, PathStrokeType::butt));

		if (slider.isEnabled())
		{
			Path valueArc;
			valueArc.addCentredArc(width / 2,
				height / 2,
				arcRadius / 1.1,
				arcRadius / 1.1,
				0.0f,
				rotaryStartAngle,
				toAngle,
				true);

			g.setColour(fill);
			g.strokePath(valueArc, PathStrokeType(15, PathStrokeType::beveled, PathStrokeType::butt));
		}

		if (slider.isEnabled())
		{
			Path valueArc;
			valueArc.addCentredArc(width / 2,
				height / 2,
				arcRadius / 1.5,
				arcRadius / 1.5,
				0.0f,
				rotaryStartAngle,
				toAngle,
				true);

			g.setColour(Colours::darkseagreen);
			g.strokePath(valueArc, PathStrokeType(10, PathStrokeType::beveled, PathStrokeType::butt));
		}

		Path valueArc;
		valueArc.addCentredArc(width / 2,
			height / 2,
			arcRadius / 2.0,
			arcRadius / 2.0,
			0.0f,
			rotaryStartAngle,
			rotaryEndAngle,
			true);

		g.setColour(fill);
		g.strokePath(valueArc, PathStrokeType(5, PathStrokeType::mitered, PathStrokeType::butt));

		// Dial:
		float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
		Path p;
		float pointerLength = radius * 0.53f;
		float pointerThickness = 10.0f;
		p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
		p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));

		// fill the pointer path:
		g.setColour(Colours::darkslategrey);
		g.fillPath(p);

	}
};

//==============================================================================
class MainComponent : public AudioAppComponent, private Timer, public Button::Listener, public::Slider::Listener
{
public:
	//==============================================================================
	MainComponent();
	~MainComponent();

	//==============================================================================
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;

	//==============================================================================
	void paint(Graphics& g) override;
	void resized() override;

	// Graphics functions:
	void timerCallback() override;
	void drawFrame(Graphics& g);

	// Slider and Button callback functions: 
	void buttonClicked(Button* button) override;
	void sliderValueChanged(Slider* slider) override;
	void sliderDragEnded(Slider* slider) override;
	void styleMenuChanged();

private:
	//==============================================================================
	
	// Square Wave Oscillators and Frequencies:
	maxiOsc osc1, osc2, osc3;
	double f1 = 0.0;
	double f2 = 0.0;
	double f3 = 0.0;

	// Envelopes:
	vector<maxiEnv> ADSR;
	vector<double> ADSRout;
	int envLen = 8000;

	// Midi: 
	MidiOutput* midiOutput;
	AudioDeviceManager deviceManager;
	ComboBox midiOutputList;
	vector<MidiMessage> midiMessages;
	vector<int> transposeBy = { 0, 0, 0, 0, 0 };
	vector<int> chosenNote;
	vector<bool> noteAlreadyOff = { true, true, true, true, true };
	vector<int> velocities; // not used.
	double previous_sum = 0;
	int playNoteIndex = 0;
	bool bottomReached = true;
	bool playNote = false;

	// Key transposition Selection Menu: 
	ComboBox keyOptions;
	Oscillator scaleRef;
	vector<int> key;

	// Oscilloscope Graphics:
	vector<double> sineBuffer;
	vector<double> bigSineBuffer;
	int graphicSize;
	int bigGraphicSize;
	bool nextSineBlockReady = false;
	int index = 0;
	int time = 0;

	// GUI
	Slider freq1, freq2, freq3, move_all, transpose_1, envelopeLength;
	OwnedArray<Slider> t_sliders;
	OwnedArray<Button> buttonsRow1;
	OwnedArray<Button> buttonsRow2;
	OwnedArray<Button> buttonsRow3;
	OwnedArray<Button> buttonsRow4;
	OwnedArray<Button> buttonsRow5;

	// Sequencer Grid: 
	vector<int> previousState = { 0, 0, 0, 0, 0 };
	vector<bool> firstButtonClick = { true, true, true, true, true };
	vector<bool> skipNote = {false, false, false, false, false};

	// Custom Look and Feel
	OtherLookAndFeel otherLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
