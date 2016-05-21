/*
  ==============================================================================

    NodeContainerUI.h
    Created: 18 May 2016 7:54:08pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef NODECONTAINERUI_H_INCLUDED
#define NODECONTAINERUI_H_INCLUDED

#include "NodeContainer.h"
#include "InspectableComponent.h"
#include "ConnectableNodeUI.h"

class NodeContainerUI :
	public ConnectableNodeUI,
	public NodeContainerListener
{
public:
	NodeContainerUI(NodeContainer * nc);
	virtual ~NodeContainerUI();


	void clear();
	NodeContainer * nodeContainer;

	// Inherited via NodeContainerListener
	virtual void nodeAdded(ConnectableNode *) override;
	virtual void nodeRemoved(ConnectableNode *) override;
	virtual void connectionAdded(NodeConnection *) override;
	virtual void connectionRemoved(NodeConnection *) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NodeContainerUI)
};



#endif  // NODECONTAINERUI_H_INCLUDED
