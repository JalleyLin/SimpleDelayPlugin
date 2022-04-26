/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleDelay_1AudioProcessorEditor::SimpleDelay_1AudioProcessorEditor (SimpleDelay_1AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    timeSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    timeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(timeSlider);
    
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(gainSlider);
    
    decayRateSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    decayRateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(decayRateSlider);
    
    setSize (400, 300);

    timeSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "delayTime", timeSlider);
    
    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"delayGain",gainSlider);
    
    decayRateSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"delayRate",decayRateSlider);
}

SimpleDelay_1AudioProcessorEditor::~SimpleDelay_1AudioProcessorEditor()
{
}

//==============================================================================
void SimpleDelay_1AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);
    g.setFont(50.0f);
    g.drawText("Decay Time", timeSlider.getBounds(), juce::Justification::centredTop, true);
    
}

void SimpleDelay_1AudioProcessorEditor::resized()
{
    timeSlider.setBounds(getWidth()/2 - 50, getHeight()/2 - 50, 100, 100);
    gainSlider.setBounds(getWidth()/2 - 150, getHeight()/2 - 50, 100, 100);
    decayRateSlider.setBounds(getWidth()/2 + 50 , getHeight()/2 - 50, 100, 100);
}
