/*
  ==============================================================================

    Spat2DNode.cpp
    Created: 2 Mar 2016 8:37:48pm
    Author:  bkupe

  ==============================================================================
*/

#include "Spat2DNode.h"
#include "NodeBaseUI.h"
#include "Spat2DNodeUI.h"

Spat2DNode::Spat2DNode() :
	NodeBase("Spat2D", NodeType::Spat2DType),
	numSpatInputs(nullptr), numSpatOutputs(nullptr), spatMode(nullptr), shapeMode(nullptr)
{
	spatMode = addEnumParameter("Mode", "Spatialization Mode (2D/3D, Beam/Proxy)");
	spatMode->addOption("Beam", BEAM);
	spatMode->addOption("Proxy", PROXY);
	
	spatMode->setValue("Proxy");

	shapeMode = addEnumParameter("Shape", "Predefined shape or free positioning");
	shapeMode->addOption("Circle", ShapeMode::CIRCLE);
	shapeMode->addOption("Free", ShapeMode::FREE);
	shapeMode->setValue("Free");

	//circle
	circleRadius = addFloatParameter("Circle Radius", "Radius of the circle to place the targets, if shape is set to circle", .8f, 0, 1);
	circleRotation = addFloatParameter("Circle Rotation", "Rotation of the circle to place the targets, if shape is set to circle", 0, 0, 360);

	targetRadius = addFloatParameter("Target Radius", "Radius for all targets", .5f, 0, 1);

	numSpatInputs = addIntParameter("Num Inputs", "Number of inputs to spacialize", 1, 0, 16);
	numSpatOutputs = addIntParameter("Num Outputs", "Number of spatialized outputs", 3, 0, 16);

	useGlobalTarget = addBoolParameter("Use Global Target", "Use a global target that will act as a max influence and affect all targets.", false);
	globalTargetPosition = addPoint2DParameter("Global Target Position", "Position of the Global Target");
	globalTargetRadius = addFloatParameter("Global Target Radius", "Radius for the global target",.5f,0,1);

	setPreferedNumAudioInput(numSpatInputs->intValue());
	setPreferedNumAudioOutput(numSpatOutputs->intValue()+2);
	
	
	updateInputOutputDataSlots();
	computeAllInfluences();
}

void Spat2DNode::setSourcePosition(int index, const Point<float>& position)
{
	Data * d = getInputData(index);
	if (d == nullptr) return;
	d->update(position.x, position.y, 0);
	computeAllInfluences();
}

void Spat2DNode::setTargetPosition(int index, const Point<float>& position)
{
	if (index == -1)
	{
		globalTargetPosition->setPoint(position);		
	}else
	{
		targetPositions[index]->setPoint(position);
		computeInfluencesForTarget(index);
	}
}

void Spat2DNode::updateInputOutputDataSlots()
{

  // need to lock to prevent access from audioThread
  // obviously, this method should not be called while performing (or we ll need to adapt it)
  const ScopedLock lk(getCallbackLock());
	if (numSpatInputs == nullptr || numSpatOutputs == nullptr) return;


	//INPUT
	while (getTotalNumInputData() > numSpatInputs->intValue())
	{
		removeInputData("Input " + String(getTotalNumInputData()));
	}

	DataType dt = modeIsBeam()? DataType::Orientation : DataType::Position;
	while (getTotalNumInputData() < numSpatInputs->intValue())
	{
		Data * d = addInputData("Input " + String(getTotalNumInputData() + 1), dt);
		Random rnd;
		d->update(rnd.nextFloat(), rnd.nextFloat());
	}

	//OUTPUT
	while (getTotalNumOutputData() > numSpatOutputs->intValue())
	{
		removeOutputData("Influence " + String(getTotalNumOutputData()));
		Point2DParameter * p= targetPositions[getTotalNumOutputData() - 1];
		targetPositions.removeAllInstancesOf(p);
		removeControllable(p);
	}


	while (getTotalNumOutputData() < numSpatOutputs->intValue())
	{
		addOutputData("Influence " + String(getTotalNumOutputData() + 1), DataType::Number);
		Point2DParameter * p = addPoint2DParameter("TargetPos" + String(getTotalNumOutputData() + 1), "");
		Random rnd;
		p->setPoint(rnd.nextFloat(), rnd.nextFloat());
		targetPositions.add(p);

		computeInfluencesForTarget(getTotalNumOutputData()-1);
	}
}

