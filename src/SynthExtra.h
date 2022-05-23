//
//  SynthExtra.h
//  Additional Plugin Code
//
//  This file is a workspace for developing new DSP objects or functions to use in your plugin.
//

class Trumpet
{
public:
    Trumpet() //Constructor 
    {
        fFrequency = 0.f;
        fLevel = 0.f;
        fDelay = 0.f;
        tremoloDepth = 0.f;
        tremoloOffset = 0.f;
        GainP = 0.f;
        GainN = 0.f;
        GainM1 = 0.f;
        GainM2 = 0.f;
        GainAB = 0.f;
        GainAr = 0.f;
        GainR1 = 0.f;
        GainR2 = 0.f;
        fSignalDelay = 0.f;
        gainBreathStage = 0.f;
        offsetM1 = 0.f;
        offsetM2 = 0.f;
    }
    ~Trumpet() //Destructor
    {

    }
   
    void setBreathInputGain(float param3)
    {
         gainBreathStage = param3; //Breath Input into Mouthpiece. (Gain ab) Range 0.8 - 1.2  Parameters[3]
    }

    void setMouthpieceOffset(float param4, float param5)
    {
        offsetM1= param4; //Trumpet Distortion. Range 0.0 - 1.0  Parameters[4]
        offsetM2 = param5; //Offset before Distortion. Range 1.0 - 1.4  Parameters[5]
    }
    
    void setValues(int pitch, float velocity)
    {
        fFrequency = 440.f * pow(2.f, (pitch - 69.f) / 12.f);
        fLevel = velocity;

        breathEG.set(Envelope::Points(0, 0)(0.1, 1)); //Attack of 0.1s
        breathEG.setLoop(1, 1);

        tubeDelay.setMaximumDelay(getSampleRate()); // delay max of 1 second
        tubeDelay.clear();
        fDelay = 1.0 / (2 * fFrequency); // in seconds
        fDelay *= getSampleRate(); // Change the seconds to samples
        tubeDelay.setDelay(fDelay);

        tremolo.reset();

        GainP = 1; 
        GainN = 0.3;

        tremoloDepth = 0.8; 
        tremoloOffset = 1.5; 

        filterNoise.set(fFrequency, 20);

        //Tube Resonator Stage
        GainAr = 0.7; //from 1.2 it start to be unestable

        bandFilterOut.set(fFrequency, 2000);
        lowFilterOut.setCutoff(fFrequency);
        GainR1 = 0.8;
        GainR2 = 0.3;
        removeHighFilter.setCutoff(fFrequency + 100);

        outFilter.setCutoff(50);
                
        tremolo.setFrequency(fFrequency / 2);

    }

    void stopNote()
    {
        breathEG.release(0.1);
    }

    float breathInput()
    {
        ////Breath Stage
        float tremoloMod = tremolo.tick();
        tremoloMod *= tremoloDepth + tremoloOffset;

        float fBreath = breathEG.tick() * tremoloMod;
        float fNoise = breathNoise.tick();
        fNoise = filterNoise.tick(fNoise);
        float fBreathNoise = fBreath * fNoise;

        fBreath *= GainP;

        fBreathNoise *= GainN;

        float fBreathOut = fBreath + fBreathNoise;
        fBreathOut *= gainBreathStage;
        return fBreathOut;
    }
    
    float mouthpiece(float fBreath)
    {
        fBreath += fSignalDelay;
        ////Mouthpiece Stage
        float fBreathinvert = fBreath * -1; //Save this value for the end of the Mouthpiece (after clipping)

        float fMouth = fBreath * offsetM1;
        fMouth += offsetM2;
        fMouth *= fBreathinvert;

        ////Tube resonator Stage
        fSignalDelay = tubeDelay.tapOut(fDelay);
        return fMouth;
    }
      
