/*
  ==============================================================================

    Spat2DViewer.h
    Created: 29 Sep 2016 7:25:11pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SPAT2DVIEWER_H_INCLUDED
#define SPAT2DVIEWER_H_INCLUDED

#include "JuceHeader.h"
#include "Spat2DNode.h"

class Spat2DHandle : public Component
{
public:
	enum HandleType { SOURCE, TARGET};
	Spat2DHandle(HandleType type, int index, float size, Colour color);
	~Spat2DHandle();

	virtual void paint(Graphics &g) override;
	virtual void mouseDown(const MouseEvent &e) override;
	virtual void mouseDrag(const MouseEvent &e) override;

	virtual void resized() override;

	HandleType type;
	int index;
	Colour color;
	float size;
	Point<float> position;

	void setPosition(Point<float> newPosition);

	virtual bool hitTest(int x, int y) override;

	class  Listener
	{
	public:

		/** Destructor. */
		virtual ~Listener() {}
		virtual void handleUserMoved(Spat2DHandle * handle, const Point<float> &newPosition) = 0;
	};

	ListenerList<Listener> handleListeners;
	void addHandleListener(Listener* newListener) { handleListeners.add(newListener); }
	void removeHandleListener(Listener* listener) { handleListeners.remove(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Spat2DHandle)
};

class Spat2DSource : public Spat2DHandle
{
public:
	Spat2DSource(int index);
	float angle;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Spat2DSource)
};

class Spat2DTarget : public Spat2DHandle
{
public:
	Spat2DTarget(int index, Colour color = Colours::orange);
	float radius;
	float influence;

	void paint(Graphics &g) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Spat2DTarget)
};

class Spat2DViewer : public Component, public Spat2DHandle::Listener, 
	public ConnectableNode::ConnectableNodeListener,
	public ControllableContainer::ControllableContainerListener
{
public:
	Spat2DViewer(Spat2DNode * node);
	~Spat2DViewer();

	OwnedArray<Spat2DSource> sources;
	OwnedArray<Spat2DTarget> targets;
	ScopedPointer<Spat2DTarget> globalTarget;

	Spat2DNode * node;
	
	void updateNumSources();
	void updateNumTargets();
	void updateTargetRadius();

	void updateSourcePosition(int sourceIndex);
	void updateTargetPosition(int targetIndex);
	void updateTargetInfluence(int targetIndex);
	
	void resized() override;
	void paint(Graphics &g) override;

	void nodeParameterChanged(ConnectableNode *, Parameter * p) override;
	void nodeInputDataChanged(ConnectableNode *, Data * d) override;
	void nodeOutputDataUpdated(ConnectableNode *, Data * d) override;

	void dataInputAdded(ConnectableNode *, Data *) override;
	void dataInputRemoved(ConnectableNode *, Data *) override;
	void controllableAdded(Controllable *) override;
	void controllableRemoved(Controllable *) override;

	// Inherited via Listener (Spat2DHandle)
	virtual void handleUserMoved(Spat2DHandle * handle, const Point<float> &newPosition) override;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Spat2DViewer)

		

		

};


#endif  // SPAT2DVIEWER_H_INCLUDED
