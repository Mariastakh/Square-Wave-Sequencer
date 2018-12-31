/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
//==============================================================================
MainComponent::MainComponent()
{
	startTimerHz(30);
	ADSR.resize(5);
	ADSRout.resize(5);
	for (int i = 0; i < 5; i++)
	{
		ADSR[i].setAttack(0);
		ADSR[i].setDecay(2);
		ADSR[i].setSustain(2000);
		ADSR[i].setRelease(2000);
		ADSR[i].trigger = 0;

		ADSRout[i] = 0;
	}

	graphicSize = 512;
	bigGraphicSize = graphicSize * 6;
	sineBuffer.resize(graphicSize);
	bigSineBuffer.resize(bigGraphicSize);
	for (int i = 0; i < graphicSize; i++)
	{
		sineBuffer[i] = 0;
	}

	for (int i = 0; i < bigGraphicSize; i++)
	{
		bigSineBuffer[i] = 0;
	}

	chosenNote.resize(5);
	for (int i = 0; i < 5; ++i)
	{
		chosenNote[i] = 9;
	}
	// midi input disable:
	//auto midiInputs = MidiInput::getDevices();
	//setMidiInput(0);
	// resize the button vector:
	//noteButtons.resize(5);
	On.resize(5);
	for (int i = 0; i < 5; ++i)
	{
		//noteButtons[i].resize(7);
		On[i].resize(7);
	}
	for (int i = 0; i < 5; i++) // columns
	{
		for (int j = 0; j < 7; j++)
		{
			On[i][j] = false;
		}
	}

	//==========================
	// Note Buttons
	//==========================
	for (int i = 0; i < 7; i++){
		auto r1 = new TextButton();
		//r1->setLookAndFeel(&otherLookAndFeel);
		r1->setButtonText("1");
		r1->setColour(TextButton::buttonOnColourId, Colours::darkorange);
		//r1->setColour(TextButton::buttonColourId, Colours::blue);
		r1->setClickingTogglesState(true);
		addAndMakeVisible(buttonsRow1.add(r1));
		buttonsRow1[i]->addListener(this);
		
		//
		auto r2 = new TextButton();
		r2->setLookAndFeel(&otherLookAndFeel);
		r2->setButtonText("2");
		r2->setColour(TextButton::buttonOnColourId, Colours::darkorange);
		r2->setClickingTogglesState(true);
		addAndMakeVisible(buttonsRow2.add(r2));	
		buttonsRow2[i]->addListener(this);

		auto r3 = new TextButton();
		r3->setLookAndFeel(&otherLookAndFeel);
		r3->setButtonText("3");
		r3->setColour(TextButton::buttonOnColourId, Colours::darkorange);
		r3->setClickingTogglesState(true);
		addAndMakeVisible(buttonsRow3.add(r3));
		buttonsRow3[i]->addListener(this);

		auto r4 = new TextButton();
		r4->setLookAndFeel(&otherLookAndFeel);
		r4->setButtonText("4");
		r4->setColour(TextButton::buttonOnColourId, Colours::darkorange);
		r4->setClickingTogglesState(true);
		addAndMakeVisible(buttonsRow4.add(r4));
		buttonsRow4[i]->addListener(this);

		auto r5 = new TextButton();
		r5->setLookAndFeel(&otherLookAndFeel);
		r5->setButtonText("5");
		r5->setColour(TextButton::buttonOnColourId, Colours::darkorange);
		r5->setClickingTogglesState(true);
		addAndMakeVisible(buttonsRow5.add(r5));
		buttonsRow5[i]->addListener(this);
	}
	//======================================
	//==========================
	for (int i = 0; i < 5; i++) {
		Slider* t = new Slider();
		//
		t->setSliderStyle(Slider::LinearVertical);
		t->setRange(-24, 24, 12);
		t->setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
		t->setColour(Slider::textBoxTextColourId, Colours::black);
		t->setColour(Slider::textBoxOutlineColourId, Colours::transparentWhite);
		//t.onValueChange = [this] { transpose_1_by = transpose_1.getValue(); };
		t->setValue(0);
		//
		addAndMakeVisible(t_sliders.add(t));
		t_sliders[i]->addListener(this);
	}
	// MIDI things:
	// Array of Midi messgaes for individual velocity control via envelopes:
	midiMessages.resize(5);
	velocities.resize(5);


	addAndMakeVisible(midiOutputList);

	midiOutputList.setTextWhenNoChoicesAvailable("No MIDI Outputs Enabled");
	MidiOutput::getDevices();
	midiOutput = MidiOutput::openDevice(0);
	midiOutputList.addItemList(MidiOutput::getDevices(), 1);

	midiOutputList.onChange = [this]() {midiOutput = MidiOutput::openDevice(midiOutputList.getSelectedItemIndex()); };

	//messageOn = MidiMessage::noteOn(1, 50, (uint8)0);

	//=====
	// Frequency Sliders
	freq1.setSliderStyle(Slider::RotaryVerticalDrag);
	freq1.setRange(0.0, 1.0);
	freq1.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
	freq1.setColour(Slider::textBoxTextColourId, Colours::black);
	freq1.setColour(Slider::textBoxOutlineColourId, Colours::transparentWhite);
	freq1.onValueChange = [this] { f1 = freq1.getValue(); };
	freq1.setValue(f1);
	freq1.setLookAndFeel(&otherLookAndFeel);
	addAndMakeVisible(freq1);

	freq2.setSliderStyle(Slider::RotaryVerticalDrag);
	freq2.setRange(0.0, 1.0);
	freq2.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
	freq2.setColour(Slider::textBoxTextColourId, Colours::black);
	freq2.setColour(Slider::textBoxOutlineColourId, Colours::transparentWhite);
	freq2.onValueChange = [this] { f2 = freq2.getValue(); };
	freq2.setValue(f2);
	freq2.setLookAndFeel(&otherLookAndFeel);
	addAndMakeVisible(freq2);

	freq3.setSliderStyle(Slider::RotaryVerticalDrag);
	freq3.setRange(0.0, 1.0);
	freq3.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
	freq3.setColour(Slider::textBoxTextColourId, Colours::black);
	freq3.setColour(Slider::textBoxOutlineColourId, Colours::transparentWhite);
	freq3.onValueChange = [this] { f3 = freq3.getValue(); };
	freq3.setValue(f3);
	freq3.setLookAndFeel(&otherLookAndFeel);
	addAndMakeVisible(freq3);
	
	// Slider to move all three frequency slider values equally:
	move_all.setSliderStyle(Slider::RotaryVerticalDrag);
	move_all.setRange(0.1, 3, 0.1);
	move_all.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
	move_all.setColour(Slider::textBoxTextColourId, Colours::black);
	move_all.setColour(Slider::textBoxOutlineColourId, Colours::transparentWhite);
	move_all.addListener(this);
	move_all.setValue(1.0);
	move_all.setLookAndFeel(&otherLookAndFeel);
	addAndMakeVisible(move_all);
	
	envelopeLength.setSliderStyle(Slider::LinearVertical);
	envelopeLength.setRange(100, 10000, 100);
	envelopeLength.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
	envelopeLength.setColour(Slider::textBoxTextColourId, Colours::black);
	envelopeLength.setColour(Slider::textBoxOutlineColourId, Colours::transparentWhite);
	envelopeLength.onValueChange =[this]{ envLen = envelopeLength.getValue(); };
	envelopeLength.setValue(8000);
	addAndMakeVisible(envelopeLength);

	// initialise midiMessages array with a noteOn event, 0 velocity 
	for (int i = 0; i < 5; i++)
	{
		midiMessages[i] = MidiMessage::noteOn(1, 50, (uint8) 0);
		velocities[i] = 100;
	}

	// Key transposition Combo Box:
	addAndMakeVisible(keyOptions);

	keyOptions.setTextWhenNoChoicesAvailable("Key");
	keyOptions.addItemList(scaleRef.keyNamesMajor,1); //string array
	keyOptions.onChange = [this]() { styleMenuChanged();
	// scaleRef.scalesMajor[keyOptions.getSelectedItemIndex()];
	};
	key.resize(7);
	for (int i = 0; i < 7; i++) 
	{
		key[i] = 0;
	}
	//
	// Make sure you set the size of the component after
	// you add any child components.
	setSize(800, 600);

    // specify the number of input and output channels that we want to open
    setAudioChannels (2, 2);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()

}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
	// Get   pointer to the left and right buffers:
	auto* leftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
	auto* rightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

	// The Audio Loop:
	for (int sample = 0; sample < bufferToFill.numSamples; sample++)
	{

		double square1 = (osc2.square(f1) + 1) / 2;
		double square2 = (osc3.square(f2) + 1) / 2;
		double square3 = (osc6.square(f3) + 1) / 2;

		//double pulse1 = (osc7.pulse(f1, 0.1) + 1) / 2;
		//double pulse2 = (osc8.pulse(f2, 0.1) + 1) / 2;
		//double pulse3 = (osc9.pulse(f3, 0.1) + 1) / 2;

		if (f1 == 0.0) {
			//pulse1 = 0;
			square1 = 0;
		}
		if (f2 == 0.0) {
			//pulse2 = 0;
			square2 = 0;
		}
		if (f3 == 0.0) {
			//pulse3 = 0;
			square3 = 0;
		}

		// double sum = (square1+square2+square3)*0.3;
		double sum = (square1 + square2 + square3);
		//Logger::outputDebugString(std::to_string(sum));
		//std::cout << sum<<  std::endl;
		sineBuffer[index] = sum;
		index += 1;
		if (index >= graphicSize) {
			index = 0;
		}

		// if the value is zero, nothing should play
		if (sum == 0.0) //(time + sample) % noteDuration == 0
		{
			// don't do anything

		}

		// if the value has changed and its not zero then we are at a new plateau
		if (sum != previous_sum && sum != 0.0)// sum == 0.0
		{
			bottomReached = true;
		}

		// if we have reached a plateau then set off a note-on event once: 
		if (bottomReached == true) //sum == previous_sum && 
		{
			// turn off current note:
			// we only do this for monophonic playback:
			if (mono == true)
			{
				//MidiMessage messageOff = MidiMessage::noteOff(messageOn.getChannel(), messageOn.getNoteNumber());
				//messageOff.setTimeStamp(time);
				//midiOutput->sendMessageNow(messageOff);
			}
			//====================
			playNote = true;
			bottomReached = false;

		}

	
		
		for (int i = 0; i < 5; i++)
		{
			ADSRout[i] = ADSR[i].adsr(1., ADSR[i].trigger);
		
			if (ADSRout[i] <= 0.1 && noteAlreadyOff[i] == false)
			{
				midiMessages[i].noteOff(midiMessages[i].getChannel(), midiMessages[i].getNoteNumber());
				midiMessages[i].setTimeStamp(time);
				midiMessages[i].setVelocity(0);
				midiOutput->sendMessageNow(midiMessages[i]);
				noteAlreadyOff[i] = true;
			}
		}

		// output
		if (playNote == true)
		{
			int whichNote = 0;
			if (playNoteIndex == 0)
			{
				// Turn off the currently playing note:
				if (noteAlreadyOff[0] == false)
				{
				midiMessages[0].noteOff(midiMessages[0].getChannel(), midiMessages[0].getNoteNumber());
				midiMessages[0].setTimeStamp(time);
				midiOutput->sendMessageNow(midiMessages[0]);
				}
				//===
				// Reset the envelope trigger:
				ADSR[0].trigger = 1;
				ADSRout[0] = ADSR[0].adsr(1., ADSR[0].trigger);
				//Logger::outputDebugString(std::to_string(ADSRout[0]));
				if (skipNote[0] == true)
				{

				}
				else {
					for (int i = 0; i < 7; i++)
					{
						if (chosenNote[0] == i)
						{
							whichNote = key[i];
							midiMessages[0].noteOn(1, 0, (uint8)100); //(unit8)100
							midiMessages[0].setNoteNumber(key[i] + transposeBy[0]);
							Logger::outputDebugString(std::to_string(key[i]));
							midiMessages[0].setVelocity(1);
							//Logger::outputDebugString("Note 1: ");
							Logger::outputDebugString(std::to_string(midiMessages[0].getNoteNumber()));
							midiMessages[0].setTimeStamp(time);
							midiOutput->sendMessageNow(midiMessages[0]);
							ADSR[0].trigger = 0;
							firstLoop = false;
							noteAlreadyOff[0] = false;
						}
					}
				}

			}
			
			if (playNoteIndex == 1)//58, 60, 48, 51, 53, 54, 55,
			{
				// Turn off the currently playing note:
				if (noteAlreadyOff[1] == false)
				{
					midiMessages[1].noteOff(midiMessages[1].getChannel(), midiMessages[1].getNoteNumber());
					midiMessages[1].setTimeStamp(time);
					midiOutput->sendMessageNow(midiMessages[1]);
				}
				//===
				// Reset the envelope trigger:
				ADSR[1].trigger = 1;
				ADSRout[1] = ADSR[1].adsr(1., ADSR[1].trigger);
				//Logger::outputDebugString(std::to_string(ADSRout[1]));
				if (skipNote[1] == true)
				{

				}
				else {
					for (int i = 0; i < 7; i++)
					{
						if (chosenNote[1] == i)
						{
							whichNote = key[i];
							midiMessages[1].noteOn(1, 0, (uint8)100); //(unit8)100
							midiMessages[1].setNoteNumber(key[i] + transposeBy[1]);
							Logger::outputDebugString(std::to_string(key[i]));
							midiMessages[1].setVelocity(1);
							//Logger::outputDebugString("Note 1: ");
							Logger::outputDebugString(std::to_string(midiMessages[1].getNoteNumber()));
							midiMessages[1].setTimeStamp(time);
							midiOutput->sendMessageNow(midiMessages[1]);
							ADSR[1].trigger = 0;
							firstLoop = false;
							noteAlreadyOff[1] = false;
						}
					}
				}
			}
			if (playNoteIndex == 2)
			{
				// Turn off the currently playing note:
				if (noteAlreadyOff[2] == false)
				{
					midiMessages[2].noteOff(midiMessages[2].getChannel(), midiMessages[2].getNoteNumber());
					midiMessages[2].setTimeStamp(time);
					midiOutput->sendMessageNow(midiMessages[2]);
				}
				//===
				// Reset the envelope trigger:
				ADSR[2].trigger = 1;
				ADSRout[2] = ADSR[2].adsr(1., ADSR[2].trigger);
				//Logger::outputDebugString(std::to_string(ADSRout[2]));
				if (skipNote[2] == true)
				{

				}
				else {
					for (int i = 0; i < 7; i++)
					{
						if (chosenNote[2] == i)
						{
							whichNote = key[i];
							midiMessages[2].noteOn(1, 0, (uint8)100); //(unit8)100
							midiMessages[2].setNoteNumber(key[i] + transposeBy[2]);
							Logger::outputDebugString(std::to_string(key[i]));
							midiMessages[2].setVelocity(1);
							//Logger::outputDebugString("Note 1: ");
							Logger::outputDebugString(std::to_string(midiMessages[2].getNoteNumber()));
							midiMessages[2].setTimeStamp(time);
							midiOutput->sendMessageNow(midiMessages[2]);
							ADSR[2].trigger = 0;
							firstLoop = false;
							noteAlreadyOff[2] = false;
						}
					}
				}
			}
			if (playNoteIndex == 3)
			{
				// Turn off the currently playing note:
				if (noteAlreadyOff[3] == false)
				{
					midiMessages[3].noteOff(midiMessages[3].getChannel(), midiMessages[3].getNoteNumber());
					midiMessages[3].setTimeStamp(time);
					midiOutput->sendMessageNow(midiMessages[3]);
				}
				//===
				// Reset the envelope trigger:
				ADSR[3].trigger = 1;
				ADSRout[3] = ADSR[3].adsr(1., ADSR[3].trigger);

				if(skipNote[3] == true)
				{
					
				}
				else {
					//Logger::outputDebugString(std::to_string(ADSRout[3]));
					for (int i = 0; i < 7; i++)
					{
						if (chosenNote[3] == i)
						{
							whichNote = key[i];
							midiMessages[3].noteOn(1, 0, (uint8)100); //(unit8)100
							midiMessages[3].setNoteNumber(key[i] + transposeBy[3]);
							Logger::outputDebugString(std::to_string(key[i]));
							midiMessages[3].setVelocity(1);
							//Logger::outputDebugString("Note 1: ");
							Logger::outputDebugString(std::to_string(midiMessages[3].getNoteNumber()));
							midiMessages[3].setTimeStamp(time);
							midiOutput->sendMessageNow(midiMessages[3]);
							ADSR[3].trigger = 0;
							firstLoop = false;
							noteAlreadyOff[3] = false;
						}
					}
				}
			}
			if (playNoteIndex == 4)
			{
				// Turn off the currently playing note:
				if (noteAlreadyOff[4] == false)
				{
					midiMessages[4].noteOff(midiMessages[4].getChannel(), midiMessages[4].getNoteNumber());
					midiMessages[4].setTimeStamp(time);
					midiOutput->sendMessageNow(midiMessages[4]);
				}
				//===
				// Reset the envelope trigger:
				ADSR[4].trigger = 1;
				ADSRout[4] = ADSR[4].adsr(1., ADSR[4].trigger);
				//Logger::outputDebugString(std::to_string(ADSRout[3]));

				if (skipNote[4] == true)
				{
					// Do nothing
				}
				else {
					for (int i = 0; i < 7; i++)
					{
						if (chosenNote[4] == i)
						{
							whichNote = key[i];
							midiMessages[4].noteOn(1, 0, (uint8)100); //(unit8)100
							midiMessages[4].setNoteNumber(key[i] + transposeBy[4]);
							Logger::outputDebugString(std::to_string(key[i]));
							midiMessages[4].setVelocity(1);
							//Logger::outputDebugString("Note 1: ");
							Logger::outputDebugString(std::to_string(midiMessages[4].getNoteNumber()));
							midiMessages[4].setTimeStamp(time);
							midiOutput->sendMessageNow(midiMessages[4]);
							ADSR[4].trigger = 0;
							firstLoop = false;
							noteAlreadyOff[4] = false;
						}
					}
				}
			}

		
			//============
			// For mono purposes:
			if (mono == true)
			{
				//for (int i = 0; i < 5; i++)
				//{
					//ADSRout[i] = ADSR[i].adsr(1., ADSR[i].trigger);
				//	}
				//double vel = 127 * ADSRout[playNoteIndex];

				// the vel value rewrites for each no midi note sent, so at the moment we are monophonic. 
				// to do envelopes you need a different program logic. 
				//messageOn = MidiMessage::noteOn(1, whichNote, (uint8)(127 * ADSRout[playNoteIndex])); //(unit8)100
				//messageOn.setTimeStamp(time);
				//midi.addEvent(messageOn, time);
				//midiOutput->sendMessageNow(messageOn);
			}
			//============
			//============
			playNote = false;
			playNoteIndex++;
			if (playNoteIndex >= 5)
			{
				playNoteIndex = 0;
			}

		}

		previous_sum = sum;
		if (index == graphicSize - 1)
		{
			nextSineBlockReady1 = true;
		}
		//  time will be constantly incrementing, sample by sample
		time = time + sample;
	}

	for (int i = 0; i < 5; i++)
	{
		ADSR[i].setSustain(envLen);
		ADSR[i].setRelease(envLen);
	}
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//=====================================
void MainComponent::timerCallback()
{
	if (nextSineBlockReady1 == true)
	{
		nextSineBlockReady1 = false;
		repaint();
	}
}

