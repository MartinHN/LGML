/*
  ==============================================================================

    ShapeShifterContainer.h
    Created: 2 May 2016 3:11:25pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SHAPESHIFTERCONTAINER_H_INCLUDED
#define SHAPESHIFTERCONTAINER_H_INCLUDED

#include "ShapeShifterPanel.h"
#include "GapGrabber.h"


class ShapeShifterContainer;

class ShapeShifterContainerListener
{
public:
	virtual ~ShapeShifterContainerListener() {}
	virtual void panelAdded(ShapeShifterContainer *) {}
	virtual void panelRemoved(ShapeShifterContainer *) {}

	virtual void containerAdded(ShapeShifterContainer *) {}
	virtual void containerRemoved(ShapeShifterContainer *) {}

	virtual void containerEmptied(ShapeShifterContainer *) {}

};


//Listener
class ShapeShifterContainer : public ShapeShifter,
	public GapGrabber::Listener, 
	public ShapeShifterPanel::Listener, 
	public ShapeShifterContainerListener
{
public:
	enum Direction { NONE, HORIZONTAL, VERTICAL};
	enum Position { TOP, BOTTOM, LEFT ,RIGHT };
	enum ContentType { CONTAINERS, PANELS };

	ShapeShifterContainer(ContentType contentType, Direction _direction);
	virtual ~ShapeShifterContainer();
	

	void resized() override;

	ContentType contentType;
	Direction direction;
	OwnedArray<GapGrabber> grabbers;
	
	Array<ShapeShifter *> shifters;
	
	//Content Type PANELS
	void insertPanelAt(ShapeShifterPanel * panel, int index);
	void removePanel(ShapeShifterPanel * panel);

	OwnedArray<ShapeShifterContainer> containers;
	ShapeShifterContainer * insertContainerAt(int index, ContentType _contentType, Direction _direction);
	void removeContainer(ShapeShifterContainer * container);
	

	virtual void grabberGrabUpdate(GapGrabber * gg, int dist) override;
	virtual void panelDetach(ShapeShifterPanel *) override;
	virtual void panelRemoved(ShapeShifterPanel *) override;

	virtual void containerEmptied(ShapeShifterContainer *) override;

	ListenerList<ShapeShifterContainerListener> containerListeners;
	void addShapeShifterContainerListener(ShapeShifterContainerListener* newListener) { containerListeners.add(newListener); }
	void removeShapeShifterContainerListener(ShapeShifterContainerListener* listener) { containerListeners.remove(listener); }

	

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ShapeShifterContainer)
};



#endif  // SHAPESHIFTERCONTAINER_H_INCLUDED