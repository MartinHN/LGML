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


#ifndef NODECONNECTIONEDITORDATASLOT_H_INCLUDED
#define NODECONNECTIONEDITORDATASLOT_H_INCLUDED



//#include "NodeBase.h"
#include "../NodeConnection.h"


//==============================================================================
/*
*/
class NodeConnectionEditorDataSlot    : public juce::Component
{
public:

    enum IOType { INPUT, OUTPUT };


    NodeConnectionEditorDataSlot (String label, int channel, NodeConnection::ConnectionType connectionType, IOType ioType); //for audio
    ~NodeConnectionEditorDataSlot();

    String label;
    
    int channel;

    Array<NodeConnectionEditorDataSlot*> connectedSlots;
    bool addConnectedSlot (NodeConnectionEditorDataSlot* s)
    {
        if (isConnectedTo (s)) return false;

        connectedSlots.add (s);
        repaint();

        return true;
    }

    bool removeConnectedSlot (NodeConnectionEditorDataSlot* s)
    {
        if (!isConnectedTo (s)) return false;

        connectedSlots.removeAllInstancesOf (s);
        repaint();

        return true;
    }

    bool isConnected() { return connectedSlots.size() > 0; }
    bool isConnectedTo (NodeConnectionEditorDataSlot* s) { return (connectedSlots.contains (s)); }
    NodeConnectionEditorDataSlot* getFirstConnectedSlot()
    {
        if (!isConnected()) return nullptr;

        return connectedSlots[0];
    }

    NodeConnection::ConnectionType connectionType;
    IOType ioType;

    bool isAudio() { return connectionType == NodeConnection::ConnectionType::AUDIO; }


    void paint (Graphics&)override ;
    void resized()override;


    void mouseDown (const MouseEvent& e) override;
    void mouseEnter (const MouseEvent& e) override;
    void mouseExit (const MouseEvent& e) override;
    void mouseMove (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;

    //Listener
    class  SlotListener
    {
    public:
        /** Destructor. */
        virtual ~SlotListener() {}

        virtual void slotMouseEnter (NodeConnectionEditorDataSlot* target) = 0;
        virtual void slotMouseExit (NodeConnectionEditorDataSlot* target) = 0;
        virtual void slotMouseDown (NodeConnectionEditorDataSlot* target) = 0;
        virtual void slotMouseMove (NodeConnectionEditorDataSlot* target) = 0;
        virtual void slotMouseUp (NodeConnectionEditorDataSlot* target) = 0;
        virtual void slotMouseDrag (NodeConnectionEditorDataSlot* target) = 0;
    };

    ListenerList<SlotListener> listeners;
    void addSlotListener (SlotListener* newListener) { listeners.add (newListener); }
    void removeSlotListener (SlotListener* listener) { listeners.remove (listener); }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NodeConnectionEditorDataSlot)
};


#endif  // NODECONNECTIONEDITORDATASLOT_H_INCLUDED