    float tubeResonator(float fTubeInput) 
    {
        float fMouthLPF = fTubeInput;
        float fMouthBPF = fTubeInput;
        float lowPassFiltered = lowFilterOut.tick(fMouthLPF);
        float bandPassFiltered = bandFilterOut.tick(fMouthBPF);
        lowPassFiltered *= GainR2;
        bandPassFiltered *= GainR1;
        float fTubeSignal = lowPassFiltered + bandPassFiltered;
        fTubeSignal = removeHighFilter.tick(fTubeSignal);
        fTubeSignal *= GainAr;
        fTubeSignal = tubeDelay.tick(fTubeSignal);
        float fMixTrumpet = outFilter.tick(fTubeSignal);
        fMixTrumpet *= fLevel;                 // (velocity)

        return fMixTrumpet;
    }

    bool isPlaying() {
        return breathEG.getStage() != Envelope::STAGE::ENV_OFF;
    }

private:
    float fFrequency;
    float fLevel;
    float fDelay;
    float gainBreathStage;
    float offsetM1;
    float offsetM2;

    //Breath Stage
    Envelope breathEG;
    Saw tremolo;
    Noise breathNoise;
    BPF filterNoise;

    //Tube Resonator
    Delay tubeDelay;
    BPF bandFilterOut;
    LPF lowFilterOut;
    LPF removeHighFilter;
    HPF outFilter;

    //Variables
    float tremoloDepth;
    float tremoloOffset;
    float GainP;
    float GainN;
    float GainM1;
    float GainM2;
    float GainAB;
    float GainAr;
    float fSignalDelay;
    float GainR1;
    float GainR2;
};

//////////////////////////////////////////////FLUTE CLASS///////////////////////////////////////////////////////////////
class Flute
{
public:
    Flute() { // constructor
        fFrequency = 0.f;
        fLevel = 0.f;
        fDelay = 0.f;
        tremoloDepth = 0.f;
        tremoloOffset = 0.f;
        GainM1 = 0.f;
        GainM2 = 0.f;
        GainAB = 0.f;
        GainAr = 0.f;
        fSignalDelay = 0.f;
        GainR1 = 0.f;
        GainR2 = 0.f;
        GainP = 0.f;
        GainN = 0.f;
        gainBreathStage = 0.f;
        offsetM1 = 0.f;
        offsetM2 = 0.f;
    }

    ~Flute() { // destructor

    }

    void setBreathInputGain(float param3)
    {
        gainBreathStage = (param3 * 3) - 1.6; //Breath Input into Mouthpiece. (Gain ab) Range 0.8 - 2.0  Parameters[3]   
    }
    void setMouthpieceOffset(float param4, float param5)
    {
        offsetM1 = 1.5;
        offsetM2 = param5; //Offset before Distortion Parameters[5]
    }

    void setGainP(float moreGainFlute)
    {
      GainP  = moreGainFlute; //Additional gain to allows hybrids with flute Breath Input
    }

    void setValues(int pitch, float velocity)
    {
        // convert note number to fundamental frequency (Hz)
        fFrequency = 440.f * pow(2.f, (pitch - 69.f) / 12.f);
        fLevel = velocity;                      // store velocity


        breathEG.set(Envelope::Points(0, 0)(0.1, 1)); //Attack of 0.1s
        breathEG.setLoop(1, 1);

        tubeDelay.setMaximumDelay(getSampleRate()); // delay max of 1 second
        tubeDelay.clear();
        fDelay = 1.0 / (2 * fFrequency); // in seconds
        fDelay *= getSampleRate(); // Change the seconds to samples
       
                                   //Previous trials of tuning:
                                   // fDelay += 1307.5 * pow(2.71828182845904523536, -0.076 * pitch);
                                   // fDelay -= 0.0321 * (pitch * pitch) - 5.4301 * pitch + 233.9;
        tubeDelay.setDelay(fDelay);

        tremolo.reset();
               
        //Breath Stage
        GainP = 0.012; 
        GainN = 0.3; // 0 to 0.3

        tremolo.setFrequency(fFrequency);
        tremoloDepth = 0.1;
        tremoloOffset = 1;

       // from 1K to 4K Hz
        filterNoise.set(fFrequency, 20);

        //Tube Resonator Stage
        GainAr = 1; //from 1.2 it start to be unestable
        
        bandFilterOut.set(fFrequency, 1000);
        lowFilterOut.setCutoff(fFrequency);
        GainR1 = 0.65;
        GainR2 = 1.1;
        removeHighFilter.setCutoff(fFrequency);

        outFilter.setCutoff(50);
    }

