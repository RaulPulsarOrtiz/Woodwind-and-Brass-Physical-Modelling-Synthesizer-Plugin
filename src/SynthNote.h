//
//  SynthNote.h
//  MySynth Plugin Header File - for individual notes
//
//  Used to declare objects and data structures used by the plugin.
//

#include "SynthPlugin.h"
//#include "SynthExtra.h"
#pragma once

//================================================================================
// MyNote - object representing a single note (within the synthesiser - see above)
//================================================================================

class MyNote : public APDI::Synth::Note
               
{
public:
    MyNote(MySynth* synthesiser) : Note(synthesiser), fFrequency(440.f), fLevel(1.f) { }
    
    MySynth* getSynthesiser() { return (MySynth*)synthesiser; }
    
    void onStartNote (int pitch, float velocity);
    bool onStopNote (float velocity);
    void onPitchWheel (int value);
    void onControlChange (int controller, int value);
    
    bool process (float** outputBuffer, int numChannels, int numSamples);
    
private:
    // Declare note class variables here
    float fFrequency;
    float fLevel;
    Envelope attackEG;
   
    Trumpet trumpet;
    Flute flute;
    Clarinet clarinet;
    Bassoon bassoon;
   // float fExciter;
};
