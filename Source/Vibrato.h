/*
  ==============================================================================

    Vibrato.h
    Created: 24 Mar 2022 11:18:21pm
    Author:  ahmed

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "Utils.h"


class Vibrato {

public:
    std::vector<float> buffer;
    utils::LFO lfo;
    utils::Range range;
    int size, max, wIdx;
    double rIdx, mix;
    float mSampleRate;

    Vibrato(float sampleRate, float freq, float delayTime, float maxDelayTime) {
        mSampleRate = sampleRate;
        buffer.resize(int(delayTime * sampleRate) + 1, 0.0f);
        mix = 1.0f;
        size = int(delayTime * sampleRate);
        max = size - 1;
        wIdx = 0;
        rIdx = 0.0f;
        setDepth(0.4f);
        setFreq(freq);
        setOffset(0.0f);
        setMix(1.0f);
    }

    void process(juce::AudioBuffer<float>& input, int channel) {
        auto* audioInput = input.getWritePointer(channel);
        for (int i = 0; i < input.getNumSamples(); i++) {
            float value = getValue(audioInput[i]);
            audioInput[i] = value;
        }
    }

    
    float getValue(float sample) {
        inc();
        rIdx = lfo.process(range);
        rIdx = wIdx - rIdx;
        if (rIdx < 0)
            rIdx += size;
        buffer[wIdx] = sample;
        float frac = rIdx - int(rIdx);
        float next = buffer[(int(rIdx)+1)% (size)];
        float Out = buffer[int(rIdx)] + frac * (next - buffer[int(rIdx)] );
        return  sample + mix * (Out - sample);
    }
    
    void inc() {
        ++wIdx;
        if (wIdx > max)
            wIdx = 0;
    }

    void setDepth(float depth) {
        range.setRange(0.0f, depth * max);
    }

    void setFreq(float freq) {
        lfo.setFreq(freq, mSampleRate);
    }

    void setOffset(float offset) {
        lfo.setOffset(offset);
    }

    void setMix(float m) {
        mix = m;
    }


private:
};
