/*
  ==============================================================================

    primeCalculator.hpp
    Created: 28 Dec 2018 7:06:56pm
    Author:  Pootpoot

  ==============================================================================
*/
#include "../JuceLibraryCode/JuceHeader.h"
class Oscillator {

public:
	Oscillator() {

		// Major:
		keyNamesMajor.add("Cmajor");
		keyNamesMajor.add("Dmajor");
		keyNamesMajor.add("Emajor");
		keyNamesMajor.add("Fmajor");
		keyNamesMajor.add("Gmajor");
		keyNamesMajor.add("Amajor");
		keyNamesMajor.add("Bmajor");
		keyNamesMajor.add("C#/Db major");
		keyNamesMajor.add("D#/Eb major");
		keyNamesMajor.add("F#/Gb major");
		keyNamesMajor.add("G#/Ab major");
		keyNamesMajor.add("A#/Bb major");
		
		// Natural Minor:
		keyNamesMajor.add("A minor");
		keyNamesMajor.add("A#/Bb minor");
		keyNamesMajor.add("B minor");
		keyNamesMajor.add("C minor");
		keyNamesMajor.add("C#/Db minor");
		keyNamesMajor.add("D minor");
		keyNamesMajor.add("D#/Eb minor");
		keyNamesMajor.add("E minor");
		keyNamesMajor.add("F minor");
		keyNamesMajor.add("F#/Gb minor");
		keyNamesMajor.add("G minor");
		keyNamesMajor.add("G#/Ab minor");

		// Blues Minor
		keyNamesMajor.add("C minor blues");
		keyNamesMajor.add("C#/Db minor blues");
		keyNamesMajor.add("D minor blues");
		keyNamesMajor.add("D#/Eb minor blues");
		keyNamesMajor.add("E minor blues");
		keyNamesMajor.add("F minor blues");
		keyNamesMajor.add("F#/Gb minor blues");
		keyNamesMajor.add("G minor blues");
		keyNamesMajor.add("G#/Ab minor blues");
		keyNamesMajor.add("A minor blues");
		keyNamesMajor.add("A#/Bb minor blues");
		keyNamesMajor.add("B minor blues");
		
		// Blues Major
		keyNamesMajor.add("C major blues");
		keyNamesMajor.add("C#/Db major blues");
		keyNamesMajor.add("D major blues");
		keyNamesMajor.add("D#/Eb major blues");
		keyNamesMajor.add("E major blues");
		keyNamesMajor.add("F major blues");
		keyNamesMajor.add("F#/Gb major blues");
		keyNamesMajor.add("G major blues");
		keyNamesMajor.add("G#/Ab major blues");
		keyNamesMajor.add("A major blues");
		keyNamesMajor.add("A#/Bb major blues");
		keyNamesMajor.add("B major blues");

	};

	int scalesMajor[48][7] = { {60, 62, 64, 65, 67, 69, 71},{62, 64, 65, 67,69, 71, 73},{64, 66, 68, 69, 71, 73, 75}, {65, 67, 69, 70, 72, 74, 76},
{67, 69, 71, 72, 74, 76, 78}, {69, 71, 73, 74, 76, 78, 80}, {71, 73, 75, 76, 78, 80, 82}, {61, 63, 65, 66, 68, 70, 72},
{63, 65, 65, 68, 70, 72, 75}, {66, 68, 70, 71,73, 75, 77}, {68, 70, 72, 73, 75, 77, 79}, {70, 72, 74, 75, 77, 79, 81},
	 {69, 71, 72, 74, 76, 77, 79}, {70, 72, 73, 75, 77, 78, 80}, {71, 73, 74, 76, 78, 79, 81},
{60, 62, 63, 65, 67, 68, 70}, {61, 63, 64, 66, 68, 69, 71}, {62, 64, 65, 67, 69, 70, 72}, {63, 65, 66, 68, 70, 71, 73},
{64, 66, 67, 69, 71, 72, 74}, {65, 67, 68, 70, 72, 73, 75}, {66, 68, 69, 71, 73, 74, 76}, {67, 69, 70, 72, 74, 75, 77},
{68, 70, 71, 73, 75, 76, 78}, {60, 63, 65, 66, 67, 70, 72}, {61, 64, 66, 67, 68, 71, 72}, {62, 65, 67, 68, 69, 72, 74},
{63, 66, 68, 69, 70, 73, 75}, {64, 67, 69, 70, 71, 74, 76}, {65, 68, 70, 71, 72, 75, 77},
{66, 69, 71, 72, 73, 76, 78}, {67, 70, 72, 73, 74, 77, 79}, {69, 71, 73, 74, 75, 78, 80},
{69, 72, 74, 75, 76, 79, 81}, {70, 73, 75, 76, 77, 80, 82}, {71, 74, 76, 77, 78, 81, 83},
	 {60, 62, 63, 64, 67, 69, 72}, {61, 63, 64, 65, 68, 70, 73}, {62, 64, 65, 66, 69, 71, 74},
{63, 65, 66, 67, 70, 72, 75}, {64, 66, 67, 68, 71, 73, 76}, {65, 67, 68, 69, 72, 74, 77},
{66, 68, 69, 70, 73, 75, 78}, {67, 69, 70, 71, 74, 76, 79}, {68, 70, 71, 72, 75, 77, 80},
{69, 72, 72, 73, 76, 78, 81}, {70, 72, 73, 74, 77, 79, 82}, {59, 61, 62, 63, 66, 68, 71} };

	