    // Clipping function
    float HardClip(float input) {

        if (input > 1)
            return 1;
        else if (input < -1)
            return -1;
        else
            return input;
    }
    
    void stopNote()
    {
        breathEG.release(0.1);
    }

    float breathInput()
    {
        //Breath Stage
        float tremoloMod = tremolo.tick();
        tremoloMod *= tremoloDepth + tremoloOffset;
        float fBreath = breathEG.tick() * tremoloMod;
        float fNoise = breathNoise.tick();
        fNoise = filterNoise.tick(fNoise);
        float fBreathNoise = fBreath * fNoise;

        fBreath *= GainP;
        fBreathNoise *= GainN;
        float fBreathOut = fBreath + fBreathNoise;

        fBreathOut *= gainBreathStage;
        return fBreathOut;
    }
   
    float mouthpiece(float fBreath)
    {
        fBreath += fSignalDelay;

        //Mouthpiece Stage
        float fBreathinvert = fBreath * -1; //Save this value for the end of the Mouthpiece (after clipping)

        float fMouth = fBreath * 1.5; //M1
        fMouth += offsetM2;

        // Clipping & Shaping function  
        float fMouthClipped = HardClip(fMouth);
                                                    // float fMouthShaped = SoftClip(fMouthClipped, fCurve);
        fMouthClipped *= fBreathinvert;

        //Tube resonator Stage
        fSignalDelay = tubeDelay.tapOut(fDelay);
        return fMouthClipped;
    }
           
    float tubeResonator(float fTubeInput)  
    {
        float bandFiltered = bandFilterOut.tick(fTubeInput);
        bandFiltered *= GainR1;
        float lowFiltered = lowFilterOut.tick(fTubeInput);
        lowFiltered *= GainR2;
        float fTubeSignal = lowFiltered + bandFiltered;
        fTubeSignal = removeHighFilter.tick(fTubeSignal);
        fTubeSignal *= GainAr;
        fTubeSignal = tubeDelay.tick(fTubeSignal);

        float fMixFlute = outFilter.tick(fTubeSignal);
        fMixFlute *= fLevel;                 // apply gain (velocity)

        return fMixFlute;
    }   

    bool isPlaying() {
        return breathEG.getStage() != Envelope::STAGE::ENV_OFF;
    }
   
private:
    float fFrequency;
    float fLevel;
    float fDelay;
    float gainBreathStage;
    float offsetM1;
    float offsetM2;

    //Breath Stage
    Envelope breathEG;
    Sine tremolo;
    Noise breathNoise;
    BPF filterNoise;
    
    //Tube Resonator
    Delay tubeDelay; 
    BPF bandFilterOut;
    LPF lowFilterOut;
    LPF removeHighFilter;
    HPF outFilter;

    //Variable
    float tremoloDepth;
    float tremoloOffset;
    float GainM1;
    float GainM2;
    float GainAB;
    float GainAr;
    float fSignalDelay;
    float GainR1;
    float GainR2;
    float GainP;
    float GainN;
};

class Clarinet
{
public:
    Clarinet()
    {
       
        tremoloDepth = 0.f;
        tremoloOffset = 0.f;
        GainM1 = 0.f;
        GainM2 = 0.f;
        GainAB = 0.f;
        GainAr = 0.f;
        fSignalDelay = 0.f;
        GainR1 = 0.f;
        GainR2 = 0.f;
        GainP = 0.f;
        GainN = 0.f;
        fFrequency = 0.f;
        fLevel = 0.f;
        fDelay = 0.f;
        gainBreathStage = 0.f;
        offsetM1 = 0.f;
        offsetM2 = 0.f;
    }
    
    ~Clarinet() //Destructor
    {
    }

    void setBreathInputGain(float param3)
    {
        gainBreathStage = param3; //Breath Input into Mouthpiece. (Gain ab) Range 0.8 - 1.2  Parameters[3]
    }

    void setMouthpieceOffset(float param4, float param5)
    {
        offsetM1 = 1; 
        offsetM2 = param5; //Offset before Distortion Parameters[5]
    }

