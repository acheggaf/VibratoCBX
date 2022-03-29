/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VibratoCBXAudioProcessorEditor::VibratoCBXAudioProcessorEditor (VibratoCBXAudioProcessor& p)
: AudioProcessorEditor (&p), audioProcessor (p), FreqListener(&audioProcessor.freq, &sinwave), DepthListener(&audioProcessor.depth, &rectangles), WidthListener(&audioProcessor.width),
MixListener(&audioProcessor.mix), sinwave(&audioProcessor.freq,90, 60, juce::Colours::black), rectangles(&audioProcessor.width, 3.14159265359f,215, 110, 70, 70, juce::Colours::black),
depthviewer(&audioProcessor.depth,315, 100, 90, 80, 3, juce::Colours::black, 0.9f), DepthValue(&audioProcessor.depth,50,50,font,juce::Colours::black), FreqValue(&audioProcessor.freq, 50, 50, font, juce::Colours::black, " Hz")
, WidthValue(&audioProcessor.width, 50, 50, font, juce::Colours::black), monoStereo("Stereo", "Mono", FacileSans, &audioProcessor.Stereo), mixSliderStyle(juce::Colours::white), MixValue(&audioProcessor.mix, 50, 50, font, juce::Colours::white)
{
    FreqSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    WidthSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    MixSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    DepthSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    
    FreqSlider.setRange(1.0f, 9.0f);
    WidthSlider.setRange(0.0f, 3.14159265359f);
    MixSlider.setRange(0.0f, 1.0f);
    DepthSlider.setRange(0.0f, 0.99f);
    
    FreqSlider.setValue(audioProcessor.freq);
    WidthSlider.setValue(audioProcessor.width);
    MixSlider.setValue(audioProcessor.mix);
    DepthSlider.setValue(audioProcessor.depth);
    
    FreqSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 25, 25);
    WidthSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 25, 25);
    MixSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 25, 25);
    DepthSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 25, 25);
    
    FreqSlider.addListener(&FreqListener);
    MixSlider.addListener(&MixListener);
    DepthSlider.addListener(&DepthListener);
    WidthSlider.addListener(&WidthListener);

    FreqSlider.setLookAndFeel(&minimSliderStyle);
    MixSlider.setLookAndFeel(&mixSliderStyle);
    DepthSlider.setLookAndFeel(&minimSliderStyle);
    WidthSlider.setLookAndFeel(&minimSliderStyle);

    addAndMakeVisible(monoStereo);
    addAndMakeVisible(DepthValue);
    addAndMakeVisible(FreqValue);
    addAndMakeVisible(WidthValue);
    addAndMakeVisible(MixValue);
    addAndMakeVisible(depthviewer);
    addAndMakeVisible(sinwave);
    addAndMakeVisible(rectangles);
    addAndMakeVisible(FreqSlider);
    addAndMakeVisible(WidthSlider);
    addAndMakeVisible(MixSlider);
    addAndMakeVisible(DepthSlider);
    
    setSize (600, 400);
}

VibratoCBXAudioProcessorEditor::~VibratoCBXAudioProcessorEditor()
{
    FreqSlider.setLookAndFeel(nullptr);
    MixSlider.setLookAndFeel(nullptr);
    DepthSlider.setLookAndFeel(nullptr);
    WidthSlider.setLookAndFeel(nullptr);
}

//==============================================================================
void VibratoCBXAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.setImageResamplingQuality(juce::Graphics::ResamplingQuality::highResamplingQuality);
    g.drawImageAt(Background, 0, 0, false);
    font.setHeight(12);
    g.setFont(font);
    g.setColour(juce::Colours::black);
    g.drawText("Frequency", 100, 240, 80, 80,juce::Justification::centred);
    g.drawText("Width", 220, 250, 60, 60, juce::Justification::centred);
    g.drawText("Depth", 330, 250, 60, 60, juce::Justification::centred);
    
    g.setColour(juce::Colours::white);
    //g.drawText("Mix :", 240, 345, 60, 40, juce::Justification::centred);
    font.setHeight(17);
    g.setFont(font);
    //g.drawText("Vibrato", 50, 20, 60, 60, juce::Justification::centred);
}

void VibratoCBXAudioProcessorEditor::resized()
{
    FreqSlider.setBounds(110, 210, 50, 50);
    WidthSlider.setBounds(225, 210, 50, 50);
    DepthSlider.setBounds(335, 210, 50, 50);
    //MixSlider.setBounds(140, 340, 300, 7);
    sinwave.setBounds(90, 115, sinwave.mWidth+4, sinwave.mHeight+5);
    rectangles.setBounds(rectangles.mX, rectangles.mY, rectangles.mWidth, rectangles.mHeight);
    depthviewer.setBounds(depthviewer.mX, depthviewer.mY, depthviewer.mWidth+2, depthviewer.mHeight+10);
    DepthValue.setBounds(335, 175, 60, 40);
    FreqValue.setBounds(115, 175, 60, 40);
    WidthValue.setBounds(225, 175, 60, 40);
    //MixValue.setBounds(275, 340, 60, 40);
    monoStereo.setBounds(450, 18, 90, 15);
    
}