void Spat2DNode::updateTargetsFromShape()
{
	if (shapeMode == nullptr) return;

	switch ((int)shapeMode->getValueData())
	{
	case FREE:
		//do nothing
		break;
	case CIRCLE:
		for (int i = 0; i < numSpatOutputs->intValue(); i++)
		{
			Point2DParameter *p = targetPositions[i];
			float angle = (i*1.f / (numSpatOutputs->intValue()) + circleRotation->floatValue() / 360.f)*float_Pi*2;
			p->setPoint(.5f+cosf(angle)*circleRadius->floatValue()*.5f,.5f+sinf(angle)*circleRadius->floatValue()*.5f);
		}
		if (useGlobalTarget->boolValue())
		{
			globalTargetPosition->setPoint(.5f, .5f);
		}
		computeAllInfluences();
		break;
	}
	 
	
}

void Spat2DNode::computeAllInfluences()
{
	//Only one source for now
	switch((int)spatMode->getValueData())
	{
	case BEAM:
		break;

	case PROXY:
		
		for (int i = 0; i < numSpatOutputs->intValue(); i++)
		{
			computeInfluencesForTarget(i);
		}
		break;
	}
}

void Spat2DNode::computeInfluencesForTarget(int targetIndex)
{
	for (int i = 0; i < numSpatInputs->intValue(); i++)
	{
		computeInfluence(i, targetIndex);
	}
}

void Spat2DNode::computeInfluence(int sourceIndex, int targetIndex)
{
	if (sourceIndex >= inputDatas.size()) return;
	if (targetIndex >= outputDatas.size()) return;

	Data * inputData = inputDatas[sourceIndex];
	Data * outputData = outputDatas[targetIndex];

	float minValue = 0;
	if (useGlobalTarget->boolValue())
	{
		Point<float> sPos = Point<float>(inputDatas[sourceIndex]->elements[0]->value, inputDatas[sourceIndex]->elements[1]->value);
		minValue = getValueForSourceAndTargetPos(sPos, globalTargetPosition->getPoint(),globalTargetRadius->floatValue());
	}

	if (numSpatInputs->intValue() > 0)
	{
		Point<float> sPos = Point<float>(inputData->elements[0]->value, inputData->elements[1]->value);
		Point<float> tPos = targetPositions[targetIndex]->getPoint();
		
		float val = jmax<float>(minValue, getValueForSourceAndTargetPos(sPos, tPos,targetRadius->floatValue()));
		outputData->update(val);
	} else
	{
		outputData->update(0);
	}
}

float Spat2DNode::getValueForSourceAndTargetPos(const Point<float>& sourcePosition, const Point<float>& targetPosition, float radius)
{
	if (radius == 0) return 0;

	float dist = jlimit<float>(0, radius, sourcePosition.getDistanceFrom(targetPosition));
	return 1 - (dist / radius);
}

void Spat2DNode::numChannelsChanged(bool /*isInput*/){
updateInputOutputDataSlots();
}

bool Spat2DNode::modeIsBeam()
{
	return (int)spatMode->getValueData() == BEAM;
}

void Spat2DNode::onContainerParameterChanged(Parameter * p)
{
	
	if (p == spatMode)
	{
		removeAllInputDatas();
		updateInputOutputDataSlots();
		updateTargetsFromShape();
	} else if (p == numSpatInputs)
	{
		setPreferedNumAudioInput(numSpatInputs->intValue());		
		updateTargetsFromShape();

	} else if (p == numSpatOutputs)
	{
		setPreferedNumAudioOutput(numSpatOutputs->intValue()+2);
		updateChannelNames();
		updateTargetsFromShape();
	} else if (p == shapeMode || p == circleRadius || p == circleRotation)
	{
		updateTargetsFromShape();
	} else if (p == globalTargetRadius || p == targetRadius || p == globalTargetPosition)
	{
		computeAllInfluences();
	}

	NodeBase::onContainerParameterChanged(p);

}

void Spat2DNode::processInputDataChanged(Data *)
{
	computeAllInfluences();
}

void Spat2DNode::updateChannelNames()
{
	setOutputChannelName(0, "Main L");
	setOutputChannelName(1, "Main R");
	for (int i = 2; i < numSpatOutputs->intValue()+2; i++)
	{
		setOutputChannelName(i, "Spat " + String(i - 2));
	}
}

void Spat2DNode::processBlockInternal(AudioBuffer<float>& buffer, MidiBuffer &)
{


	int numSamples = buffer.getNumSamples();
	if (totalNumInputChannels == 0) return;

	for (int i = 2; i < totalNumOutputChannels; i++)
	{
		float influence = outputDatas[i - 2]->elements[0]->value;
		buffer.copyFrom(i, 0, buffer.getReadPointer(0), numSamples, influence);
		buffer.addFrom(i, 0, buffer.getReadPointer(1), numSamples, influence);
	}
	
	
}

ConnectableNodeUI * Spat2DNode::createUI() {
	return new NodeBaseUI(this,new Spat2DNodeContentUI());
}
