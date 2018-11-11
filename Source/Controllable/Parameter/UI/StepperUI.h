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


#ifndef FLOATSTEPPERUI_H_INCLUDED
#define FLOATSTEPPERUI_H_INCLUDED

#include "ParameterUI.h"
#include "BetterStepper.h"


template <typename T>
class StepperUI : public ParameterUI, private Slider::Listener
{

public:
    StepperUI ( ParameterBase* _parameter);
    virtual ~StepperUI();



    ScopedPointer<BetterStepper> slider;

    void resized() override;

    void setStepInterval(T i);

    void mouseDown(const MouseEvent & e)override;
protected:
    void valueChanged (const var&) override;
    virtual void rangeChanged ( ParameterBase* p) override;

    // Inherited via Listener
    virtual void sliderValueChanged (Slider* slider) override;

    


    T stepInterval;

};


typedef StepperUI<int> IntStepperUI;
typedef StepperUI<floatParamType> FloatStepperUI;

#endif  // FLOATSTEPPERUI_H_INCLUDED
