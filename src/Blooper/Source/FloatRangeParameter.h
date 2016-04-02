/*
  ==============================================================================

    FloatRangeParameter.h
    Created: 8 Mar 2016 6:49:15pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef FLOATRANGEPARAMETER_H_INCLUDED
#define FLOATRANGEPARAMETER_H_INCLUDED


#include "Parameter.h"

class FloatRangeParameter : public Parameter
{
public:
    FloatRangeParameter(const String &niceName, const String &description, const float &initialValueMin, const float &initialValueMax, const float &minValue = 0, const float &maxValue = 1, bool enabled = true);
    ~FloatRangeParameter() {}

    float minValue;
    float maxValue;

    float valueMin;
    float valueMax;

    void setValuesMinMax(const float &_valueMin, const float &_valueMax, bool silentSet = false, bool force = false)
    {
        if (!force && this->valueMin == _valueMin && this->valueMax == _valueMax) return;
        this->valueMin = jlimit<float>(minValue, maxValue, _valueMin);
        this->valueMax = jlimit<float>(_valueMin, maxValue, _valueMax);
        if (!silentSet) notifyValueChanged();
    }

    void setValuesMaxMin(const float &_valueMin, const float &_valueMax, bool silentSet = false, bool force = false)
    {
        if (!force && this->valueMin == _valueMin && this->valueMax == _valueMax) return;
        this->valueMax = jlimit<float>(minValue, maxValue, _valueMax);
        this->valueMin = jlimit<float>(minValue, _valueMax, _valueMin);
        if (!silentSet) notifyValueChanged();
    }

    float getNormalizedValueMin()
    {
        return jmap<float>(valueMin, minValue, maxValue, 0, 1);
    }

    float getNormalizedValueMax()
    {
        return jmap<float>(valueMax, minValue, maxValue, 0, 1);
    }
    String toString()override { return String::formatted("%f,%f,%f,%f",valueMin,valueMax,minValue,maxValue);}

    void fromString(const String & s,bool = false, bool = false) override{
        StringArray sa;
        sa.addTokens(s,",");
        DBG("to implement");
        jassertfalse;}

#if !HEADLESS
    ControllableUI * createDefaultControllableEditor()override{return nullptr;};
#endif
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FloatRangeParameter)
};





#endif  // FLOATRANGEPARAMETER_H_INCLUDED
