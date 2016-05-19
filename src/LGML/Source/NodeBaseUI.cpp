/*
 ==============================================================================

 NodeBaseUI.cpp
 Created: 3 Mar 2016 11:52:50pm
 Author:  bkupe

 ==============================================================================
 */



#include "NodeBaseUI.h"
#include "NodeBase.h"
#include "NodeManagerUI.h"
#include "ConnectorComponent.h"
#include "FloatSliderUI.h"
#include "MainComponent.h"

//==============================================================================
NodeBaseUI::NodeBaseUI(NodeBase * _node, NodeBaseContentUI * _contentContainer, NodeBaseHeaderUI * _headerContainer) :
ConnectableNodeUI(_node),
mainContainer(this,_contentContainer,_headerContainer),
node(_node),
dragIsLocked(false)
{
    connectorWidth = 10;

    inputContainer.setConnectorsFromNode(node);
    outputContainer.setConnectorsFromNode(node);

    addAndMakeVisible(mainContainer);
    addAndMakeVisible(inputContainer);
    addAndMakeVisible(outputContainer);
    getHeaderContainer()->addMouseListener(this, true);// (true, true);

    mainContainer.setNodeAndNodeUI(node, this);
    if(getWidth() == 0 || getHeight() == 0) setSize(280,100);

    node->xPosition->addParameterListener(this);
    node->yPosition->addParameterListener(this);
    node->xPosition->hideInEditor = true;
    node->yPosition->hideInEditor = true;

	node->enabledParam->addParameterListener(this);

}

NodeBaseUI::~NodeBaseUI()
{
	node->xPosition->removeParameterListener(this);
	node->yPosition->removeParameterListener(this);
	node->enabledParam->removeParameterListener(this);
}

void NodeBaseUI::moved(){

    if(node->xPosition->intValue() != getBounds().getCentreX() || node->yPosition->intValue() != getBounds().getCentreY() ){
        node->xPosition->setValue((float)getBounds().getCentreX());
        node->yPosition->setValue((float)getBounds().getCentreY());
    }
}


void NodeBaseUI::setNode(NodeBase *)
{
    //parameters
}


void NodeBaseUI::paint (Graphics&)
{

}

void NodeBaseUI::resized()
{
    Rectangle<int> r = getLocalBounds();
    Rectangle<int> inputBounds = r.removeFromLeft(connectorWidth);
    Rectangle<int> outputBounds = r.removeFromRight(connectorWidth);

    mainContainer.setBounds(r);
    inputContainer.setBounds(inputBounds);
    outputContainer.setBounds(outputBounds);

}

void NodeBaseUI::parameterValueChanged(Parameter * p) {

	if (p == node->xPosition || p == node->yPosition) {
		setCentrePosition((int)node->xPosition->value, (int)node->yPosition->value);
	}
	else if (p == node->enabledParam)
	{
		mainContainer.repaint();
	}
}


// allow to react to custom mainContainer.contentContainer
void NodeBaseUI::childBoundsChanged (Component* c){
    // if changes in this layout take care to update  childBounds changed to update when child resize itself (NodeBaseContentUI::init()
    if(c == &mainContainer){
        int destWidth = mainContainer.getWidth() + 2* connectorWidth;
        int destHeight = mainContainer.getHeight();
        if(getWidth()!=destWidth ||
           destHeight != getHeight()){
            setSize(destWidth, destHeight);
        }
    }
}


#pragma warning( disable : 4100 ) //still don't understand why this is generating a warning if not disabled by pragma.
void NodeBaseUI::mouseDown(const juce::MouseEvent &e)
{
	if (e.eventComponent != &mainContainer.headerContainer->grabber) return;

    nodeInitPos = getBounds().getCentre();

	/*
    // don't want to drag if over volume
    if(NodeBaseAudioCtlUI * ctlUI = mainContainer.audioCtlUIContainer){
        Point<int> mouse = getMouseXYRelative();
        Component * found = getComponentAt(mouse.x,mouse.y);

        dragIsLocked = (dynamic_cast<NodeBaseAudioCtlUI *>(found) == ctlUI);
    }
	*/
}
#pragma warning( default : 4100 )


