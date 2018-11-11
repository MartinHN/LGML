/* Copyright © Organic Orchestra, 2017
*
* This file is part of LGML.  LGML is a software to manipulate sound in realtime
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation (version 3 of the License).
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
*/

#if !ENGINE_HEADLESS

#include "StringParameterUI.h"
#include "../../../UI/Style.h"
#include "../UndoableHelper.h"

String varToString(const var &v){
    String stringValue;
    if(v.isArray()){
        StringArray arr;
        for(auto vl:*v.getArray()){
            arr.add(vl.toString());
        }
        stringValue = "[" + arr.joinIntoString(",") + " ]";
    }
    else{
        stringValue = v.toString();
    }
    return stringValue;

}

StringParameterUI::StringParameterUI ( ParameterBase* p) :
    ParameterUI (p), autoSize (false), maxFontHeight (12)
{

    addChildComponent (nameLabel);
    setNameLabelVisible (false);
    addAndMakeVisible (valueLabel);
    addMouseListener(this, true);
    nameLabel.setJustificationType (Justification::topLeft);
    nameLabel.setText (prefix + parameter->niceName + suffix, NotificationType::dontSendNotification);

    valueLabel.setJustificationType (Justification::topLeft);
    valueLabel.setText (varToString(parameter->value), NotificationType::dontSendNotification);

    bool stringEditable = parameter->isEditable ;
    valueLabel.setEditable (false, stringEditable);

    valueLabel.addListener (this);

    valueLabel.setBorderSize(BorderSize<int>(0));
    setBackGroundIsTransparent (!stringEditable);
    nameLabel.setTooltip (p->description);

    arraySize = p->value.isArray()?p->value.getArray()->size():-1;

    setSize (200, 20); //default size
}

void StringParameterUI::setAutoSize (bool value)
{
    autoSize = value;
    valueChanged (parameter->value);
}

void StringParameterUI::setPrefix (const String& _prefix)
{
    if (prefix == _prefix) return;

    prefix = _prefix;
    valueChanged (parameter->stringValue());
}

void StringParameterUI::setSuffix (const String& _suffix)
{
    if (suffix == _suffix) return;

    suffix = _suffix;
    valueChanged (parameter->stringValue());
}

void StringParameterUI::setNameLabelVisible (bool visible)
{
    //    if (nameLabelIsVisible == visible) return;
    nameLabelIsVisible = visible;
    nameLabel.setVisible (visible);
}
void StringParameterUI::setBackGroundIsTransparent (bool t)
{
    valueLabel.setColour (valueLabel.backgroundColourId, Colours::transparentWhite.withAlpha (t ? 0 : 0.1f));
}



void StringParameterUI::resized()
{
    Rectangle<int> r = getLocalBounds();
    int nameLabelWidth = 100;// nameLabel.getFont().getStringWidth(nameLabel.getText());

    if (nameLabelIsVisible)
    {
        nameLabel.setBounds (r.removeFromLeft (nameLabelWidth));
        nameLabel.setFont (nameLabel.getFont().withHeight (jmin<float> ((float)r.getHeight(), maxFontHeight)));

    }

    valueLabel.setBounds (r);
    valueLabel.setFont (valueLabel.getFont().withHeight (jmin<float> ((float)r.getHeight(), maxFontHeight)));

}


void StringParameterUI::valueChanged (const var& v)
{
    String stringValue = varToString(v);

valueLabel.setText (prefix + stringValue+ suffix, NotificationType::dontSendNotification);
    if (autoSize)
    {
        int nameLabelWidth = nameLabel.getFont().getStringWidth (nameLabel.getText());
        int valueLabelWidth = valueLabel.getFont().getStringWidth (valueLabel.getText());
        int tw = valueLabelWidth;

        if (nameLabelIsVisible) tw += 5 + nameLabelWidth;

        setSize (tw + 10, (int)valueLabel.getFont().getHeight());
    }

}

void StringParameterUI::labelTextChanged (Label*)
{
    if(arraySize!=-1){
        StringArray arr;
        String sv = valueLabel.getText();
        sv = sv.removeCharacters("[]");
        arr.addTokens(sv,",","");
        Array<var> varList;
        for(int i = 0 ; i < arraySize;i++){
            if(i<arr.size()){
                varList.add(arr[i].getFloatValue());
            }
            else{
                varList.add(0);
            }
        }

        parameter->setValue(varList);
    }
    else{
    //String  originalString = valueLabel.getText().substring(prefix.length(), valueLabel.getText().length() - suffix.length());
        UndoableHelpers::setValueUndoable(parameter, valueLabel.getText());

    }
}

#endif
