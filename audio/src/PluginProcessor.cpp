/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "Voice.h"
#include "HostParam.h"

// UI header, should be hidden behind a factory
#include <PluginEditor.h>

//==============================================================================
PluginAudioProcessor::PluginAudioProcessor()
    : delay(*this)
    , stepSeq(*this)
    , chorus(*this)
    , clip(*this)
    , lowFi(*this)
    , synth(midiState)
{
    for (int i = 0; i < 3; ++i) {
        addParameter(new HostParam<Param>(osc[i].fine));
        addParameter(new HostParam<Param>(osc[i].coarse));
        addParameter(new HostParam<ParamStepped<eOscWaves>>(osc[i].waveForm));
        addParameter(new HostParam<Param>(osc[i].pitchModAmount2));
        addParameter(new HostParam<Param>(osc[i].vol));
        addParameter(new HostParam<Param>(osc[i].panDir));
        addParameter(new HostParam<Param>(osc[i].trngAmount));
        addParameter(new HostParam<Param>(osc[i].pulseWidth));
    }

    for (int i = 0; i < 2; ++i) {
        addParameter(new HostParam<Param>(env[i].attack));
        addParameter(new HostParam<Param>(env[i].decay));
        addParameter(new HostParam<Param>(env[i].sustain));
        addParameter(new HostParam<Param>(env[i].release));
        addParameter(new HostParam<Param>(env[i].attackShape));
        addParameter(new HostParam<Param>(env[i].decayShape));
        addParameter(new HostParam<Param>(env[i].releaseShape));
    }

    for (int i = 0; i < 3; ++i) {
        addParameter(new HostParam<ParamStepped<eLfoWaves>>(lfo[i].wave));
        addParameter(new HostParam<Param>(lfo[i].freq));
        addParameter(new HostParam<ParamStepped<eOnOffToggle>>(lfo[i].tempSync));
        addParameter(new HostParam<Param>(lfo[i].noteLength));
        addParameter(new HostParam<Param>(lfo[i].fadeIn));
    }

    for (int i = 0; i < 2; ++i) {
        addParameter(new HostParam<Param>(filter[i].lpCutoff));
        addParameter(new HostParam<Param>(filter[i].hpCutoff));
        addParameter(new HostParam<Param>(filter[i].resonance));
        addParameter(new HostParam<Param>(filter[i].passtype));
    }

    addParameter(new HostParam<Param>(envVol[0].attack));
    addParameter(new HostParam<Param>(envVol[0].decay));
    addParameter(new HostParam<Param>(envVol[0].release));

    addParameter(new HostParam<Param>(clippingFactor));

    addParameter(new HostParam<Param>(delayFeedback));
    addParameter(new HostParam<Param>(delayDryWet));
    addParameter(new HostParam<Param>(delayTime));

    positionInfo[0].resetToDefault();
    positionInfo[1].resetToDefault();

    addParameter(new HostParam<ParamStepped<eOnOffToggle>>(lowFiActivation));

    /*Create ModMatrixRows here*/
    for (size_t f = 0; f < filter.size(); ++f) {
        String boxName = String::formatted("filter %u", f + 1);
        globalModMatrix.addModMatrixRow(eModSource::eNone, static_cast<destinations>(DEST_FILTER1_LC + f), &filter[f].lpModAmount1, boxName + " lpModSrcBox1");
        globalModMatrix.addModMatrixRow(eModSource::eNone, static_cast<destinations>(DEST_FILTER1_LC + f), &filter[f].lpModAmount2, boxName + " lpModSrcBox2");
        globalModMatrix.addModMatrixRow(eModSource::eNone, static_cast<destinations>(DEST_FILTER1_HC + f), &filter[f].hpModAmount1, boxName + " hpModSrcBox1");
        globalModMatrix.addModMatrixRow(eModSource::eNone, static_cast<destinations>(DEST_FILTER1_HC + f), &filter[f].hpModAmount2, boxName + " hpModSrcBox2");
        globalModMatrix.addModMatrixRow(eModSource::eNone, static_cast<destinations>(DEST_FILTER1_RES + f), &filter[f].resModAmount1, boxName + " resModSrcBox1");
        globalModMatrix.addModMatrixRow(eModSource::eNone, static_cast<destinations>(DEST_FILTER1_RES + f), &filter[f].resModAmount2, boxName + " resModSrcBox2");
    }
    for (size_t o = 0; o < osc.size(); ++o) {
        String boxName = String::formatted("osc %u", o + 1);
        globalModMatrix.addModMatrixRow(eModSource::eNone, static_cast<destinations>(DEST_OSC1_GAIN + o), &osc[o].gainModAmount1, boxName + " GainModSrc1");
        globalModMatrix.addModMatrixRow(eModSource::eNone, static_cast<destinations>(DEST_OSC1_GAIN + o), &osc[o].gainModAmount2, boxName + " GainModSrc2");
        globalModMatrix.addModMatrixRow(eModSource::eNone, static_cast<destinations>(DEST_OSC1_PAN + o), &osc[o].panModAmount1, boxName + " PanModSrc1");
        globalModMatrix.addModMatrixRow(eModSource::eNone, static_cast<destinations>(DEST_OSC1_PAN + o), &osc[o].panModAmount2, boxName + " PanModSrc2");
        globalModMatrix.addModMatrixRow(eModSource::eNone, static_cast<destinations>(DEST_OSC1_PI + o), &osc[o].pitchModAmount1, boxName + " oscPitchModSrc1");
        globalModMatrix.addModMatrixRow(eModSource::eNone, static_cast<destinations>(DEST_OSC1_PI + o), &osc[o].pitchModAmount2, boxName + " oscPitchModSrc2");
        globalModMatrix.addModMatrixRow(eModSource::eNone, static_cast<destinations>(DEST_OSC1_PW + o), &osc[o].shapeModAmount1, boxName + " WidthModSrc1");
        globalModMatrix.addModMatrixRow(eModSource::eNone, static_cast<destinations>(DEST_OSC1_PW + o), &osc[o].shapeModAmount2, boxName + " WidthModSrc2");
    }

    for (size_t e = 0; e < env.size(); ++e) {
        String boxName = String::formatted("env %u", e + 2);
        globalModMatrix.addModMatrixRow(eModSource::eNone, static_cast<destinations>(DEST_ENV2_SPEED + e), &env[e].speedModAmount1, boxName + " envSpeedModSrcBox1");
        globalModMatrix.addModMatrixRow(eModSource::eNone, static_cast<destinations>(DEST_ENV2_SPEED + e), &env[e].speedModAmount2, boxName + " envSpeedModSrcBox2");
    }
    globalModMatrix.addModMatrixRow(eModSource::eNone, static_cast<destinations>(DEST_VOL_ENV_SPEED), &envVol[0].speedModAmount1, "env vol envSpeedModSrcBox1");
    globalModMatrix.addModMatrixRow(eModSource::eNone, static_cast<destinations>(DEST_VOL_ENV_SPEED), &envVol[0].speedModAmount2, "env vol envSpeedModSrcBox1");

    for (size_t l = 0; l < lfo.size(); ++l) {
        String boxName = String::formatted("lfo %u", l + 1);
        globalModMatrix.addModMatrixRow(eModSource::eNone, static_cast<destinations>(DEST_LFO1_FREQ + l), &lfo[l].freqModAmount1, boxName + " freqModSrc1");
        globalModMatrix.addModMatrixRow(eModSource::eNone, static_cast<destinations>(DEST_LFO1_FREQ + l), &lfo[l].freqModAmount2, boxName + " freqModSrc2");
        // LFO Gain is handled in directly @ voice.renderModulation()
    }
}

