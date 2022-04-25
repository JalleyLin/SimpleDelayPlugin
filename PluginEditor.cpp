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
    
    setSize (400, 300);

    timeSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "delayTime", timeSlider);
}

SimpleDelay_1AudioProcessorEditor::~SimpleDelay_1AudioProcessorEditor()
{
}

//==============================================================================
void SimpleDelay_1AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);

}

void SimpleDelay_1AudioProcessorEditor::resized()
{
    timeSlider.setBounds(getWidth()/2 - 100, getHeight()/2 - 50, 200, 100);
}
