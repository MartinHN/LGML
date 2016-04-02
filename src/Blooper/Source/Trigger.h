/*
  ==============================================================================

    Trigger.h
    Created: 8 Mar 2016 1:09:29pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef TRIGGER_H_INCLUDED
#define TRIGGER_H_INCLUDED

#include "Controllable.h"

class TriggerButtonUI;
class TriggerBlinkUI;

class Trigger : public Controllable
{
public:
    Trigger(const String &niceName, const String &description, bool enabled = true);
    ~Trigger() {}


#if !HEADLESS
    TriggerButtonUI * createButtonUI();
    TriggerBlinkUI * createBlinkUI();
    ControllableUI * createDefaultControllableEditor()override;
#endif

    void trigger()
    {
        if (enabled) listeners.call(&Listener::triggerTriggered, this);
    }

public:
    //Listener
    class  Listener
    {
    public:
        /** Destructor. */
        virtual ~Listener() {}
        virtual void triggerTriggered(Trigger * p) = 0;
    };

    ListenerList<Listener> listeners;
    void addTriggerListener(Trigger::Listener* newListener) { listeners.add(newListener); }
    void removeTriggerListener(Trigger::Listener* listener) { listeners.remove(listener); }


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Trigger)
};



#endif  // TRIGGER_H_INCLUDED
