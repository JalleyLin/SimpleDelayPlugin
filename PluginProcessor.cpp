/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleDelay_1AudioProcessor::SimpleDelay_1AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),apvts(*this,nullptr,"Parameters",createParameterLayout())
#endif
{
}

SimpleDelay_1AudioProcessor::~SimpleDelay_1AudioProcessor()
{
}

//==============================================================================
const juce::String SimpleDelay_1AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleDelay_1AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SimpleDelay_1AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SimpleDelay_1AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SimpleDelay_1AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleDelay_1AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SimpleDelay_1AudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleDelay_1AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SimpleDelay_1AudioProcessor::getProgramName (int index)
{
    return {};
}

void SimpleDelay_1AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SimpleDelay_1AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    auto delayBuffferSize = sampleRate * 40; // modefied to 40!
    delayBuffer.setSize(getTotalNumInputChannels(), (int)delayBuffferSize);
}

void SimpleDelay_1AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimpleDelay_1AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void SimpleDelay_1AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    auto bufferSize = buffer.getNumSamples();
    auto delayBufferSize = delayBuffer.getNumSamples();
    
//    int count = 4; //times of echo appearing
    
    auto g = 0.2f;
    
    auto g20 = 0.05f;
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        
        filleBuffer(buffer, channel, bufferSize, delayBufferSize);
        
        //read from the past in the delay buffer, then add it back to the original signal
        auto tm = apvts.getRawParameterValue("delayTime");
        float delayTime = tm->load();
//        std::cout << delayTime << std::endl;
        
        for (int count = 1; count <= 5; count++)
        {
            
            auto readPosition = writePosition - getSampleRate() * delayTime * count ;  //延时时间点主要变量
                
            if (readPosition < 0)
            {
                readPosition += delayBufferSize;
            }
                
                
            if (readPosition + bufferSize < delayBufferSize)
            {
                buffer.addFromWithRamp(channel, 0, delayBuffer.getReadPointer(channel, readPosition), bufferSize, g, g);
            }
            else
            {
                auto numSampleToEnd = delayBufferSize - readPosition;
                buffer.addFromWithRamp(channel, 0, delayBuffer.getReadPointer(channel, readPosition), numSampleToEnd, g, g);
                
                auto numSampleToStart = bufferSize - numSampleToEnd;
                buffer.addFromWithRamp(channel, numSampleToEnd, delayBuffer.getReadPointer(channel, 0), numSampleToStart, g, g);
            }
            
            g -= g20;
        }
            
        
//        filleBuffer(buffer, channel, bufferSize, delayBufferSize);
        
    }
    //update buffer positions
    writePosition += bufferSize;
    writePosition %= delayBufferSize;
}

void SimpleDelay_1AudioProcessor::filleBuffer(juce::AudioBuffer<float>& buffer ,int channel, int bufferSize, int delayBufferSize, float g)
{
    //copy input signal to a delay buffer
    auto* channelData = buffer.getWritePointer (channel);
    
    if (delayBufferSize > bufferSize + writePosition)
    {
//        delayBuffer.copyFromWithRamp(channel, writePosition, channelData, bufferSize, 0.1f, 0.1f);
        delayBuffer.copyFrom(channel, writePosition, channelData, bufferSize, g);
    }
    else
    {
        auto numSampleToEnd = delayBufferSize - writePosition;
        auto numSampleAtStart = bufferSize -numSampleToEnd;
        
//        delayBuffer.copyFromWithRamp(channel, writePosition, channelData, numSampleToEnd, 0.1f, 0.1f);
//        delayBuffer.copyFromWithRamp(channel, 0, channelData, numSampleAtStart, 0.1f, 0.1f);
        
        delayBuffer.copyFrom(channel, writePosition, channelData, numSampleToEnd, g);
        delayBuffer.copyFrom(channel, 0, channelData, numSampleAtStart, g);
    }
}

void SimpleDelay_1AudioProcessor::readBuffer(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer, int channel, int bufferSize, int delayTime, float gain)
{

}

//==============================================================================
bool SimpleDelay_1AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SimpleDelay_1AudioProcessor::createEditor()
{
    return new SimpleDelay_1AudioProcessorEditor (*this);
}

//==============================================================================
void SimpleDelay_1AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SimpleDelay_1AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleDelay_1AudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout SimpleDelay_1AudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>("delayTime","delay time", 0.0f, 2.0f, 1.0f));
    return layout;
}
