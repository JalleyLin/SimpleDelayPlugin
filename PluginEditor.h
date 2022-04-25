/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SimpleDelay_1AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SimpleDelay_1AudioProcessorEditor (SimpleDelay_1AudioProcessor&);
    ~SimpleDelay_1AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider timeSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> timeSliderAttachment;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimpleDelay_1AudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleDelay_1AudioProcessorEditor)
};
