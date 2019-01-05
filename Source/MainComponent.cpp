#include "MainComponent.h"
//==============================================================================
MainComponent::MainComponent()
{
	// Start the timer - this will call the timerCallback function
	// which will refresh the oscilloscope graphics:
	startTimerHz(30);

	// Envelopes, resize and initialise:
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

	//====== MIDI: ======
	chosenNote.resize(5);
	midiMessages.resize(5);
	for (int i = 0; i < 5; ++i)
	{
		chosenNote[i] = 9;
		// initialise midiMessages array with a noteOn event, 0 velocity 
		midiMessages[i] = MidiMessage::noteOn(1, 50, (uint8)0);

	}

	//=== Midi input Combo Box: ===
	// Let's user choose which device to output MIDI to:
	addAndMakeVisible(midiOutputList);
	midiOutputList.setTextWhenNoChoicesAvailable("No MIDI Outputs Enabled");
	MidiOutput::getDevices();
	midiOutput = MidiOutput::openDevice(0);
	midiOutputList.addItemList(MidiOutput::getDevices(), 1);
	midiOutputList.onChange = [this]() {midiOutput = MidiOutput::openDevice(midiOutputList.getSelectedItemIndex()); };

	//=== Key transposition Combo Box: ===
	// Let's the user choose a key in which the 
	// notes will be played: 
	addAndMakeVisible(keyOptions);
	keyOptions.setTextWhenNoChoicesAvailable("Key");
	keyOptions.addItemList(scaleRef.keyNamesMajor, 1); //string array
	keyOptions.onChange = [this]() { styleMenuChanged();
	};

	// Array where the selected key values will be stored:
	key.resize(7);
	for (int i = 0; i < 7; i++)
	{
		key[i] = 0;
	}

	//====== Oscilloscope Graphics: ==================
	// The audio buffer 512 in length, however we want
	// smoother graphics, so we're going to keep sampling
	// the audio buffer to accumulate several buffers' worth
	// of data, hence the sineBuffer (512), and bigSineBuffer:
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

	//==== Frequency Sliders ====
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
	
	// Global Frequency increase and decrease slider:
	// For times when we want to equally increase or decrease
	// the speed of the pattern while keeping the original form:
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

	//======== Transposition Sliders: ===============
	for (int i = 0; i < 5; i++) {
		Slider* t = new Slider();
		//
		t->setSliderStyle(Slider::LinearVertical);
		t->setRange(-24, 24, 12);
		t->setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
		t->setColour(Slider::textBoxTextColourId, Colours::black);
		t->setColour(Slider::textBoxOutlineColourId, Colours::transparentWhite);
		t->setValue(0);
		addAndMakeVisible(t_sliders.add(t));
		t_sliders[i]->addListener(this);
	}

	//=== Sequencer Grid Buttons ====
	for (int i = 0; i < 7; i++) {
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

	setSize(800, 600);
    setAudioChannels (0, 2);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
	// Get   pointer to the left and right buffers:
	auto* leftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
	auto* rightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

	// The Audio Loop:
	for (int sample = 0; sample < bufferToFill.numSamples; sample++)
	{
		// Assingn the oscillator objects to Square wave generators, and store the current value:
		double square1 = (osc1.square(f1) + 1) / 2;
		double square2 = (osc2.square(f2) + 1) / 2;
		double square3 = (osc3.square(f3) + 1) / 2;

		// If the oscillator is turned off, make sure it is equal to 0
		if (f1 == 0.0) {
			square1 = 0.;
		}
		if (f2 == 0.0) {
			square2 = 0.;
		}
		if (f3 == 0.0) {
			square3 = 0.;
		}

		// Add up the square waves: 
		double sum = (square1 + square2 + square3);
		
		// Add the current amplitde to the graphics buffer, and increment the index of the graphics buffer:
		sineBuffer[index] = sum;
		index += 1;
		// Wrap around:
		if (index >= graphicSize) {
			index = 0;
		}

		// If the value has changed and its not zero then we are at a new plateau
		if (sum != previous_sum && sum != 0.0)
		{
			bottomReached = true;
		}

		// if we have reached a plateau then set off a note-on event once, 
		// but turn off bottomReached immediately so that no more note-on 
		// events are triggered:
		if (bottomReached == true)
		{
			playNote = true;
			bottomReached = false;

		}

		// Update the envelopes for each note:
		for (int i = 0; i < 5; i++)
		{
			ADSRout[i] = ADSR[i].adsr(1., ADSR[i].trigger);
		
			// If the envelope is off and the note is also off, stop the note from playing:
			if (ADSRout[i] <= 0.1 && noteAlreadyOff[i] == false)
			{
				midiMessages[i].noteOff(midiMessages[i].getChannel(), midiMessages[i].getNoteNumber());
				midiMessages[i].setTimeStamp(time);
				midiMessages[i].setVelocity(0);
				midiOutput->sendMessageNow(midiMessages[i]);
				noteAlreadyOff[i] = true;
			}
		}

		//======= The Midi output: ========
		// If a midi note has been triggered, then go through every midi object in the sequencer (5 in total)
		if (playNote == true)
		{
			// This will store the value of the note to be played:
			int whichNote = 0;

			// If its the first column in the sequencer:
			if (playNoteIndex == 0)
			{
				// Turn off the currently playing note if it isn't already off:
				if (noteAlreadyOff[0] == false)
				{
				midiMessages[0].noteOff(midiMessages[0].getChannel(), midiMessages[0].getNoteNumber());
				midiMessages[0].setTimeStamp(time);
				midiOutput->sendMessageNow(midiMessages[0]);
				}

				// Reset the envelope trigger:
				ADSR[0].trigger = 1;
				ADSRout[0] = ADSR[0].adsr(1., ADSR[0].trigger);
				
				if (skipNote[0] == true)
				{
					// Don't go sending any notes.
				}
				else {
					for (int i = 0; i < 7; i++)
					{
						// if the note has been selected on the sequencer:
						if (chosenNote[0] == i)
						{
							// Assign it to the corresponding note in the key
							whichNote = key[i];
							// Set the midiMessage to the key:
							// Setting the note number and velocity
							// in the noteOn() function caused unpredictable behaviour,
							// or no assignment, so they are explicitly set using
							// setNoteNumber() and setVelocity():
							midiMessages[0].noteOn(1, 0, (uint8)100); //(unit8)100
							midiMessages[0].setNoteNumber(key[i] + transposeBy[0]);
							midiMessages[0].setVelocity(1);
							//midiMessages[0].setTimeStamp(time);
							// Send the message:
							midiOutput->sendMessageNow(midiMessages[0]);
							// Set envelope trigger to 0, so it doesn't retrigger:
							ADSR[0].trigger = 0;
							// The note isn't off anymore:
							noteAlreadyOff[0] = false;
						}
					}
				}

			}
			
			// If its the second column in the sequencer:
			if (playNoteIndex == 1)
			{
				if (noteAlreadyOff[1] == false)
				{
					midiMessages[1].noteOff(midiMessages[1].getChannel(), midiMessages[1].getNoteNumber());
					midiMessages[1].setTimeStamp(time);
					midiOutput->sendMessageNow(midiMessages[1]);
				}
				ADSR[1].trigger = 1;
				ADSRout[1] = ADSR[1].adsr(1., ADSR[1].trigger);
				if (skipNote[1] == true)
				{

				}
				else {
					for (int i = 0; i < 7; i++)
					{
						if (chosenNote[1] == i)
						{
							whichNote = key[i];
							midiMessages[1].noteOn(1, 0, (uint8)100); 
							midiMessages[1].setNoteNumber(key[i] + transposeBy[1]);
							midiMessages[1].setVelocity(1);
							midiMessages[1].setTimeStamp(time);
							midiOutput->sendMessageNow(midiMessages[1]);
							ADSR[1].trigger = 0;
							noteAlreadyOff[1] = false;
						}
					}
				}
			}

			// If its the third column in the sequencer:
			if (playNoteIndex == 2)
			{
				if (noteAlreadyOff[2] == false)
				{
					midiMessages[2].noteOff(midiMessages[2].getChannel(), midiMessages[2].getNoteNumber());
					midiMessages[2].setTimeStamp(time);
					midiOutput->sendMessageNow(midiMessages[2]);
				}
				ADSR[2].trigger = 1;
				ADSRout[2] = ADSR[2].adsr(1., ADSR[2].trigger);

				if (skipNote[2] == true)
				{

				}
				else {
					for (int i = 0; i < 7; i++)
					{
						if (chosenNote[2] == i)
						{
							whichNote = key[i];
							midiMessages[2].noteOn(1, 0, (uint8)100);
							midiMessages[2].setNoteNumber(key[i] + transposeBy[2]);
							midiMessages[2].setVelocity(1);
							midiMessages[2].setTimeStamp(time);
							midiOutput->sendMessageNow(midiMessages[2]);
							ADSR[2].trigger = 0;
							noteAlreadyOff[2] = false;
						}
					}
				}
			}

			// If its the fourth column in the sequencer:
			if (playNoteIndex == 3)
			{
				if (noteAlreadyOff[3] == false)
				{
					midiMessages[3].noteOff(midiMessages[3].getChannel(), midiMessages[3].getNoteNumber());
					midiMessages[3].setTimeStamp(time);
					midiOutput->sendMessageNow(midiMessages[3]);
				}
				ADSR[3].trigger = 1;
				ADSRout[3] = ADSR[3].adsr(1., ADSR[3].trigger);

				if(skipNote[3] == true)
				{
					
				}
				else {
					for (int i = 0; i < 7; i++)
					{
						if (chosenNote[3] == i)
						{
							whichNote = key[i];
							midiMessages[3].noteOn(1, 0, (uint8)100);
							midiMessages[3].setNoteNumber(key[i] + transposeBy[3]);
							midiMessages[3].setVelocity(1);
							midiMessages[3].setTimeStamp(time);
							midiOutput->sendMessageNow(midiMessages[3]);
							ADSR[3].trigger = 0;
							noteAlreadyOff[3] = false;
						}
					}
				}
			}

			// If its the fifth column in the sequencer:
			if (playNoteIndex == 4)
			{
				if (noteAlreadyOff[4] == false)
				{
					midiMessages[4].noteOff(midiMessages[4].getChannel(), midiMessages[4].getNoteNumber());
					midiMessages[4].setTimeStamp(time);
					midiOutput->sendMessageNow(midiMessages[4]);
				}

				ADSR[4].trigger = 1;
				ADSRout[4] = ADSR[4].adsr(1., ADSR[4].trigger);

				if (skipNote[4] == true)
				{} else {
					for (int i = 0; i < 7; i++)
					{
						if (chosenNote[4] == i)
						{
							whichNote = key[i];
							midiMessages[4].noteOn(1, 0, (uint8)100); 
							midiMessages[4].setNoteNumber(key[i] + transposeBy[4]);
							midiMessages[4].setVelocity(1);
							midiMessages[4].setTimeStamp(time);
							midiOutput->sendMessageNow(midiMessages[4]);
							ADSR[4].trigger = 0;
							noteAlreadyOff[4] = false;
						}
					}
				}
			}

		
			//============
			// Turn off the MIDI event trigger:
			playNote = false;
			// Advance through the sequencer
			playNoteIndex++;
			// Wrap around
			if (playNoteIndex >= 5)
			{
				playNoteIndex = 0;
			}

		}

		// set the previous square-wave value to the current value:
		previous_sum = sum;

		// If the graphics buffer has been filled, then trigger the timer callback
		// and refresh the graphics: 
		if (index == graphicSize - 1)
		{
			nextSineBlockReady = true;
		}
		//  time will be constantly incrementing, sample by sample
		time = time + sample;
	}

	// Update the envelope sustain and release if 
	// they have been changed by the user:
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

    // Send MIDI note events for all notes, so we have no hanging notes after
    // we close the software. Update: this doesn't work. 
	
	for (int i = 0; i < 5; i++)
	{
		ADSR[i].trigger = 0;

			midiMessages[i].noteOff(midiMessages[i].getChannel(), midiMessages[i].getNoteNumber());
			midiMessages[i].setVelocity(0);
			midiOutput->sendMessageNow(midiMessages[i]);
			noteAlreadyOff[i] = true;
		
	}
	delete midiOutput;
}

//=====================================
void MainComponent::timerCallback()
{
	// If true, redraw the oscilloscope graphics:
	if (nextSineBlockReady == true)
	{
		nextSineBlockReady = false;
		repaint();
	}
}

//==============================================================================



//==============================================================================
void MainComponent::paint (Graphics& g)
{
	g.fillAll(Colours::whitesmoke);  
	g.fillAll(Colours::transparentBlack);

	g.setColour(Colours::darkslategrey);
	g.fillRect(0, 0, 800, 600);
	g.setColour(Colours::whitesmoke);
	g.fillRect(2.5, 2.5, 795, 595);
	g.setColour(Colours::darkslategrey);
	g.fillRect(194, 45, 318, 210);
	g.setColour(Colours::darkgrey);
	g.fillRect(199, 50, 308, 200);
	drawFrame(g);
}

//=======================================================
void MainComponent::resized()
{
	// Sliders for the square-wave frequencies:
	freq1.setBounds(10, 40, 150, 150);
	freq2.setBounds(10, 230, 150, 150);
	freq3.setBounds(10, 420, 150, 150);

	// Speed slider:
	move_all.setBounds(210, 300, 100, 100);

	// Transposition sliders:
	for (int i = 0; i < 5; i++)
	{
		t_sliders[i]->setBounds(515+(i*50), 230, 50, 100);
	}

	// Envelope slider:
	envelopeLength.setBounds(350, 300,50, 100);
	
	// Sequencer Buttons:
	for (int i = 0; i < buttonsRow1.size(); ++i)
	{
		buttonsRow1[i]->setBounds(520, 60 + (i*25), 40, 20);
		buttonsRow2[i]->setBounds(520 + 50, 60 + (i*25), 40, 20);
		buttonsRow3[i]->setBounds(520 + (50*2), 60 + (i*25), 40, 20);
		buttonsRow4[i]->setBounds(520 + (50*3), 60 + (i*25), 40, 20);
		buttonsRow5[i]->setBounds(520 + (50*4), 60 + (i*25), 40, 20);
	}

	// MIDI and Key ComboBox:
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

	// Code for switching the colours of the buttons when on : 
	// currently the step prior to the one actually playing 
	// switches colour, so this needs to be fixed:
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
	// Series of if-else statements that control the toggle-button states. 
	// Should be made less complex/easy-to-follow-through:

	// If the button is from sequencer step 1
	if (buttonsRow1.contains(button))
	{
		// If this isn't the very first click during runtime:
		if(firstButtonClick[0] == false)
		{ 
			// If the button at that index isn't the same one as previously toggled, or it is the same one but we're currently skipping it:
			if (buttonsRow1.indexOf(button) != previousState[0] || (buttonsRow1.indexOf(button) == previousState[0] && skipNote[0] == true))
			{
				// If we haven't already been skipping the note:
				if (skipNote[0] != true)
				{
					// turn the previous step off:
					buttonsRow1[previousState[0]]->setToggleState(false, dontSendNotification);
				}
				// Set the note to be played to the new selected note index:
				chosenNote[0] = buttonsRow1.indexOf(button);
				// Set the previously toggled button index to the current one:
				previousState[0] = buttonsRow1.indexOf(button);
				// Don't skip the note:
				skipNote[0] = false;

			} else { skipNote[0] = true; }

		// Else if this is the first time we've selected the sequencer step since runtime start:
		} else {
			chosenNote[0] = buttonsRow1.indexOf(button);
			previousState[0] = buttonsRow1.indexOf(button);
			firstButtonClick[0] = false;
		}
	}

	// If the button is from sequencer step 2
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

	// If the button is from sequencer step 3
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

	// If the button is from sequencer step 4
	if (buttonsRow4.contains(button))
	{
		if (firstButtonClick[3] == false)
		{
			if (buttonsRow4.indexOf(button) != previousState[3] || (buttonsRow4.indexOf(button) == previousState[3] && skipNote[3] == true) )
			{
				if (skipNote[3] != true)
				{
					buttonsRow4[previousState[3]]->setToggleState(false, dontSendNotification);
				} 
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

	// If the button is from sequencer step 5
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


void MainComponent::sliderValueChanged(Slider* slider)
{
	// Define the behaviour of the transpose sliders:
	// i.e. assign the values to the transposeBy variables. 
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
	// Define the behaviour of the speed slider:
	// i.e. multiply the current frequencies:
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
	// Define the behaviour for the Key selection Combo Box:
	for (int i = 0; i < 7; i++)
	{
		key[i] = scaleRef.scalesMajor[keyOptions.getSelectedItemIndex()][i];
	}
}
