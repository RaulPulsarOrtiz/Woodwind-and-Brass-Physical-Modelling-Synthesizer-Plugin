//
//  SynthPlugin.cpp
//  MySynth Plugin Source Code - for the top-level synthesiser object
//
//  Used to define the bodies of functions used by the plugin, as declared in SynthPlugin.h.
//

#include "SynthPlugin.h"
#include "SynthNote.h"

////////////////////////////////////////////////////////////////////////////
// SYNTH - represents the whole synthesis plugin
////////////////////////////////////////////////////////////////////////////

extern "C" {
    // Called to create the synthesiser (used to add your synth to the host plugin)
    CREATE_FUNCTION createSynth(float sampleRate, const char* resources) {
        ::stk::Stk::setSampleRate(sampleRate);
        
        //==========================================================================
        // CONTROLS - Use this array to completely specify your UI
        // - tells the system what parameters you want, and how they are controlled
        // - add or remove parameters by adding or removing entries from the list
        // - each control should have an expressive label / caption
        // - controls can be of different types: ROTARY, BUTTON, TOGGLE, SLIDER, or MENU (see definitions)
        // - for rotary and linear sliders, you can set the range of values (make sure the initial value is inside the range)
        // - for menus, replace the three numeric values with a single array of option strings: e.g. { "one", "two", "three" }
        // - by default, the controls are laid out in a grid, but you can also move and size them manually
        //   i.e. replace AUTO_SIZE with { 50,50,100,100 } to place a 100x100 control at (50,50)
        
        const Parameters CONTROLS = {
//            {   "PITCH\nBEND",Parameter::WHEEL,0.0, 16384, 8192, AUTO_SIZE  },
//            {   "MOD\nWHEEL",Parameter::WHEEL, 0.0, 127.0, 0.0,  AUTO_SIZE  },

            //  name,       type,              min, max, initial, size
            {   "Choose BreathInput",  Parameter::MENU, {"Flute", "Trumpet", "Clarinet", "Bassoon"},  { 10 ,30 ,100 ,30 } },
            {   "Choose Mouthpiece",  Parameter::MENU, {"Flute", "Trumpet", "Clarinet", "Bassoon"}, { 120 ,30 ,100 ,30 }  },
            {   "Choose TubeResonator",  Parameter::MENU, {"Flute", "Trumpet", "Clarinet", "Bassoon"}, { 230 ,30 ,100 ,30 }},
            {   "Excitation AB", Parameter::ROTARY, 0.8, 1.2, 1.0, { 10 ,100 ,100 ,70 } },
            {   "Trumpet Distortion",  Parameter::ROTARY, 0.0, 1.0, 0.5, { 100 ,100 ,100 ,70 } },
            {   "Offset (Am2)",  Parameter::ROTARY, 1.0, 1.4, 1.2, { 190 ,100 ,100 ,70 }  },
            //{   "gain LPF",  Parameter::ROTARY, 0.0, 2.0, 1.1, AUTO_SIZE  },
           // {   "Param 7",  Parameter::ROTARY, 0.0, 1.0, 0.0, AUTO_SIZE  },
           // {   "Param 8",  Parameter::ROTARY, 0.0, 1.0, 0.0, AUTO_SIZE  },
           // {   "Param 9",  Parameter::ROTARY, 0.0, 1.0, 0.0, AUTO_SIZE  },
        };

        const Presets PRESETS = {
            { "Preset 1", { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },
            { "Preset 2", { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },
            { "Preset 3", { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },
        };
        
        APDI::Synth* pSynth = new MySynth(CONTROLS, PRESETS, resources);
        pSynth->setSampleRate(sampleRate);

        return pSynth;
    }
}

// Constructor: called when the synth plugin is first created / loaded
MySynth::MySynth(const Parameters& parameters, const Presets& presets, const char* resources)
: Synth(parameters, presets, resources)
{
    for(int n=0; n<32; n++) // create synthesiser's notes
        notes[n] = new MyNote(this);
    
    // Initialise synthesier variables, etc.
}

// Destructor: called when the synthesiser is terminated / unloaded
MySynth::~MySynth()
{
    // Put your own additional clean up code here (e.g. free memory)
    
    for(int n=0; n<32; n++) // delete synthesiser's notes
        delete notes[n];
}

// EVENT HANDLERS: handle different user input (button presses, preset selection, drop menus)

void MySynth::presetLoaded(int iPresetNum, const char *sPresetName)
{
    // A preset has been loaded, so you could perform setup, such as retrieving parameter values
    // using getParameter and use them to set state variables in the plugin
}

void MySynth::optionChanged(int iOptionMenu, int iItem)
{
    // An option menu, with index iOptionMenu, has been changed to the entry, iItem
}

void MySynth::buttonPressed(int iButton)
{
    // A button, with index iButton, has been pressed
}

// Applies audio processing to a buffer of audio
// (inputBuffer contains the input audio, and processed samples should be stored in outputBuffer)
void MySynth::postProcess(const float** inputBuffers, float** outputBuffers, int numSamples)
{
    float fIn0, fIn1, fOut0 = 0, fOut1 = 0;
    const float *pfInBuffer0 = inputBuffers[0], *pfInBuffer1 = inputBuffers[1];
    float *pfOutBuffer0 = outputBuffers[0], *pfOutBuffer1 = outputBuffers[1];
    
//    float fGain = parameters[0];
    
    while(numSamples--)
    {
        // Get sample from input
        fIn0 = *pfInBuffer0++;
        fIn1 = *pfInBuffer1++;
        
        // Add your effect processing here
        fOut0 = fIn0;
        fOut1 = fIn1;
        
        // Copy result to output
        *pfOutBuffer0++ = fOut0;
        *pfOutBuffer1++ = fOut1;
    }
}
