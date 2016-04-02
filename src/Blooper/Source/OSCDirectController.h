/*
  ==============================================================================

    OSCDirectController.h
    Created: 8 Mar 2016 10:27:37pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef OSCDIRECTCONTROLLER_H_INCLUDED
#define OSCDIRECTCONTROLLER_H_INCLUDED

#include "OSCController.h"

class OSCDirectController : public OSCController, public ControllableContainer::Listener
{
public:
    OSCDirectController();

    void processMessage(const OSCMessage &msg) override;
#if !HEADLESS
    ControllerUI * createUI() override;
#endif
    // Inherited via Listener
    virtual void controllableAdded(Controllable * c) override;
    virtual void controllableRemoved(Controllable * c) override;
    virtual void controllableContainerAdded(ControllableContainer * cc) override;
    virtual void controllableContainerRemoved(ControllableContainer * cc) override;

    virtual void controllableFeedbackUpdate(Controllable * c) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OSCDirectController)


        // Inherited via Listener


};


#endif  // OSCDIRECTCONTROLLER_H_INCLUDED
