/*
  ==============================================================================

    Utils.h
    Created: 24 Mar 2022 11:18:09pm
    Author:  ahmed

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

namespace utils
{
    class Range
    {
    public :
        float TAU{6.28318530718f};
        float start,end,interval;
        Range(double s = 0, float e = 0) {
            start = s;
            end = e;
        }

        void setRange(double s, double e) {
            start = s;
            end = e;
            interval = end - start;
        }

        float getInterval() {
            return interval;
        }
        float getStart() {
            return start;
        }
        float getEnd() {
            return end;
        }
    private :
    };

    class Osc {
    public:
        float TAU{6.28318530718f};
        float phase{ 0.0f };
        float offset{ 0.0f };
        float inc{ 0.0f };

        float process() {
            phase += inc;
            if (phase > TAU)
                phase -= TAU;
            return std::sin(phase+offset);
        }
        
        void setFreq(float freq, float sampleRate) {
            inc = TAU * freq / sampleRate;
        }

        void setOffset(double Offset) {
            offset = Offset * (TAU * 0.5f);
        }

    private:
    };

    class LFO {
    public:
        float TAU{6.28318530718f};
        Osc osc;

        void setFreq(float freq, float sampleRate) {
            
            osc.setFreq(freq, sampleRate);
        }

        void setOffset(float offset) {
            osc.setOffset(offset);
        }

        float process(Range& range) {
            return range.start + range.interval * (osc.process() * 0.5f + 0.5f);
        }

    private:
    };

    class SinWaveViewer : public juce::Component, juce::Timer
    {
    public:
        int mSteps{ 1 };
        int mWidth, mHeight;
        juce::Colour mColour;
        std::vector<float> mValues;
        Range range;
        int wIdx{0};
        LFO mLFO;

        SinWaveViewer(float* freq, float sampleRate, int steps, int width, int height, juce::Colour c) {
            range = Range(0.0f, 1.0f);
            mLFO.setFreq(*freq, sampleRate);
            mSteps = steps;
            mHeight = height;
            mWidth = width;
            mColour = c;
            mValues.resize(width+1, 0.0f);
            this->startTimerHz(45);
        }

        void paint(juce::Graphics& g) override {
            g.setColour(mColour);
            juce::Path p;
            float HalfHeight = float(mHeight) / 2.0f;
            for (int i = 0; i < mWidth; i++) {
                float fact = (mValues[i] - 0.5f);
                if (i == 0) p.startNewSubPath(0, (1 + fact) * HalfHeight);
                else {
                    float fact1 = (mValues[i-1] - 0.5f);
                    p.quadraticTo(i - 1, (1 + fact1) * HalfHeight, i , (1 + fact) * HalfHeight);
                }
            }
            g.strokePath(p, juce::PathStrokeType(2.0f));
        }

        void increment(int steps) {
            float value{0.0f};
            for (int i = 0; i < steps; i++) {
                value = mLFO.process(range);
            }
            mValues[mWidth - wIdx - 1] = value;
            wIdx = (wIdx + 1) % (mWidth);
        }

        void timerCallback() override {
            increment(mSteps);
            this->repaint();
        }

    private:

    };

    class SinWave : public juce::Component {
    public:

        float* mFreq;
        int mWidth, mHeight;
        juce::Colour mColour;
        float pi{ 3.14159265359f };
        float TAU{ 6.28318530718f };
        std::vector<float> mValues;
        float phase{ 0.0f };
        

        SinWave(float* freq, int width, int height, juce::Colour colour) {
            mFreq = freq;
            mWidth = width;
            mHeight = height;
            mColour = colour;
            mValues.resize(mWidth, 0.0f);
        }

        void paint(juce::Graphics& g) override{
            g.setColour(mColour);
            float inc = 2*pi*(*mFreq)/float(2.0*mWidth);
            juce::Path p;
            float cHeight = (float(mHeight) / 2.1f);
            for (int i = 0; i < mWidth; i++) {
                float fact = (std::sin(phase) - 0.4f);
                if (i == 0) p.startNewSubPath(0, (1.5f + fact) * cHeight);
                else {
                    float fact1 = (std::sin(phase-inc) - 0.5f);
                    p.lineTo( i, (1.5f + fact) * cHeight + 1);
                }
                phase += inc;
                if (phase > TAU)
                    phase -= TAU;
            }
            juce::Path p1;
            p1.addRectangle(1, 2, mWidth-1, mHeight);
            g.strokePath(p, juce::PathStrokeType(2.0f));
            g.strokePath(p1, juce::PathStrokeType(2.2f));
        }

    private:
    };

    class RectangleWidth : public juce::Component, juce::Timer  
    {
    public :
        float* mWidthe;
        int mWidth, mHeight;
        juce::Colour mColour;
        float pi{ 3.14159265359f };
        float normalizableValue;
        float inc{0.0f};
        float phase{0.0f};
        float offset{2.0f};
        float mFreq{30.0f};
        float mouvementFreq{1.4f};
        int mX, mY;
        int distanceMouved{3};
        
        
        RectangleWidth(float* width, float norm, int x, int y,int Width, int height, juce::Colour c) {
            mWidthe = width;
            mWidth = Width;
            mHeight = height;
            mColour = c;
            mX = x;
            mY = y;
            normalizableValue = norm;
            mFreq = 30;
            float seconds = 2;
            inc = 2 * pi * mouvementFreq / (seconds * mFreq);
            this->startTimerHz(mFreq);
        }

        void paint(juce::Graphics& g) override {
            g.setColour(mColour);
            juce::Path p;
            p.addRectangle(2, 2, mWidth-4, mHeight-4);
            float fact = ((*mWidthe/normalizableValue));
            p.addRectangle(2+fact * mWidth, 2+fact * mHeight, 0.5f*(1.0f - fact)*(mWidth-4), 0.5f*(1.0f - fact) * (mHeight-4));
            g.strokePath(p, juce::PathStrokeType(2.1f));
        }

        void timerCallback() override{
            mouveComponent();
            this->repaint();
           
        }
        
        
        void mouveComponent()
        {
            int distanceToMouve = int(distanceMouved * std::sin(phase + offset));
            
            this->setBounds(mX,mY+distanceToMouve,mWidth,mHeight);
            
            phase += inc;
            
            if (phase > 2.0f*pi)
                phase -= 2.0f*pi;
            
        }


    private:

    };

    class MinimalisticSliderStyle : public juce::LookAndFeel_V4
    {
    public:
        juce::ColourGradient gradientColour;
        juce::DropShadow dropShadow;
        int mOffset{ 5 };

        MinimalisticSliderStyle()
        {
        }

        void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& sld) override
        {

            auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
            auto centreX = (float)x + (float)width * 0.5f;
            auto centreY = (float)y + (float)height * 0.5f;
            auto rx = centreX - radius;
            auto ry = centreY - radius;
            auto rw = radius * 2.0f;
            auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
            juce::uint8 level{ 0 };
            juce::Colour white{ juce::Colour(level,level,level,1.0f) };
            g.setColour(white);
            g.drawEllipse(rx, ry, rw, rw, 2.0f);

            juce::Path p;
            auto pointerLength = radius * 0.33f;
            auto pointerThickness = 2.0f;
            p.addRoundedRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength, 0.0f);
            p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));


            g.fillPath(p);
        }



    private:

    };

    class DepthViewer : public juce::Component, juce::Timer {
    public:
        float* mLevel;
        int num{1};
        float pi{ 3.14159265359f };
        juce::Colour mColour;
        int mWidth, mHeight;
        int mNum;
        juce::Path p;
        float inc{0.0f};
        float phase{0.0f};
        float offset{0.0f};
        float mFreq{30.0f};
        float mouvementFreq{1.2f};
        int mX, mY;
        int distanceMouved{4};
        
        DepthViewer(float* level, int x,int y , int width, int height, int n, juce::Colour c, float alpha) {
            mX = x;
            mY = y;
            mLevel = level;
            mColour = c;
            mHeight = height-2;
            mWidth = width;
            mNum = n;
            mColour = mColour.withAlpha(alpha);
            mFreq = 40;
            float seconds = 2;
            inc = 2 * pi * mouvementFreq / (seconds * mFreq);
            this->startTimerHz(mFreq);
        }


        void paint(juce::Graphics& g) override {

            g.setColour(mColour);
            num = int((*mLevel) / (1.0f / float(mNum))) + 1;
            num = (num > mNum ? mNum : num);
            
            p.clear();
            int oHeight = int(float(mHeight) / float(mNum + 1));
            int mWidthOverTwo = int(float(mWidth) / 2.0f);
            for (int i = 0; i < num; i++) {
                p.startNewSubPath(0, (mNum - i) * oHeight+1);
                p.lineTo(mWidthOverTwo, (mNum + 1 - i) * oHeight+1);
                p.lineTo(mWidth, (mNum - i) * oHeight);
                p.lineTo(mWidthOverTwo, (mNum - 1 - i) * oHeight+1);
                p.closeSubPath();
            }
            g.strokePath(p, juce::PathStrokeType(2.0f));
        }

        void timerCallback() override{
            mouveComponent();
            this->repaint();
           
        }
        
        
        void mouveComponent()
        {
            int distanceToMouve = int(distanceMouved * std::sin(phase + offset));
            
            this->setBounds(mX,mY+distanceToMouve,mWidth,mHeight);
            
            phase += inc;
            
            if (phase > 2.0f*pi)
                phase -= 2.0f*pi;
            
        }
        

    private:
    };

    class TextBox : public juce::Component, juce::Timer 
    {
    public:
        float* mValue;
        juce::Font mFont;
        juce::Colour mColour;
        juce::String mText{""};
        int mWidth, mHeight;


        TextBox(float* value, int width, int height, juce::Font f, juce::Colour colour, juce::String text = "") {
            mValue = value;
            mFont = f;
            mColour = colour;
            mWidth = width;
            mHeight = height;
            mText = text;
            this->startTimerHz(20);
        }

        void paint(juce::Graphics& g) override {
            g.setColour(mColour);
            g.setFont(mFont);
            g.drawText(juce::String(float(int(( * mValue)*100))/100.0f)+mText, 0, 0, mWidth, mHeight, juce::Justification::centred);

        }

        void timerCallback() override {
            this->repaint();
        }

    private:
    };




    class ToggleButton : public juce::Component
    {
    public:

        juce::String name1;
        juce::String name2;
        bool* toggle;
        juce::Font font;

        ToggleButton(juce::String n1, juce::String n2, juce::Font& f, bool* state)
        {
            name1 = n1;
            name2 = n2;
            font = f;
            toggle = state;

        }

        void paint(juce::Graphics& g) override
        {
            int rectangleLength = ((int)(this->getWidth() / 2));
            int height = this->getHeight();
            font.setHeight(10);
            if (!(*toggle)) {
                g.setColour(juce::Colours::white);
                juce::Path p;
                p.addRectangle(0, 0, rectangleLength, height);
                g.setFont(font);
                g.drawText(name1, 0, 1, rectangleLength, height, 36);
                g.strokePath(p, juce::PathStrokeType(1.0f));

                g.setColour(juce::Colours::white);
                juce::Path p1;
                p1.addRectangle(rectangleLength, 0, rectangleLength, height);
                g.fillPath(p1);
                g.setColour(juce::Colours::black);
                g.drawText(name2, rectangleLength, 1, rectangleLength, height, 36);
            }
            else
            {
                g.setColour(juce::Colours::white);
                juce::Path p;
                p.addRectangle(0, 0, rectangleLength, height);
                g.fillPath(p);
                g.setFont(font);
                g.setColour(juce::Colours::black);
                g.drawText(name1, 0, 1, rectangleLength, height, 36);


                juce::Path p1;
                p1.addRectangle(rectangleLength, 0, rectangleLength, height);
                g.setColour(juce::Colours::white);
                g.drawText(name2, rectangleLength, 1, rectangleLength, height, 36);
                g.strokePath(p1, juce::PathStrokeType(1.0f));
            }

        }

        void mouseDown(const juce::MouseEvent& event) override
        {
            *toggle = !(*toggle);
            this->repaint();
        }
        void setSize(float s)
        {
            font.setHeight(s);
        }


    private:

    };


    class LinearMinimSliderStyle : public juce::LookAndFeel_V4
    {
    public:
        juce::Colour mColour;

        LinearMinimSliderStyle(juce::Colour c)
        {
            mColour = c;
        }

        void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider& s) override
        {
            g.setColour(mColour);
            juce::Path p;
            p.addRoundedRectangle(x, y, width, height, 8);
            g.strokePath(p, juce::PathStrokeType(1.0f));
            juce::Path p1;
            p1.addRoundedRectangle(x, y, s.getValue() * float(width), height, 8);
            g.fillPath(p1);

        }



    private:

    };


}