PluginAudioProcessor::~PluginAudioProcessor()
{
}

//==============================================================================
const String PluginAudioProcessor::getName() const
{
#ifdef JucePlugin_Name
    return JucePlugin_Name;
#else
    // standalone
    return "plugin";
#endif
}

const String PluginAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String PluginAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool PluginAudioProcessor::isInputChannelStereoPair (int index) const
{
    ignoreUnused(index);
    return true;
}

bool PluginAudioProcessor::isOutputChannelStereoPair (int index) const
{
    ignoreUnused(index);
    return true;
}

bool PluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PluginAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double PluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PluginAudioProcessor::setCurrentProgram (int index)
{
    ignoreUnused(index);
}

const String PluginAudioProcessor::getProgramName (int index)
{
    ignoreUnused(index);
    return String();
}

void PluginAudioProcessor::changeProgramName (int index, const String& newName)
{
    ignoreUnused(index,newName);
}

//==============================================================================
void PluginAudioProcessor::prepareToPlay (double sRate, int samplesPerBlock)
{
    ignoreUnused(samplesPerBlock);
    synth.setCurrentPlaybackSampleRate(sRate);
    synth.clearVoices();

    for (int i = 8; --i >= 0;)
    {
        synth.addVoice(new Voice(*this, samplesPerBlock));
    }
    synth.clearSounds();
    synth.addSound(new Sound());

    delay.init(getNumOutputChannels(), sRate);
    chorus.init(getNumOutputChannels(), sRate);
}

void PluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void PluginAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    updateHostInfo();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // I've added this to avoid people getting screaming feedback
    // when they first compile the plugin, but obviously you don't need to
    // this code if your algorithm already fills all the output channels.
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    stepSeq.runSeq(midiMessages, buffer.getNumSamples(), getSampleRate());

    // pass these messages to the keyboard state so that it can update the component
    // to show on-screen which keys are being pressed on the physical midi keyboard.
    // This call will also add midi messages to the buffer which were generated by
    // the mouse-clicking on the on-screen keyboard.
    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

    // and now get the synth to process the midi events and generate its output.
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    // Low fidelity effect
    //////////////////////
    // If the effect is activated, the algorithm is applied
    if (lowFiActivation.getStep() == eOnOffToggle::eOn) {
        lowFi.bitReduction(buffer);
    }

    if (clippingActivation.getStep() == eOnOffToggle::eOn) {
        clip.clipSignal(buffer, 0, buffer.getNumSamples());
    }
    // fx
    // delay
    if (delayActivation.getStep() == eOnOffToggle::eOn) {
        delay.render(buffer, 0, buffer.getNumSamples()); // adds the delay to the outputBuffer
    }
    // chorus
    if (chorActivation.getStep() == eOnOffToggle::eOn) {
        chorus.render(buffer, 0); // adds the chorus to the outputBuffer
    }

    // master volume
    for (int c = 0; c < buffer.getNumChannels(); ++c)
    {
        FloatVectorOperations::multiply(buffer.getWritePointer(c, 0), Param::fromDb(masterAmp.getUI()), buffer.getNumSamples());
    }

    // master pan
    if (buffer.getNumChannels() == 2)
    {
        // Linear pan
        float rightGain = ( (masterPan.get() / 100.f) + 1.f) / 2.f;
        float leftGain = 1.f - rightGain;
        // Constant power pan
        //float p = (float_Pi * ((masterPan.get() / 100.f) + 1.f)) / 4.f;
        //float rightGain = sin(p);
        //float leftGain = cos(p);

        // left
        FloatVectorOperations::multiply(buffer.getWritePointer(0, 0), leftGain, buffer.getNumSamples());
        // right
        FloatVectorOperations::multiply(buffer.getWritePointer(1, 0), rightGain, buffer.getNumSamples());
    }

    //midiMessages.clear(); // NOTE: for now so debugger does not complain
                          // should we set the JucePlugin_ProducesMidiOutput macro to 1 ?
}

void PluginAudioProcessor::updateHostInfo()
{
    // currentPositionInfo used for getting the bpm.
    if (AudioPlayHead* pHead = getPlayHead())
    {
        if (pHead->getCurrentPosition (positionInfo[getAudioIndex()])) {
            positionIndex.exchange(getGUIIndex());
            return;
        }
    }
    positionInfo[getAudioIndex()].resetToDefault();
}

//==============================================================================
bool PluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* PluginAudioProcessor::createEditor()
{
    return new PluginAudioProcessorEditor (*this);
}

//==============================================================================
void PluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    SynthParams::writeXMLPatchHost(destData, eSerializationParams::eAll);
}

void PluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    SynthParams::readXMLPatchHost(data, sizeInBytes, eSerializationParams::eAll);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginAudioProcessor();
}
