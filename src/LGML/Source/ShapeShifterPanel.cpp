/*
  ==============================================================================

    MovablePanel.cpp
    Created: 2 May 2016 3:08:37pm
    Author:  bkupe

  ==============================================================================
*/

#include "ShapeShifterPanel.h"
#include "Style.h"

#include "ShapeShifterManager.h"

ShapeShifterPanel::ShapeShifterPanel(ShapeShifterContent *_content, ShapeShifterPanelTab * sourceTab) :
	currentContent(nullptr), transparentBackground(false), targetMode(false), candidateZone(NONE), candidateTargetPoint(Point<float>())
{
	addAndMakeVisible(header);
	header.addHeaderListener(this);

	if (sourceTab == nullptr)
	{
		addContent(_content);
	}else
	{
		attachTab(sourceTab);
	}

	
}

ShapeShifterPanel::~ShapeShifterPanel()
{
	header.removeHeaderListener(this);
	listeners.call(&Listener::panelRemoved, this);
}


void ShapeShifterPanel::setCurrentContent(ShapeShifterContent * _content)
{
	if (_content == currentContent) return;

	if (currentContent != nullptr)
	{
		ShapeShifterPanelTab * tab = header.getTabForContent(currentContent);
		if(tab != nullptr) tab->setSelected(false);
		removeChildComponent(currentContent);
	}


	currentContent = _content;

	if (currentContent != nullptr)
	{
		ShapeShifterPanelTab * tab = header.getTabForContent(currentContent);
		if (tab != nullptr) tab->setSelected(true);
		
		addAndMakeVisible(currentContent);
	}
	resized();
}

void ShapeShifterPanel::setTargetMode(bool value)
{
	if (targetMode == value) return;
	targetMode = value;
	repaint();
}

void ShapeShifterPanel::paint(Graphics & g)
{
	g.setColour(BG_COLOR.withAlpha(transparentBackground?.3f:1));
	g.fillRect(getLocalBounds().withTrimmedTop(headerHeight));
}

void ShapeShifterPanel::paintOverChildren(Graphics & g)
{
	DBG("Paint over children " << String(targetMode));
	if (!targetMode) return;
	Rectangle<int> r = getLocalBounds();
	
	Colour hc = HIGHLIGHT_COLOR.withAlpha(.5f);
	Colour nc = NORMAL_COLOR.withAlpha(.2f);
	int reduceAmount = 2;

	
	g.setColour(candidateZone == AttachZone::TOP ? hc : nc);
	g.fillRect(r.removeFromTop(getHeight()*.2f).reduced(reduceAmount));

	g.setColour(candidateZone == AttachZone::BOTTOM ? hc : nc);
	g.fillRect(r.removeFromBottom(getHeight()*.2f).reduced(reduceAmount));

	g.setColour(candidateZone == AttachZone::LEFT ? hc : nc);
	g.fillRect(r.removeFromLeft(getWidth()*.2f).reduced(reduceAmount));

	g.setColour(candidateZone == AttachZone::RIGHT ? hc : nc);
	g.fillRect(r.removeFromRight(getWidth()*.2f).reduced(reduceAmount));


	g.setColour(candidateZone == AttachZone::CENTER ? hc : nc);
	g.fillRect(r);
}

void ShapeShifterPanel::resized()
{
	Rectangle<int> r = getLocalBounds();
	header.setBounds(r.removeFromTop(headerHeight));
	if (currentContent != nullptr)
	{
		currentContent->setBounds(r);
	}
}

void ShapeShifterPanel::setTransparentBackground(bool value)
{
	if (transparentBackground == value) return;
	transparentBackground = value;
	repaint();
}

void ShapeShifterPanel::attachTab(ShapeShifterPanelTab * tab)
{
	header.attachTab(tab);
	contents.add(tab->content);
	setCurrentContent(tab->content);
}

void ShapeShifterPanel::detachTab(ShapeShifterPanelTab * tab)
{
	ShapeShifterContent * content = tab->content;

	Rectangle<int> r = getScreenBounds();

	header.removeTab(tab,false);

	int cIndex = contents.indexOf(content);
	contents.removeAllInstancesOf(content);

	if (currentContent == content)
	{
		if (contents.size() > 0)
		{
			DBG("here !");
			setCurrentContent(contents[juce::jlimit<int>(0,contents.size()-1, cIndex)]);
		}else
		{
			listeners.call(&Listener::panelEmptied, this);
		}
	}

	ShapeShifterPanel * newPanel = ShapeShifterManager::getInstance()->createPanel(content,tab);
	ShapeShifterManager::getInstance()->showPanelWindow(newPanel,r);
}

void ShapeShifterPanel::addContent(ShapeShifterContent * content, bool setCurrent)
{
	header.addTab(content);
	contents.add(content);
	if(setCurrent) setCurrentContent(content);
}

void ShapeShifterPanel::removeTab(ShapeShifterPanelTab * tab)
{
	ShapeShifterContent * content = tab->content;
	header.removeTab(tab, true);

	int cIndex = contents.indexOf(content);
	contents.removeAllInstancesOf(content);

	if (currentContent == content)
	{
		if (contents.size() > 0)
		{
			setCurrentContent(contents[juce::jmax<int>(cIndex, 0)]);
		}
		else
		{
			listeners.call(&Listener::panelEmptied, this);
		}
	}
}



ShapeShifterPanel::AttachZone ShapeShifterPanel::checkAttachZone(ShapeShifterPanel * source)
{
	AttachZone z = AttachZone::NONE;

	candidateTargetPoint = getLocalPoint(source, Point<float>());
	
	float rx = candidateTargetPoint.x / getWidth();
	float ry = candidateTargetPoint.y / getHeight();

	DBG("Check Attach Zone (" << header.getTabForContent(currentContent)->getName() << ") : " << rx << ", " << ry);

	if (rx < 0 || rx > 1 || ry < 0 || ry > 1)
	{
		//keep none
	}else
	{
		if (rx < .2f) z = AttachZone::LEFT;
		else if (rx > .8f) z = AttachZone::RIGHT;
		else if (ry < .2f) z = AttachZone::TOP;
		else if (ry > .8f) z = AttachZone::BOTTOM;
		else z = AttachZone::CENTER;
	}

	setCandidateZone(z);
	return candidateZone;
}

void ShapeShifterPanel::setCandidateZone(AttachZone zone)
{
	if (candidateZone == zone) return;
	candidateZone = zone;
	repaint();
}

void ShapeShifterPanel::tabDrag(ShapeShifterPanelTab * tab)
{
	if(!isDetached() || contents.size() > 1) detachTab(tab);
	else listeners.call(&Listener::tabDrag, this);
}

void ShapeShifterPanel::tabSelect(ShapeShifterPanelTab * tab)
{
	setCurrentContent(tab->content);
}

void ShapeShifterPanel::headerDrag()
{
	if (!isDetached()) listeners.call(&Listener::panelDetach, this);
	else listeners.call(&Listener::headerDrag, this);
}