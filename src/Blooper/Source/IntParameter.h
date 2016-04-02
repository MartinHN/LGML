/*
  ==============================================================================

    IntParameter.h
    Created: 8 Mar 2016 1:22:23pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef INTPARAMETER_H_INCLUDED
#define INTPARAMETER_H_INCLUDED

#include "Parameter.h"
class IntSliderUI;

class IntParameter : public Parameter
{
public:
    IntParameter(const String &niceName, const String &description, const int &initialValue, const int &minValue = 0, const int &maxValue = 1, bool enabled = true);
    ~IntParameter() {}

    int minValue;
    int maxValue;
    int value;

    void setValue(const int &_value, bool silentSet = false, bool force = false)
    {
        if (!force && this->value == _value) return;
        this->value = jlimit<int>(minValue, maxValue, _value);
        if (!silentSet) notifyValueChanged();
    }

    void setNormalizedValue(const float &normalizedValue, bool silentSet = false, bool force = false)
    {
        setValue((const int)jmap<float>(normalizedValue, (float)minValue, (float)maxValue), silentSet, force);
    }

    float getNormalizedValue() override
    {
        return jmap<float>((float)value, (float)minValue, (float)maxValue, 0.f, 1.f);
    }

    String toString() override{return String(value);}
    void fromString(const String & s,bool silentSet = false, bool force = false) override{setValue(s.getIntValue(),silentSet,force);};
#if !HEADLESS
    IntSliderUI * createSlider();
    ControllableUI * createDefaultControllableEditor()override;
#endif
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IntParameter)
};


#endif  // INTPARAMETER_H_INCLUDED
