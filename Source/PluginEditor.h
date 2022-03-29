/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Utils.h"

//==============================================================================
/**
*/

class SliderListenerRepaintComponent : public juce::Slider::Listener
{
public:
    float* param;
    juce::Component* sw;

    SliderListenerRepaintComponent(float* p, juce::Component* s)
    {
        sw = s;
        param = p;
    }

    void sliderValueChanged(juce::Slider* slider) override
    {
        (*param) = slider->getValue();
        sw->repaint();
    }


private:
};

class SliderMinim : public juce::Slider::Listener
{
public:
    float* param;
    
    SliderMinim(float* p)
    {
        param = p;
    }
    
    void sliderValueChanged (juce::Slider* slider) override
    {
        (*param) = slider->getValue();
    }
    
    
private:
};

class VibratoCBXAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    VibratoCBXAudioProcessorEditor (VibratoCBXAudioProcessor&);
    ~VibratoCBXAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VibratoCBXAudioProcessor& audioProcessor;
    
    juce::Image Background = juce::ImageCache::getFromMemory(BinaryData::VibratoCBX_jpg, BinaryData::VibratoCBX_jpgSize);
    juce::Font font = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::COPASETI_TTF,
        BinaryData::COPASETI_TTFSize));
    juce::Font FacileSans = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::FacileSans_otf,
        BinaryData::FacileSans_otfSize));

    juce::Slider FreqSlider;
    juce::Slider DepthSlider;
    juce::Slider WidthSlider;
    juce::Slider MixSlider;
    
    utils::LinearMinimSliderStyle mixSliderStyle;
    utils::ToggleButton monoStereo;

    utils::TextBox DepthValue;
    utils::TextBox FreqValue;
    utils::TextBox WidthValue;
    utils::TextBox MixValue;

    utils::SinWave sinwave;
    utils::RectangleWidth rectangles;
    utils::DepthViewer depthviewer;
    utils::MinimalisticSliderStyle minimSliderStyle;


    SliderListenerRepaintComponent FreqListener;
    SliderListenerRepaintComponent DepthListener;
    SliderMinim WidthListener;
    SliderMinim MixListener;
        
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VibratoCBXAudioProcessorEditor)
};
