/*
  ==============================================================================

    ConnectableNode.cpp
    Created: 18 May 2016 11:33:58pm
    Author:  bkupe

  ==============================================================================
*/

#include "ConnectableNode.h"

#include "ConnectableNodeUI.h"


  ConnectableNode::ConnectableNode(const String & name, NodeType _type) :
	  type(_type),
	  canBeRemovedByUser(true),
	  ControllableContainer(name)
  {
	  //set Params
	  nameParam = addStringParameter("Name", "Set the name of the node.", name);
	  enabledParam = addBoolParameter("Enabled", "Set whether the node is enabled or disabled", true);
	  xPosition = addFloatParameter("xPosition", "x position on canvas", 0, 0, 99999);
	  yPosition = addFloatParameter("yPosition", "y position on canvas", 0, 0, 99999);

	  xPosition->isControllableExposed = false;
	  yPosition->isControllableExposed = false;
	  xPosition->isPresettable = false;
	  yPosition->isPresettable = false;
	  nameParam->isPresettable = false;
	  enabledParam->isPresettable = false;
  }

  ConnectableNode::~ConnectableNode()
{
}

bool ConnectableNode::hasAudioInputs()
{
	//to override
	return false;
}

bool ConnectableNode::hasAudioOutputs()
{
	//to override
	return false;
}

bool ConnectableNode::hasDataInputs()
{
	//to override
	return false;
}

bool ConnectableNode::hasDataOutputs()
{
	//to override
	return false;
}


void ConnectableNode::parameterValueChanged(Parameter * p)
{
	if (p == nameParam)
	{
		setNiceName(nameParam->stringValue());
	}
	else if (p == enabledParam)
	{
		nodeListeners.call(&ConnectableNodeListener::nodeEnableChanged, this);
	}
	else {
		ControllableContainer::parameterValueChanged(p);
	}
}

ConnectableNodeUI * ConnectableNode::createUI()
{
	DBG("No implementation in child node class !");
	jassert(false);
	return nullptr;
}


void ConnectableNode::remove(bool askBeforeRemove)
{
	if (askBeforeRemove)
	{
		int result = AlertWindow::showOkCancelBox(AlertWindow::AlertIconType::QuestionIcon, "Remove node", "Do you want to remove the node ?");
		if (result == 0) return;
	}

	nodeListeners.call(&ConnectableNode::ConnectableNodeListener::askForRemoveNode, this);
}

var ConnectableNode::getJSONData()
{
	var data = ControllableContainer::getJSONData();
	return data;
}

void ConnectableNode::loadJSONDataInternal(var data)
{
	ControllableContainer::loadJSONDataInternal(data);
}



/////////////////////////////  AUDIO

AudioProcessorGraph::Node * ConnectableNode::getAudioNode(bool)
{
	//to override
	return nullptr;
}


void ConnectableNode::addToAudioGraph()
{
	//to override
}

void ConnectableNode::removeFromAudioGraph()
{
	//To override
}



void ConnectableNode::setInputChannelNames(int startChannel, StringArray names)
{
	for (int i = startChannel; i < startChannel + names.size(); i++)
	{
		setInputChannelName(i, names[i]);
	}
}

void ConnectableNode::setOutputChannelNames(int startChannel, StringArray names)
{
	for (int i = startChannel; i < startChannel + names.size(); i++)
	{
		setOutputChannelName(i, names[i]);
	}
}

void ConnectableNode::setInputChannelName(int channelIndex, const String & name)
{
	while (inputChannelNames.size() < (channelIndex + 1))
	{
		inputChannelNames.add(String::empty);
	}

	inputChannelNames.set(channelIndex, name);
}

void ConnectableNode::setOutputChannelName(int channelIndex, const String & name)
{
	while (outputChannelNames.size() < (channelIndex + 1))
	{
		outputChannelNames.add(String::empty);
	}

	outputChannelNames.set(channelIndex, name);
}

String ConnectableNode::getInputChannelName(int channelIndex)
{
	String defaultName = "Input " + String(channelIndex);
	if (channelIndex < 0 || channelIndex >= inputChannelNames.size()) return defaultName;

	String s = inputChannelNames[channelIndex];
	if (s.isNotEmpty()) return s;
	return defaultName;
}

String ConnectableNode::getOutputChannelName(int channelIndex)
{
	String defaultName = "Output " + String(channelIndex);
	if (channelIndex < 0 || channelIndex >= outputChannelNames.size()) return defaultName;

	String s = outputChannelNames[channelIndex];
	if (s.isNotEmpty()) return s;
	return defaultName;
}


/////////////////////////////  DATA
Data * ConnectableNode::getInputData(int)
{
	//to override
	return nullptr;
}

Data * ConnectableNode::getOutputData(int)
{
	//to override
	return nullptr;
}

int ConnectableNode::getTotalNumInputData()
{
	//to override
	return 0;
}

int ConnectableNode::getTotalNumOutputData()
{
	//to override
	return 0;
}

StringArray ConnectableNode::getInputDataInfos()
{
	return StringArray();
}

StringArray ConnectableNode::getOutputDataInfos()
{
	return StringArray();
}

Data::DataType ConnectableNode::getInputDataType(const String & , const String & )
{
	//to override
	return Data::DataType::Unknown;
}

Data::DataType ConnectableNode::getOutputDataType(const String & , const String & )
{
	//to override
	return Data::DataType::Unknown;
}

Data * ConnectableNode::getOutputDataByName(const String & )
{
	//to override
	return nullptr;
}

Data * ConnectableNode::getInputDataByName(const String & )
{
	//to override
	return nullptr;
}




