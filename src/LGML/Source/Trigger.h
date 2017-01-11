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
#include "QueuedNotifier.h"

class TriggerButtonUI;
class TriggerBlinkUI;

class Trigger : public Controllable
{
public:
	Trigger(const String &niceName, const String &description, bool enabled = true);
	~Trigger() {masterReference.clear();}
	
	
	
	TriggerButtonUI * createButtonUI(Trigger * target = nullptr);
	TriggerBlinkUI * createBlinkUI(Trigger * target = nullptr);
	ControllableUI * createDefaultUI(Controllable * targetControllable = nullptr) override;
	
	virtual DynamicObject * createDynamicObject() override;
	
	void trigger()
	{
		if (enabled && !isTriggering){
			isTriggering = true;
            listeners.call(&Listener::triggerTriggered, this);
            queuedNotifier.addMessage(new WeakReference<Trigger>(this));
            isTriggering = false;
		}
	}
	
	// avoid feedback loop in listeners
	bool isTriggering;
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
	
    
	
	QueuedNotifier<WeakReference<Trigger>> queuedNotifier;
	typedef QueuedNotifier<WeakReference<Trigger>>::Listener AsyncListener;
	void addAsyncTriggerListener(AsyncListener * l){queuedNotifier.addListener(l);}
	void removeAsyncTriggerListener(AsyncListener * l){queuedNotifier.removeListener(l);}
	
private:
	WeakReference<Trigger>::Master masterReference;
	friend class WeakReference<Trigger>;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Trigger)
};



#endif  // TRIGGER_H_INCLUDED
