/*
  ==============================================================================

    StringParameter.cpp
    Created: 9 Mar 2016 12:29:30am
    Author:  bkupe

  ==============================================================================
*/

#include "StringParameter.h"


StringParameter::StringParameter(const String & niceName, const String &description, const String & initialValue, bool enabled) :
    Parameter(Type::STRING, niceName, description, enabled)
{
    setValue(initialValue);

}

#if !HEADLESS
#include "StringParameterUI.h"

StringParameterUI * StringParameter::createStringParameterUI()
{
    return new StringParameterUI(this);
}


ControllableUI* StringParameter::createDefaultControllableEditor(){
    return createStringParameterUI();
};
#endif