	StringArray keyNamesMajor;
	
private:
	//int array[3] = { 2, 3, 4 };
};

/*

int scalesMajor[12][7] = { {60, 62, 64, 65, 67, 69, 71},{62, 64, 65, 67,69, 71, 73},{64, 66, 68, 69, 71, 73, 75}, {65, 67, 69, 70, 72, 74, 76},
{67, 69, 71, 72, 74, 76, 78}, {69, 71, 73, 74, 76, 78, 80}, {71, 73, 75, 76, 78, 80, 82}, {61, 63, 65, 66, 68, 70, 72},
{63, 65, 65, 68, 70, 72, 75}, {66, 68, 70, 71,73, 75, 77}, {68, 70, 72, 73, 75, 77, 79}, {70, 72, 74, 75, 77, 79, 81}  };


vector<vector<int>> scalesNaturalMinor = { {69, 71, 72, 74, 76, 77, 79}, {70, 72, 73, 75, 77, 78, 80}, {71, 73, 74, 76, 78, 79, 81},
{60, 62, 63, 65, 67, 68, 70}, {61, 63, 64, 66, 68, 69, 71}, {62, 64, 65, 67, 69, 70, 72}, {63, 65, 66, 68, 70, 71, 73},
{64, 66, 67, 69, 71, 72, 74}, {65, 67, 68, 70, 72, 73, 75}, {66, 68, 69, 71, 73, 74, 76}, {67, 69, 70, 72, 74, 75, 77},
{68, 70, 71, 73, 75,76, 78, 80} };

vector<string> keyNamesNaturalMinor = { "Aminor", "A#/Bb minor", "Bminor", "Cminor", "C#/Db minor", "Dminor", "D#/Eb minor", "Eminor",
"Fminor", "F#/Gb minor", "Gminor", "G#/Ab minor" };

vector<vector<int>> scalesBlueMinor = { {60, 63, 65, 66, 67, 70, 72}, {61, 64, 66, 67, 68, 71, 72}, {62, 65, 67, 68, 69, 72, 74},
{63, 66, 68, 69, 70, 73, 75}, {64, 67, 69, 70, 71, 74, 76}, {65, 68, 70, 71, 72, 75, 77},
{66, 69, 71, 72, 73, 76, 78}, {67, 70, 72, 73, 74, 77, 79}, {69, 71, 73, 74, 75, 78, 80},
{69, 72, 74, 75, 76, 79, 81}, {70, 73, 75, 76, 77, 80, 82}, {71, 74, 76, 77, 78, 81, 83} };

vector<string> keyNamesBluesMinor = { "C", "C#/Db", "D", "D#/Eb", "E", "F", "F#/Gb", "G", "G#/Ab", "A", "A#/Bb", "B" };


vector<vector<int>> scalesBlueMajor = { {60, 62, 63, 64, 67, 69, 72}, {61, 63, 64, 65, 68, 70, 73}, {62, 64, 65, 66, 69, 71, 74},
{63, 65, 66, 67, 70, 72, 75}, {64, 66, 67, 68, 71, 73, 76}, {65, 67, 68, 69, 72, 74, 77},
{66, 68, 69, 70, 73, 75, 78}, {67, 69, 70, 71, 74, 76, 79}, {68, 70, 71, 72, 75, 77, 80},
{69, 72, 72, 73, 76, 78, 81}, {70, 72, 73, 74, 77, 79, 82}, {59, 61, 62, 63, 66, 68, 71} };

vector<string> keyNamesBluesMajor = { "C", "C#/Db", "D", "D#/Eb", "E", "F", "F#/Gb", "G", "G#/Ab", "A", "A#/Bb", "B" };
*/
/*
C3=60 
C#=61 Db = 61
D=62 
D#=63 Eb=63
E=64 
E# = 65 F=65 
F#=66 
G=67 
G#=68 Ab = 68
A=69 
A#=70 Bb = 70
B=71 

C4=72   
C#=73 
D=74 
D#=75 
E=76 
F=77 
F#=78 
G=79 
G#=80 
A=81 
A#=82 
B=83 
C5=84
*/
