//
//  SynthNote.cpp
//  MySynth Plugin Source Code - for individual notes
//
//  Used to define the bodies of functions used by the plugin, as declared in SynthPlugin.h.
//

#include "SynthNote.h"

//================================================================================
// MyNote - object representing a single note (within the synthesiser - see above)
//================================================================================

// Triggered when a note is started (use to initialise / prepare note processing)
void MyNote::onStartNote (int pitch, float velocity)
{
   attackEG.set(Envelope::Points(0, 0)(0.1, 1));
   attackEG.setLoop(1, 1);
   trumpet.setValues(pitch, velocity);
   flute.setValues(pitch, velocity);
   clarinet.setValues(pitch, velocity);
   bassoon.setValues(pitch, velocity);
}
// Triggered when a note is stopped (return false to keep the note alive)
bool MyNote::onStopNote (float velocity)
{
    flute.stopNote();
    trumpet.stopNote();
    clarinet.stopNote();
    bassoon.stopNote();
    attackEG.release(0.1);
    return false;
}

void MyNote::onPitchWheel (int value){

}
 
void MyNote::onControlChange (int controller, int value){

}


// Called to render the note's next buffer of audio (generates the sound)
// (return false to terminate the note)
bool MyNote::process (float** outputBuffer, int numChannels, int numSamples)
{
    float fBreathInput = 0;
    float fMouthpiece = 0;
    float fTubeResonator = 0;
    float fMix = 0;
    float* pfOutBuffer0 = outputBuffer[0], *pfOutBuffer1 = outputBuffer[1];
   
    if (parameters[0] == 0 && parameters[1] != 0 || parameters[2] != 0) // Allows to has Flute as BreathInput for the rest of instruments
    {
        flute.setGainP(0.50);
    }

    if (parameters[1] == 0 || parameters[1] == 2 || parameters[1] == 3)
    {
        parameters[4] = 0;
    }
   
    trumpet.setBreathInputGain(parameters[3]);
    trumpet.setMouthpieceOffset(parameters[4], parameters[5]);
    flute.setBreathInputGain(parameters[3]);
    flute.setMouthpieceOffset(parameters[4], parameters[5]);
    clarinet.setBreathInputGain(parameters[3]);
    clarinet.setMouthpieceOffset(parameters[4], parameters[5]);
    bassoon.setBreathInputGain(parameters[3]);
   
    while(numSamples--)
    {
      float fFluteBreath = flute.breathInput();
      float fTrumpetBreath = trumpet.breathInput(); 
      float fClarinetBreath = clarinet.breathInput();
      float fBassoonBreath = bassoon.breathInput();

      if (parameters[0] == 0)
      {
          fBreathInput = fFluteBreath;
      }
      else if (parameters[0] == 1)
      {
          fBreathInput = fTrumpetBreath;
      }
     else if (parameters[0] == 2)
     {
         fBreathInput = fClarinetBreath;
     }
     else if (parameters[0] == 3)
      {
          fBreathInput = fBassoonBreath;
      }

      if (parameters[1] == 0)
      {
          fMouthpiece = flute.mouthpiece(fBreathInput);
      }
      else if (parameters[1] == 1)
      {
          fMouthpiece = trumpet.mouthpiece(fBreathInput);
      }
      else if (parameters[1] == 2)
      {
          fMouthpiece = clarinet.mouthpiece(fBreathInput);
      }
      else if (parameters[1] == 3)
      {
          fMouthpiece = bassoon.mouthpiece(fBreathInput);
      }

      if (parameters[2] == 0)
      {
          fMix = flute.tubeResonator(fMouthpiece);
      }
      else if (parameters[2] == 1)
      {
          fMix = trumpet.tubeResonator(fMouthpiece);
      }
      else if (parameters[2] == 2)
      {
          fMix = clarinet.tubeResonator(fMouthpiece);
      }
      else if (parameters[2] == 3)
      {
          fMix = bassoon.tubeResonator(fMouthpiece);
      }

       fMix *= attackEG.tick();
      
        *pfOutBuffer0++ = fMix;
        *pfOutBuffer1++ = fMix;
    }
    
    return flute.isPlaying() || trumpet.isPlaying() || clarinet.isPlaying() || bassoon.isPlaying();
}