//==============================================================================



//==============================================================================
void MainComponent::paint (Graphics& g)
{
	//Image background = ImageCache::getFromMemory(BinaryData::New_Project_png, BinaryData::New_Project_pngSize);
	//g.drawImageAt(background, 0, 0);
	g.fillAll(Colours::whitesmoke);  // yellow
	g.fillAll(Colours::transparentBlack);

	g.setColour(Colours::darkslategrey);
	g.fillRect(0, 0, 800, 600);
	g.setColour(Colours::whitesmoke);
	g.fillRect(2.5, 2.5, 795, 595);
	// background buttons panel:
	//g.setColour(Colours::darkolivegreen); //darkgoldenrod //palegoldenrod //palegreen // darkseagreen
	//g.fillRect(515, 53, 250, 210); //
	//g.fillRect(200, 259, 565, 250);
	//
	g.setColour(Colours::darkslategrey);
	g.fillRect(194, 45, 318, 210); //200
	g.setColour(Colours::darkgrey);
	g.fillRect(199, 50, 308, 200); //200
	drawFrame(g);
}

//=======================================================
void MainComponent::resized()
{
	// Sliders for first sample:
	freq1.setBounds(10, 40, 150, 150);
	freq2.setBounds(10, 230, 150, 150);
	freq3.setBounds(10, 420, 150, 150);
	move_all.setBounds(210, 300, 100, 100);

	for (int i = 0; i < 5; i++)
	{
		t_sliders[i]->setBounds(515+(i*50), 230, 50, 100);
	}

	envelopeLength.setBounds(350, 300,50, 100);
	
	// Labels:
	for (int i = 0; i < buttonsRow1.size(); ++i)
	{
		buttonsRow1[i]->setBounds(520, 60 + (i*25), 40, 20);
		buttonsRow2[i]->setBounds(520 + 50, 60 + (i*25), 40, 20);
		buttonsRow3[i]->setBounds(520 + (50*2), 60 + (i*25), 40, 20);
		buttonsRow4[i]->setBounds(520 + (50*3), 60 + (i*25), 40, 20);
		buttonsRow5[i]->setBounds(520 + (50*4), 60 + (i*25), 40, 20);
	}
	// ComboBox:
	midiOutputList.setBounds(700, 10, 70, 30);
	keyOptions.setBounds(600, 10, 70, 30);

}

