/*
  ==============================================================================

    DummyNode.cpp
    Created: 3 Mar 2016 12:31:33pm
    Author:  bkupe

  ==============================================================================
*/

#include "DummyNode.h"
#include "NodeBaseUI.h"
#include "DummyNodeContentUI.h"

DummyNode::DummyNode() :
    NodeBase("DummyNode",NodeType::DummyType)
{


    freq1Param = addFloatParameter("Freq 1", "This is a test int slider",.23f);
    freq2Param = addFloatParameter("Freq 2", "This is a test int slider", .55f);

    testTrigger = addTrigger("Test Trigger", "Youpi");

	enumParam = addEnumParameter("Mode", "Enum Mode test");
	enumParam->addOption("Left / Right","lr");
	enumParam->addOption("Right / Left", "rl");
	enumParam->addOption("Mixed", "mixed");

	//AUDIO
	setPlayConfigDetails(2, 3, getSampleRate(), getBlockSize());

	//DATA
	addInputData("IN Number", DataType::Number);
	addInputData("IN Position", DataType::Position);

	addOutputData("OUT Number", DataType::Number);
	addOutputData("OUT Orientation", DataType::Orientation);
}

 DummyNode::~DummyNode()
{
}

 void DummyNode::onContainerParameterChanged(Parameter * p)
 {
     NodeBase::onContainerParameterChanged(p);
     if (p == freq1Param)
     {
         //       ((DummyAudioProcessor*)audioProcessor)->amp = p->getNormalizedValue();
         period1 = (int)(44100.0f / (1.0f + 440.0f*p->getNormalizedValue()));
     }
     else if (p == freq2Param)
     {
         period2 = (int)(44100.0f / (1.0f + 440.0f*p->getNormalizedValue()));
	 } else if (p == enumParam)
	 {
		 //DBG("Enum param changed : " << enumParam->stringValue() << " / " << enumParam->getValueData().toString());
	 }
 }

 void DummyNode::processBlockInternal(AudioBuffer<float>& buffer, MidiBuffer &) {
	 
	 for (int i = 0; i < buffer.getNumSamples(); i++) {
		 if (enumParam->getValueData() == "lr")
		 {
			 buffer.addSample(0, i, (float)(amp*cos(2.0*double_Pi*step1*1.0 / period1)));
			 buffer.addSample(1, i, (float)(amp*cos(2.0*double_Pi*step2*1.0 / period2)));
		 } else if (enumParam->getValueData() == "rl")
		 {
			 buffer.addSample(1, i, (float)(amp*cos(2.0*double_Pi*step1*1.0 / period1)));
			 buffer.addSample(0, i, (float)(amp*cos(2.0*double_Pi*step2*1.0 / period2)));
		 } else if (enumParam->getValueData() == "mixed")
		 {
			 buffer.addSample(0, i, (float)(amp*cos(2.0*double_Pi*step1*1.0 / period1)));
			 buffer.addSample(0, i, (float)(amp*cos(2.0*double_Pi*step2*1.0 / period2)));
			 buffer.addSample(1, i, (float)(amp*cos(2.0*double_Pi*step1*1.0 / period1)));
			 buffer.addSample(1, i, (float)(amp*cos(2.0*double_Pi*step2*1.0 / period2)));
		 }
		 step1++;
		 step2++;
		 if (step1>period1) { step1 = 0; }
		 if (step2>period2) { step2 = 0; }
	 }




 }

 inline void DummyNode::processInputDataChanged(Data * d)
 {
	 //DBG("DummyNode :: Input data changed " << d->name);

	 if (d->name == "IN Number")
	 {
		 amp = d->getElement("value")->value;
	 }
 }

 ConnectableNodeUI * DummyNode::createUI()
{

    NodeBaseUI * ui = new NodeBaseUI(this,new DummyNodeContentUI());
    return ui;

}
