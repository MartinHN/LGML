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


#ifndef PARAMETERUI_H_INCLUDED
#define PARAMETERUI_H_INCLUDED

#include "../Parameter.h"
#include "../../../JuceHeaderUI.h"
#include "../../../UI/Inspector/InspectableComponent.h"


class ParameterUI : public InspectableComponent,
    protected ParameterBase::AsyncListener,
    private ParameterBase::Listener,
    public Controllable::Listener
{
public:
    ParameterUI ( ParameterBase* parameter);
    virtual ~ParameterUI();

    WeakReference<ParameterBase> parameter;

    bool showLabel;
    bool showValue;

    void setCustomText (const String text);

    enum MappingState
    {
        NOMAP,
        MAPSOURCE,
        MAPDEST
    };

    void setMappingState (const bool  s);
    void setMappingDest (bool _isMappingDest);

    bool isDraggable;
    bool isSelected;
    void updateOverlayEffect();

    void visibilityChanged() override;
    void parentHierarchyChanged()override;

protected:

    String customTextDisplayed;
    // helper to spot wrong deletion order
    bool shouldBailOut();

    // here we are bound to only one parameter so no need to pass parameter*
    // for general behaviour see AsyncListener
    virtual void valueChanged (const var& ) {};
    virtual void rangeChanged ( ParameterBase* ) {};

    
    String getTooltip() override;
    virtual void mouseDown (const MouseEvent& e) override;
    virtual void mouseUp (const MouseEvent& e) override;

private:
    // see ParameterBase::AsyncListener
    virtual void newMessage (const ParameterBase::ParamWithValue& p) override;

    // never change this as value can be changed from other threads
    void parameterValueChanged ( ParameterBase* , ParameterBase::Listener * /*notifier=nullptr*/) override {};
    void parameterRangeChanged ( ParameterBase* )override {};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterUI)
    friend class LGMLDragger;
    MappingState mappingState;
    bool hasValidControllable;
    ScopedPointer<ImageEffectFilter> mapEffect;



    // Inherited via Listener
    virtual void controllableStateChanged (Controllable* c) override;
    virtual void controllableControlAddressChanged (Controllable* c) override;


    bool isMappingDest;
private:
    WeakReference<ParameterUI>::Master masterReference;
    friend class WeakReference<ParameterUI>;

    bool wasShowing;


};


//    this class allow to automaticly generate label / ui element for parameter listing in editor
//    it owns the created component
class NamedParameterUI : public ParameterUI, public Label::Listener
{
public:
    NamedParameterUI (ParameterUI* ui, int _labelWidth, bool labelAbove = false);
    void resized()override;
    bool labelAbove;
    void labelTextChanged (Label* labelThatHasChanged) override;
    Label controllableLabel;
    int labelWidth;
    ScopedPointer <ParameterUI > ownedParameterUI;
    void controllableControlAddressChanged (Controllable*)override;

    
};





#endif  // PARAMETERUI_H_INCLUDED