//=======================================================================
void MainComponent::drawFrame(Graphics& g)
{

	for (int i = 0; i < 512; i++)
	{
		bigSineBuffer[i + 2560] = bigSineBuffer[i + 2048];
		bigSineBuffer[i + 2048] = bigSineBuffer[i + 1536];
		bigSineBuffer[i + 1536] = bigSineBuffer[i + 1024];
		bigSineBuffer[i + 1024] = bigSineBuffer[i + 512];
		bigSineBuffer[i + 512] = bigSineBuffer[i];
		bigSineBuffer[i] = sineBuffer[i];
	}

	for (int i = 1; i < bigGraphicSize - 1; i++) {
		float height1 = (bigSineBuffer[i] * 0.3) * 5;
		float height_prev = (bigSineBuffer[i - 1] * 0.3) * 5;

		g.setColour(Colours::skyblue);
		g.drawLine(200.0 + (((i / 10) - 1)), 150.0 - (height_prev * 20), 200.0 + (((i / 10))), 150.0 - (height1 * 20), 5);
	}

	// Code for flipping the colours of the buttons when on : 
	if (playNoteIndex == 0)
	{

		for (int i = 0; i < 5; i++)
		{
			if (chosenNote[0] == i)
			{
				buttonsRow1[i]->setColour(TextButton::buttonOnColourId, Colours::red);
			}
			else {
				buttonsRow1[i]->setColour(TextButton::buttonOnColourId, Colours::darkorange);
			}
		}
	}
	else {
		for (int i = 0; i < 5; i++)
		{
			buttonsRow1[i]->setColour(TextButton::buttonOnColourId, Colours::darkorange);
		}
	}
	//
	if (playNoteIndex == 1)
	{

		for (int i = 0; i < 5; i++)
		{
			if (chosenNote[1] == i)
			{
				buttonsRow2[i]->setColour(TextButton::buttonOnColourId, Colours::red);
			}
			else {
				buttonsRow2[i]->setColour(TextButton::buttonOnColourId, Colours::darkorange);
			}
		}
	}
	else {
		for (int i = 0; i < 5; i++)
		{
			buttonsRow2[i]->setColour(TextButton::buttonOnColourId, Colours::darkorange);
		}
	}
	//
	if (playNoteIndex == 2)
	{

		for (int i = 0; i < 5; i++)
		{
			if (chosenNote[2] == i)
			{
				buttonsRow3[i]->setColour(TextButton::buttonOnColourId, Colours::red);
			}
			else {
				buttonsRow3[i]->setColour(TextButton::buttonOnColourId, Colours::darkorange);
			}
		}
	}
	else {
		for (int i = 0; i < 5; i++)
		{
			buttonsRow3[i]->setColour(TextButton::buttonOnColourId, Colours::darkorange);
		}
	}
	//
	if (playNoteIndex == 3)
	{

		for (int i = 0; i < 5; i++)
		{
			if (chosenNote[3] == i)
			{
				buttonsRow4[i]->setColour(TextButton::buttonOnColourId, Colours::red);
			}
			else {
				buttonsRow4[i]->setColour(TextButton::buttonOnColourId, Colours::darkorange);
			}
		}
	}
	else {
		for (int i = 0; i < 5; i++)
		{
			buttonsRow4[i]->setColour(TextButton::buttonOnColourId, Colours::darkorange);
		}
	}
	//
	if (playNoteIndex == 4)
	{

		for (int i = 0; i < 5; i++)
		{
			if (chosenNote[4] == i)
			{
				buttonsRow5[i]->setColour(TextButton::buttonOnColourId, Colours::red);
			}
			else {
				buttonsRow5[i]->setColour(TextButton::buttonOnColourId, Colours::darkorange);
			}
		}
	}
	else {
		for (int i = 0; i < 5; i++)
		{
			buttonsRow5[i]->setColour(TextButton::buttonOnColourId, Colours::darkorange);
		}
	}
}
void MainComponent::buttonClicked(Button* button)
{

	// should map index to letter
	if (buttonsRow1.contains(button))
	{
		if(firstButtonClick[0] == false)
		{ 
			//
			if (buttonsRow1.indexOf(button) != previousState[0] || (buttonsRow1.indexOf(button) == previousState[0] && skipNote[0] == true))
			{
				if (skipNote[0] != true)
				{
					buttonsRow1[previousState[0]]->setToggleState(false, dontSendNotification);
				}
				else {}; // do nothing
				chosenNote[0] = buttonsRow1.indexOf(button);
				previousState[0] = buttonsRow1.indexOf(button);
				skipNote[0] = false;
			}
			else { skipNote[0] = true; }
			//
		}
		else {
			chosenNote[0] = buttonsRow1.indexOf(button);
			previousState[0] = buttonsRow1.indexOf(button);
			//Logger::outputDebugString(std::to_string(previousState[0]));
			firstButtonClick[0] = false;
		}
	}

	if (buttonsRow2.contains(button))
	{
		if (firstButtonClick[1] == false)
		{
			//
			if (buttonsRow2.indexOf(button) != previousState[1] || (buttonsRow2.indexOf(button) == previousState[1] && skipNote[1] == true))
			{
				if (skipNote[1] != true)
				{
					buttonsRow2[previousState[1]]->setToggleState(false, dontSendNotification);
				}
				else {}; // do nothing
				chosenNote[1] = buttonsRow2.indexOf(button);
				previousState[1] = buttonsRow2.indexOf(button);
				skipNote[1] = false;
			}
			else { skipNote[1] = true; }
			//
		}
		else {
			chosenNote[1] = buttonsRow2.indexOf(button);
			previousState[1] = buttonsRow2.indexOf(button);
			firstButtonClick[1] = false;
		}
	}

	if (buttonsRow3.contains(button))
	{
		if (firstButtonClick[2] == false)
		{
			//
			if (buttonsRow3.indexOf(button) != previousState[2] || (buttonsRow3.indexOf(button) == previousState[2] && skipNote[2] == true))
			{
				if (skipNote[2] != true)
				{
					buttonsRow3[previousState[2]]->setToggleState(false, dontSendNotification);
				}
				else {}; // do nothing
				chosenNote[2] = buttonsRow3.indexOf(button);
				previousState[2] = buttonsRow3.indexOf(button);
				skipNote[2] = false;
			}
			else { skipNote[2] = true; }
			//
		}
		else {
			chosenNote[2] = buttonsRow3.indexOf(button);
			previousState[2] = buttonsRow3.indexOf(button);
			firstButtonClick[2] = false;
		}
	}

	if (buttonsRow4.contains(button))
	{
		if (firstButtonClick[3] == false)
		{
			if (buttonsRow4.indexOf(button) != previousState[3] || (buttonsRow4.indexOf(button) == previousState[3] && skipNote[3] == true) )
			{
				if (skipNote[3] != true)
				{
					buttonsRow4[previousState[3]]->setToggleState(false, dontSendNotification);
				} else {}; // do nothing
				chosenNote[3] = buttonsRow4.indexOf(button);
				previousState[3] = buttonsRow4.indexOf(button);
				skipNote[3] = false; } else { skipNote[3] = true;  }

		}
		else 
		{
			chosenNote[3] = buttonsRow4.indexOf(button);
			previousState[3] = buttonsRow4.indexOf(button);
			firstButtonClick[3] = false;
		}
	}

	if (buttonsRow5.contains(button))
	{
		if (firstButtonClick[4] == false)
		{
			if (buttonsRow5.indexOf(button) != previousState[4] || (buttonsRow5.indexOf(button) == previousState[4] && skipNote[4] == true))
			{
				if (skipNote[4] != true)
				{
					buttonsRow5[previousState[4]]->setToggleState(false, dontSendNotification);
				}
				else {}; // do nothing
				chosenNote[4] = buttonsRow5.indexOf(button);
				previousState[4] = buttonsRow5.indexOf(button);
				skipNote[4] = false;
			}
			else { skipNote[4] = true; }
		}
		else {
			chosenNote[4] = buttonsRow5.indexOf(button);
			previousState[4] = buttonsRow5.indexOf(button);
			firstButtonClick[4] = false;
		}
	}
}

//=============================================
void MainComponent::addMessageToBuffer(const MidiMessage& message)
{
	auto timestamp = message.getTimeStamp();
	auto sampleNumber = (int)(timestamp * 44100);
	midiBuffer.addEvent(message, sampleNumber);
}

void MainComponent::sliderValueChanged(Slider* slider)
{
	for (int i = 0; i < 5; i++)
	{
		if (slider == t_sliders[i])
		{
			transposeBy[i] = t_sliders[i]->getValue();
		}
	}
}

void MainComponent::sliderDragEnded(Slider* slider)
{
	double multiplyBy = move_all.getValue();
		f1 = freq1.getValue();
		f1 *= multiplyBy;
		f2 = freq2.getValue();
		f2 *= multiplyBy;
		f3 = freq3.getValue();
		f3 *= multiplyBy;
}

//==================================================
void MainComponent::styleMenuChanged()
{
	for (int i = 0; i < 7; i++)
	{
		key[i] = scaleRef.scalesMajor[keyOptions.getSelectedItemIndex()][i];
		Logger::outputDebugString(to_string(key[i]));
	}
}