    void setValues(int pitch, float velocity)
    {
        fFrequency = 440.f * pow(2.f, (pitch - 69.f) / 12.f);
        std::cout << "frequency is: " << fFrequency << "\n";
        fLevel = velocity;                      // store velocity

        breathEG.set(Envelope::Points(0, 0)(0.1, 1)); //Attack of 0.1s
        breathEG.setLoop(1, 1);

        tubeDelay.setMaximumDelay(getSampleRate()); // delay max of 1 second
        tubeDelay.clear();
        fDelay = 1.0 / (2 * fFrequency); // in seconds
        fDelay *= getSampleRate(); // Change the seconds to samples 
        tubeDelay.setDelay(fDelay);

        tremolo.reset();

        Envelope triangle;
        triangle.set(Envelope::Points(0, 0)(0.025, 1)(0.075, -1)(0.1, 0));
        lips = triangle;
        lips.setFrequency(fFrequency);

        //Breath Stage Setting
        GainP = 1; 
        GainN = 0.3; // 0 to 0.3

        tremoloDepth = 0.8; 
        tremoloOffset = 1.5;

       // from 1K to 4K Hz
        filterNoise.set(fFrequency, 50);        

        //Tube Resonator Stage
        GainAr = 0.9; //from 1.2 it start to be unestable      

        bandFilterOut.set(fFrequency, 2000);
        lowFilterOut.setCutoff(fFrequency);
        GainR1 = 0.8;
        GainR2 = 0.3;
        removeHighFilter.setCutoff(fFrequency + 100);

        outFilter.setCutoff(50);
    }

    void stopNote()
    {
        breathEG.release(0.1);
    }

    // Clipping function
    float HardClip(float input) {

        if (input > 1)
            return 1;
        else if (input < -1)
            return -1;
        else
            return input;
    }

    float breathInput()
    {
        tremolo.setFrequency(fFrequency / 2);
        float tremoloMod = tremolo.tick();
        tremoloMod *= tremoloDepth + tremoloOffset;

        float fBreath = breathEG.tick() * tremoloMod;
        float noise = breathNoise.tick();
        noise = filterNoise.tick(noise);
        float fBreathNoise = fBreath * noise;

        fBreath *= GainP;

        fBreathNoise *= GainN;

        float BreathOut = fBreath + fBreathNoise;
        BreathOut *= gainBreathStage;
        BreathOut *= lips.tick();
        return BreathOut;
    }

    float mouthpiece(float fBreath)
    {
        fBreath += fSignalDelay;

        ////Mouthpiece Stage
        float fBreathinvert = fBreath * -1; //Save this value for the end of the Mouthpiece (after clipping)

        float fMouth = fBreath * offsetM1;
        fMouth += offsetM2;
        float fMouthClipped = HardClip(fMouth);
     
        ////Tube resonator Stage
        fSignalDelay = tubeDelay.tapOut(fDelay);
        return fMouthClipped;
    }

    float tubeResonator(float fTubeInput)
    {
        float fMouthLPF = fTubeInput;
        float fMouthBPF = fTubeInput;

        float lowPassFiltered = lowFilterOut.tick(fMouthLPF);
        float bandPassFiltered = bandFilterOut.tick(fMouthBPF);
        lowPassFiltered *= 0.3;
        bandPassFiltered *= 0.8;
        float tubeSignal = lowPassFiltered + bandPassFiltered;
        tubeSignal = removeHighFilter.tick(tubeSignal);
        tubeSignal *= GainAr;
        tubeSignal = tubeDelay.tick(tubeSignal);
        float fMixClarinet = outFilter.tick(tubeSignal);
        fMixClarinet *= fLevel;                 // apply gain (velocity)
        fMixClarinet *= 2;
        return fMixClarinet;
    }
   
    bool isPlaying() {
        return breathEG.getStage() != Envelope::STAGE::ENV_OFF;
    }
private:
    float fFrequency;
    float fLevel;
    float fDelay;
    float GainR1;
    float GainR2;
    float GainP;
    float GainN;
    float tremoloDepth;
    float tremoloOffset;
    float GainAB;
    float GainM1;
    float GainM2;
    float fSignalDelay;
    float GainAr;
    float gainBreathStage;
    float offsetM1;
    float offsetM2;

