/*
  ==============================================================================

    BoolParameter.cpp
    Created: 8 Mar 2016 1:22:15pm
    Author:  bkupe

  ==============================================================================
*/

#include "BoolParameter.h"

BoolParameter::BoolParameter(const String & niceName, const String &description, const bool & initialValue, bool enabled) :
    Parameter(Type::BOOL, niceName, description, enabled)
{
    setValue(initialValue);
}


#if !HEADLESS
#include "BoolToggleUI.h"

BoolToggleUI * BoolParameter::createToggle()
{
    return new BoolToggleUI(this);
}


ControllableUI * BoolParameter::createDefaultControllableEditor() {return createToggle();}
#endif