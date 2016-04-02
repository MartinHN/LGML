/*
  ==============================================================================

    DummyNode.cpp
    Created: 3 Mar 2016 12:31:33pm
    Author:  bkupe

  ==============================================================================
*/

#include "DummyNode.h"

#if !HEADLESS
#include "NodeBaseUI.h"
#include "DummyNodeContentUI.h"
NodeBaseUI * DummyNode::createUI(){NodeBaseUI * ui = new NodeBaseUI(this,new DummyNodeContentUI());return ui;

}
#endif

DummyNode::DummyNode(NodeManager * nodeManager,uint32 nodeId) :
    NodeBase(nodeManager,nodeId, "DummyNode", new DummyAudioProcessor, new DummyDataProcessor)
{

    dataProcessor->addInputData("IN Number", DataProcessor::DataType::Number);
    dataProcessor->addInputData("IN Position", DataProcessor::DataType::Position);

    dataProcessor->addOutputData("OUT Number", DataProcessor::DataType::Number);
    dataProcessor->addOutputData("OUT Orientation", DataProcessor::DataType::Orientation);

    testFloatParam = addFloatParameter("Volume", "This is a test int slider",.23f);
    testTrigger = addTrigger("Test Trigger", "Youpi");

}

 DummyNode::~DummyNode()
{
}

 void DummyNode::parameterValueChanged(Parameter * p)
 {
     NodeBase::parameterValueChanged(p);
     if (p == testFloatParam)
     {
//       ((DummyAudioProcessor*)audioProcessor)->amp = p->getNormalizedValue();
         ((DummyAudioProcessor*)audioProcessor)->period = (int)(44100.0f/(1.0f+440.0f*p->getNormalizedValue()));
     }
 }

