/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

auto getPhaserRateName() { return juce::String("Phaser Rate Hz"); }
auto getPhaserCenterFreqName() { return juce::String("Phaser Center Freq Hz"); }
auto getPhaserDepthName() { return juce::String("Phaser Depth %"); }
auto getPhaserFeedbackName() { return juce::String("Phaser Feedback %"); }
auto getPhaserMixName() { return juce::String("Phaser mix %"); }

auto getChoursRateName() { return juce::String("Chorus Rate Hz"); }
auto getChorusDepthName() { return juce::String("Chorus Depth %"); }
auto getChorusCenterDelayName() { return juce::String("Chorus Center delay Ms"); }
auto getChorusFeedbackName() { return juce::String("Chorus Feedback %"); }
auto getChorusMixName() { return juce::String("Chorus mix %"); }

//==============================================================================
MultieffectsAudioProcessor::MultieffectsAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    auto floatParams = std::array{
        &phaserRateHz,
        &phaserCenterFreqHz,
        &phaserDepthPercent,
        &phaserFeedbackPercent,
        &phaserMixPercent
    };
    auto floatNameFuncs = std::array{
         &getPhaserRateName,
         &getPhaserCenterFreqName,
         &getPhaserDepthName,
         &getPhaserFeedbackName,
         &getPhaserMixName,
    };

    for (size_t i = 0; i < floatParams.size(); ++i) {
        auto ptrtoParamPtr = floatParams[i];
       *ptrToParamPtr= dynamic_cast<juce::AudioParamterFloat*>(aptvs.getParameter(flaotNameFuncs[i]()));

       jassert(*ptrToParamPtr != nullptr);
    
    }

}

MultieffectsAudioProcessor::~MultieffectsAudioProcessor()
{
}

//==============================================================================
const juce::String MultieffectsAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MultieffectsAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MultieffectsAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MultieffectsAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MultieffectsAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MultieffectsAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MultieffectsAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MultieffectsAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MultieffectsAudioProcessor::getProgramName (int index)
{
    return {};
}

void MultieffectsAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MultieffectsAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void MultieffectsAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MultieffectsAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

juce::AudioProcessorValueTreeState::ParameterLayout MultieffectsAudioProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

  /*  name = naemFunction();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name, versionHint },
        name,
        parameterRange,
        defaultValue,
        unitSuffix,
    )*/

    const int versionHint = 1;

    auto name = getPhaserRateName();//calls the function we made before
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name, versionHint },
        name,                          //range freq in hertz(min,max,rate  of  change, skew factor
        juce::NormalisableRange<float>(0.01f, 2.f, 0.01f, 1.f),
        0.2f, //the default value
        "Hz"
    ));

    auto name = getPhaserDepthName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name, versionHint },
        name,                         
        juce::NormalisableRange<float>(0.01f, 1.f, 0.01f, 1.f),
        0.05f,
        "%"
    ));

    auto name = getPhaserCenterFreqName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name, versionHint },
        name,
        juce::NormalisableRange<float>(20.f, 2000.f, 1.f, 1.f),
        1000.f,
        "Hz"
    ));

    auto name = getPhaserFeedbackName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name, versionHint },
        name,
        juce::NormalisableRange<float>(-1.f, 1.f, 0.01f, 1.f),
        0.0f,
        "%"
    ));

    auto name = getPhaserMixName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name, versionHint },
        name,
        juce::NormalisableRange<float>(0.01f, 1.f, 0.01f, 1.f),
        0.05f,
        "%"
    ));

    /*chorus
    rate hz
    depth 0 to 1
    center delay ms 1 to 100
    feedback -1 to 1 
    mix:0 to 1*/ 




    return layout;
}

void MultieffectsAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    //TODO
    //add apvts-done
    //update dsp here from audio params 
    //save/load settings
    // save/load dsp order
    //drag to reorder gui
    //gui design for each
    //metering
    //prepare all dsp
    //stereo 
    //video has more as bonuses, maybe later POST


    auto newDSPOrder = DSP_Order();

    //trying to pull
    while (dspOrderFifo.pull(newDSPOrder)) {

    }

        //if pulled, replace dsp order
    if (newDSPOrder != DSP_Order()) 
        DSP_Order = newDSPOrder;

    //connverts dspOrder into an array of pointers
    DSP_Pointers dspPointers;
    
    for (size_t i = 0; i < dspPointers.size(); ++i) {
        switch (dspOrder[i])
        {
        case DSP_Options::Phase:
            dspPointers[i] = &phaser;
                break;
        case DSP_Options::Chorus:
            dspPointers[i] = &chorus;
                break;
        case DSP_Options::OverDrive:
            dspPointers[i] = &overdrive;
                break;
        case DSP_Options::LadderFilter:
            dspPointers[i] = &ladderFilter;
                break;
        case DSP_Options::END_OF_LIST:
            jassertfalse
                break;
         
        }

        //processing(making a block and a context to be manipulated)
        auto block = juce::dsp::AudioBlock<float>(buffer);
        auto context juce::dsp::ProcessContextReplacing<float>(block);
        for (size_t i = 0; i < dspPointers.size(); ++i) {
            if (dspPointers[i] != nullptr) {
                dspPointers[i]->process(context);
             }
        }

    }


    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool MultieffectsAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MultieffectsAudioProcessor::createEditor()
{
    return new MultieffectsAudioProcessorEditor (*this);
}

//==============================================================================
void MultieffectsAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MultieffectsAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MultieffectsAudioProcessor();
}
