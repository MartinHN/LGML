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


#ifndef BETTERINTSTEPPER_H_INCLUDED
#define BETTERINTSTEPPER_H_INCLUDED

#include "../../../JuceHeaderUI.h"

class BetterStepper : public Slider
{
public:
    BetterStepper (TooltipClient* tooltip=nullptr);
    virtual ~BetterStepper();
    void resized()override;
    bool isMini;
    void setEditable(bool s);
    String getTooltip() override;
private:
    bool isEditable;
    TooltipClient * tooltipClient;
    void mouseWheelMove (const MouseEvent& e, const MouseWheelDetails& wheel) override;
    void mouseEnter(const MouseEvent& e)override;

    uint64 timeEntered ;

};




#endif  // BETTERINTSTEPPER_H_INCLUDED