    //Breath Stage
    Envelope breathEG;
    LPF tubeFilter;
    Square tremolo;
    Noise breathNoise;
    BPF filterNoise;
    Wavetable lips;

    //Tube Resonator
    Delay tubeDelay;
    BPF bandFilterOut;
    LPF lowFilterOut;
    LPF removeHighFilter;
    HPF outFilter; 
};

class Bassoon
{
public:
    Bassoon() //Constructor
    {
        fFrequency = 0;
        fLevel = 0;

        fDelay = 0;
        fDelayReed = 0;
        fBreath = 0;
        vibratoDepth = 0;
        vibratoOffset = 0;
        fBelltoReed = 0;
        GainM1 = 0;
        GainM2 = 0;
        fDelayLines = 0;
        fMix = 0;
        gainBreathStage = 0.f; 
    }

    ~Bassoon() // Destructor
    {

    }
    void setValues(int pitch, float velocity)
    {
        fFrequency = 440.f * pow(2.f, (pitch - 69.f) / 12.f);
        fFrequency *= 0.5;
        fLevel = velocity;                      // store velocity

        breathEG.set(Envelope::Points(0, 0)(0.1, 1)); //Attack of 0.1s
        breathEG.setLoop(1, 1);

        fDelay = 1.0 / (2 * fFrequency); // in second
        fDelay *= getSampleRate();
        fDelayReed = fDelay / 4;
        reedDelay.setDelay(fDelayReed);
        bellDelay.setDelay(fDelay);

        reflecFilter.setCutoff(500);
        filterNoise.setCutoff(4000);
        vibrato.setFrequency(fFrequency);
        vibratoDepth = 0.1;
        vibratoOffset = 1;
        fBelltoReed = 0.f;
        GainM1 = 1;
        GainM2 = 1;
        fDelayLines = 0.f;
        filterBell.setCutoff(50);
        
    }
    void setBreathInputGain(float param3)
    {
        gainBreathStage = param3; //Breath Input into Mouthpiece. (Gain ab) Range 0.8 - 1.2  Parameters[3]
    }
  
    void stopNote() 
    {
        breathEG.release(0.1);
    }

    float breathInput()
    {
        //BREATH STAGE
        fBreath = breathEG.tick();

        float vibratoMod = vibrato.tick();
        vibratoMod *= vibratoDepth + vibratoOffset;
        fBreath *= vibratoMod;
        float fNoise = noise.tick();
        fNoise *= 0.3;
        fNoise = filterNoise.tick(fNoise);

        float fBreathNoise = fBreath * fNoise;
        fBreathNoise *= 0.01; //Gain N
        fBreath *= 0.5; //Gain P
        float BreathtoReed = fBreath + fBreathNoise;
        BreathtoReed *= gainBreathStage; //Gain AB
        return BreathtoReed;
    }

    float mouthpiece(float fBreath)
    {
        //REED AND BELL
        fBreath += fDelayLines;

        float fBreathinvert = fBreath * -1;

        float fReed = fBreath * GainM1;
        fReed += GainM2;
        fReed *= fBreathinvert;
        float fBell = reflecFilter.tick(fReed);
        fBelltoReed = bellDelay.tapOut(fDelay);
        float reedtoBell = reedDelay.tapOut(fDelayReed);
        fDelayLines = fBelltoReed + reedtoBell;
        return fBell;
    }

    float tubeResonator(float fTubeInput)
    {
        float output = bellDelay.tick(fTubeInput); 
        output = filterBell.tick(output);
        fMix = output;  
        fMix *= fLevel;                 // apply gain (velocity)
        return fMix;
    }

    bool isPlaying() {
        return breathEG.getStage() != Envelope::STAGE::ENV_OFF;
    }
private:
    float fFrequency;
    float fLevel;
    float fDelay;
    float fDelayReed;
    float fBreath;
    float vibratoDepth;
    float vibratoOffset;
    float fBelltoReed;
    float GainM1;
    float GainM2;
    float fDelayLines;
    float fMix;
    float gainBreathStage;
    Envelope breathEG;
    Delay reedDelay;
    Delay bellDelay;
    LPF reflecFilter;
    Noise noise;
    LPF filterNoise;
    Sine vibrato;
    HPF filterBell;
};