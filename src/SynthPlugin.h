//
//  SynthPlugin.h
//  MySynth Plugin Header File - for the top-level synthesiser object
//
//  Used to declare objects and data structures used by the plugin.
//

#pragma once

#include "apdi/Plugin.h"
#include "apdi/Helpers.h"
using namespace APDI;

#include "SynthExtra.h"

//===================================================================================================
// MySynth - object representing the whole synthesiser (containing multiple note objects - see below)
//===================================================================================================

class MySynth : public APDI::Synth
{
public:
    // constructor (initialise variables, etc.)
    MySynth(const Parameters& parameters, const Presets& presets, const char* resources);
    ~MySynth(); // destructor (clean up, free memory, etc.)

    void setSampleRate(float sampleRate){ stk::Stk::setSampleRate(sampleRate); }
    float getSampleRate() const { return stk::Stk::sampleRate(); };
    
    void postProcess(const float** inputBuffers, float** outputBuffers, int numSamples);
    
    void presetLoaded(int iPresetNum, const char *sPresetName);
    void optionChanged(int iOptionMenu, int iItem);
    void buttonPressed(int iButton);
    
private:
    // Declare synthesiser-level member variables here
   
};