void NodeBaseUI::mouseUp(const juce::MouseEvent &){
	selectThis();
}

void NodeBaseUI::mouseDrag(const MouseEvent & e)
{
	if (e.eventComponent != &mainContainer.headerContainer->grabber) return;
    //if(dragIsLocked) return;

    Point<int> diff = Point<int>(e.getPosition() - e.getMouseDownPosition());
    Point <int> newPos = nodeInitPos + diff;
    node->xPosition->setValue((float)newPos.x);
    node->yPosition->setValue((float)newPos.y);
}

bool NodeBaseUI::keyPressed(const KeyPress & key)
{
	if (!isSelected) return false;
	if (key.getKeyCode() == KeyPress::deleteKey || key.getKeyCode() == KeyPress::backspaceKey)
	{
		node->remove();
		return true;
	}

	return false;
}




NodeBaseUI::MainContainer::MainContainer(NodeBaseUI * _nodeUI, NodeBaseContentUI * content, NodeBaseHeaderUI * header) :
nodeUI(_nodeUI),
headerContainer(header),
contentContainer(content),
audioCtlUIContainer(nullptr)
{

    if (headerContainer == nullptr) headerContainer = new NodeBaseHeaderUI();
    if (contentContainer == nullptr) contentContainer = new NodeBaseContentUI();


    addAndMakeVisible(headerContainer);
    addAndMakeVisible(contentContainer);

}

void NodeBaseUI::MainContainer::setNodeAndNodeUI(NodeBase * _node, NodeBaseUI * _nodeUI)
{
    if(_node->hasAudioOutputs()){
        jassert(audioCtlUIContainer==nullptr);
        audioCtlUIContainer = new NodeBaseAudioCtlUI();
        addAndMakeVisible(audioCtlUIContainer);
        audioCtlUIContainer->setNodeAndNodeUI(_node,_nodeUI);
    }

    headerContainer->setNodeAndNodeUI(_node, _nodeUI);
    contentContainer->setNodeAndNodeUI(_node, _nodeUI);

    resized();
}

void NodeBaseUI::MainContainer::paint(Graphics & g)
{
    g.setColour(nodeUI->node->enabledParam->boolValue()? PANEL_COLOR:PANEL_COLOR.darker(.7f));
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 4);

    g.setColour(nodeUI->isSelected ?HIGHLIGHT_COLOR:LIGHTCONTOUR_COLOR);
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(1),4.f, nodeUI->isSelected?2.f:.5f);


}


void NodeBaseUI::MainContainer::resized()
{

    // if changes in this layout take care to update  childBounds changed to update when child resize itself (NodeBaseContentUI::init()
    Rectangle<int> r = getLocalBounds();
    if(r.getWidth()==0 || r.getHeight() == 0)return;

    Rectangle<int> headerBounds = r.removeFromTop(headerContainer->getHeight());
    headerContainer->setBounds(headerBounds);
    if(audioCtlUIContainer){
        r.removeFromRight(audioCtlContainerPadRight);
        audioCtlUIContainer->setBounds(r.removeFromRight(10).reduced(0, 4));
    }
    contentContainer->setBounds(r);
}
void NodeBaseUI::MainContainer::childBoundsChanged (Component* c){
    if(c == contentContainer || c== audioCtlUIContainer){
        int destWidth = contentContainer->getWidth()+(audioCtlUIContainer?audioCtlUIContainer->getWidth()+audioCtlContainerPadRight:0);
        int destHeight = contentContainer->getHeight()+headerContainer->getHeight();
        if(getWidth() !=  destWidth||
           getHeight() != destHeight){
            setSize(destWidth,destHeight);
        }
    }
}
