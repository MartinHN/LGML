/*
 ==============================================================================

 NodeBase.cpp
 Created: 2 Mar 2016 8:36:17pm
 Author:  bkupe

 ==============================================================================
 */

#include "NodeBase.h"
#include "NodeManager.h"
#include "TimeManager.h"

#include "AudioHelpers.h"

NodeBase::NodeBase(const String &name,NodeType _type, bool _hasMainAudioControl) :
ConnectableNode(name,_type,_hasMainAudioControl),
dryWetFader(5000,5000,false,1),
muteFader(1000,1000,false,1),
lastDryVolume(0),
globalRMSValueIn(0),
globalRMSValueOut(0),
wasEnabled(false)

{

  logVolume = float01ToGain(DB0_FOR_01);

  lastVolume = hasMainAudioControl ? outputVolume->floatValue() : 0;
  dryWetFader.setFadedIn();
  muteFader.startFadeIn();


  for (int i = 0; i < 2; i++) rmsValuesIn.add(0);
  for (int i = 0; i < 2; i++) rmsValuesIn.add(0);
  startTimerHz(30);

}


NodeBase::~NodeBase()
{
  stopTimer();
  NodeBase::masterReference.clear();
  clear();
}


bool NodeBase::hasAudioInputs()
{
  //to override
  return getTotalNumInputChannels() > 0;
}

bool NodeBase::hasAudioOutputs()
{
  //to override
  return getTotalNumOutputChannels() > 0;
}

bool NodeBase::hasDataInputs()
{
  //to override
  return getTotalNumInputData()>0;
}

bool NodeBase::hasDataOutputs()
{
  //to override
  return getTotalNumOutputData()>0;
}


void NodeBase::onContainerParameterChanged(Parameter * p)
{
  if(!p)return;
  ConnectableNode::onContainerParameterChanged(p);

  if(p==outputVolume){
    logVolume = float01ToGain(outputVolume->floatValue());
  }

  //ENABLE PARAM ACT AS A BYPASS

  if (p == enabledParam)
  {
    if(enabledParam->boolValue()){dryWetFader.startFadeIn();}
    else {dryWetFader.startFadeOut();}
  }

}

void NodeBase::clear()
{
  clearInternal();

  //Data
  inputDatas.clear();
  outputDatas.clear();
  stopTimer();

  //removeFromAudioGraph();
}




//Save / Load

var NodeBase::getJSONData()
{
  var data = ConnectableNode::getJSONData();

  MemoryBlock m;

  // TODO we could implement that for all node objects to be able to save any kind of custom data
  getStateInformation(m);

  if (m.getSize()) {
    var audioProcessorData(new DynamicObject());
    audioProcessorData.getDynamicObject()->setProperty("state", m.toBase64Encoding());
    data.getDynamicObject()->setProperty("audioProcessor", audioProcessorData);
  }

  return data;
}

void NodeBase::loadJSONDataInternal(var data)
{
  ConnectableNode::loadJSONDataInternal(data);

  var audioProcessorData = data.getProperty("audioProcessor", var());
  String audioProcessorStateData = audioProcessorData.getProperty("state",var());

  MemoryBlock m;
  m.fromBase64Encoding(audioProcessorStateData);
  setStateInformation(m.getData(), (int)m.getSize());
}

//ui

ConnectableNodeUI * NodeBase::createUI() {
  DBG("No implementation in child node class !");
  jassert(false);
  return nullptr;
}




/////////////////////////////////////// AUDIO

void NodeBase::processBlockBypassed(AudioBuffer<float>& /*buffer*/, juce::MidiBuffer& /*midiMessages*/){
  // no op
}

void NodeBase::processBlock(AudioBuffer<float>& buffer,
                            MidiBuffer& midiMessages) {

  // be sure to delete input if we are not enabled and a random buffer enters
  // juceAudioGraph seems to use the fact that we shouldn't process audio to pass others
  int numSample = buffer.getNumSamples();

  //Already set and class parameters
  //int totalNumInputChannels = getTotalNumInputChannels();
  //int totalNumOutputChannels =getTotalNumOutputChannels();




  if (rmsListeners.size() || rmsChannelListeners.size()) {
    curSamplesForRMSInUpdate += numSample;
    if (curSamplesForRMSInUpdate >= samplesBeforeRMSUpdate) {
      updateRMS(buffer, globalRMSValueIn,rmsValuesIn,totalNumInputChannels,rmsChannelListeners.size()==0);
      curSamplesForRMSInUpdate = 0;
    }
  }



  muteFader.incrementFade(numSample);
  dryWetFader.incrementFade(numSample);
  const double crossfadeValue = dryWetFader.getCurrentFade();
  const double muteFadeValue =muteFader.getCurrentFade();


  // on disable
  if(wasEnabled && crossfadeValue==0 ){


    wasEnabled = false;
  }
  // on Enable
  if(!wasEnabled && crossfadeValue>0 ){

    wasEnabled = true;
  }

  if (!isSuspended())
  {

    double curVolume = logVolume*crossfadeValue*muteFadeValue;
    double curDryVolume = logVolume*(1.0-crossfadeValue)*muteFadeValue;

    if(crossfadeValue!=1){
      // copy only what we are expecting
      int maxCommonChannels = jmin(totalNumOutputChannels,totalNumInputChannels);
      crossFadeBuffer.setSize(maxCommonChannels, numSample);
      for(int i = 0 ; i < maxCommonChannels ; i++){
        crossFadeBuffer.copyFrom(i, 0, buffer, i, 0, numSample);
      }
    }
    if(lastVolume==0 && curVolume==0){
      processBlockBypassed(buffer, midiMessages);
    }
    else{
      processBlockInternal(buffer, midiMessages);
    }

    if(crossfadeValue!=1 || hasMainAudioControl){
      buffer.applyGainRamp(0, numSample, lastVolume, (float)curVolume);

    }


    // crossfade if we have a dry mix i.e at least one input channel
    if (totalNumInputChannels > 0 && totalNumOutputChannels > 0)
    {
      if (crossfadeValue != 1 && crossFadeBuffer.getNumChannels()>0) {
        for (int i = 0; i < totalNumOutputChannels; i++) {
          buffer.addFromWithRamp(i, 0, crossFadeBuffer.getReadPointer(jmin(i,crossFadeBuffer.getNumChannels()-1)), numSample, (float)lastDryVolume, (float)curDryVolume);
        }
      }
    }


    if(muteFadeValue == 0){
      buffer.clear();
    }
    lastVolume = (float)curVolume;
    lastDryVolume = curDryVolume;

  }
  else{
    DBG("suspended");
  }


  // be sure to delete out if we are not enabled and a random buffer enters
  // juceAudioGraph seems to use the fact that we shouldn't process audio to pass others
  //  for(int i = totalNumOutputChannels;i < buffer.getNumChannels() ; i++){
  //    buffer.clear(i,0,numSample);
  //  }

  if (rmsListeners.size() || rmsChannelListeners.size()) {
    curSamplesForRMSOutUpdate += numSample;
    if (curSamplesForRMSOutUpdate >= samplesBeforeRMSUpdate) {
      updateRMS(buffer, globalRMSValueOut,rmsValuesOut,totalNumOutputChannels,rmsChannelListeners.size()==0);
      curSamplesForRMSOutUpdate = 0;
    }
  }





};

bool NodeBase::setPreferedNumAudioInput(int num) {

  int oldNumChannels = getTotalNumInputChannels();

  {

    if (parentNodeContainer != nullptr){
      {
        const ScopedLock lk( parentNodeContainer->innerGraph->getCallbackLock());
        setPlayConfigDetails(num, getTotalNumOutputChannels(),
                             getSampleRate(),
                             getBlockSize());



        totalNumInputChannels = getTotalNumInputChannels();
        parentNodeContainer->updateAudioGraph(false);
        if(oldNumChannels!=getTotalNumInputChannels()){
          // numChannelsChanged is called within the lock so that Nodes can update freely their memory used in processblock
          numChannelsChanged(true);
        }
      }

    }
    else{
      // here is only if the Node sets a default prefered audio Input (in its constructor)
      setPlayConfigDetails(num, getTotalNumOutputChannels(),
                           getSampleRate(),
                           getBlockSize());
      totalNumInputChannels = getTotalNumInputChannels();
      if(oldNumChannels!=getTotalNumInputChannels()){

        numChannelsChanged(true);
      }
    }
  }

  rmsValuesIn.clear();

  for (int i = 0; i < totalNumInputChannels; i++) rmsValuesIn.add(0);


  if (totalNumInputChannels > oldNumChannels)
  {
    for (int i = oldNumChannels; i < totalNumInputChannels; i++)
    {
      nodeListeners.call(&ConnectableNodeListener::audioInputAdded, this, i);
    }
  }
  else
  {
    for (int i = oldNumChannels - 1; i >= totalNumInputChannels; i--)
    {
      nodeListeners.call(&ConnectableNodeListener::audioInputRemoved, this, i);
    }
  }


  nodeListeners.call(&ConnectableNodeListener::numAudioInputChanged, this,num);

  return true;
}


bool NodeBase::setPreferedNumAudioOutput(int num) {

  int oldNumChannels = getTotalNumOutputChannels();
  {

    if (parentNodeContainer != nullptr){
      //      parentNodeContainer->getAudioGraph()->suspendProcessing(true);
      {
        const ScopedLock lk( parentNodeContainer->getAudioGraph()->getCallbackLock());
        setPlayConfigDetails(getTotalNumInputChannels(), num,
                             getSampleRate(),
                             getBlockSize());



        totalNumOutputChannels = getTotalNumOutputChannels();
        parentNodeContainer->updateAudioGraph(false);
        if(oldNumChannels!=totalNumOutputChannels){
          numChannelsChanged(false);
        }
      }
      //      if(ContainerOutNode* n = dynamic_cast<ContainerOutNode*>(this)){
      //        n->parentNodeContainer->setPreferedNumAudioOutput(totalNumOutputChannels);
      //      }


      //      parentNodeContainer->getAudioGraph()->suspendProcessing(false);
    }
    else{
      setPlayConfigDetails(getTotalNumInputChannels(), num,
                           getSampleRate(),
                           getBlockSize());
      totalNumOutputChannels = getTotalNumOutputChannels();
      if(oldNumChannels!=totalNumOutputChannels){
        numChannelsChanged(false);
      }

    }
  }

  rmsValuesOut.clear();

  for (int i = 0; i < totalNumOutputChannels; i++) rmsValuesOut.add(0);


  if (totalNumOutputChannels > oldNumChannels)
  {
    for (int i = oldNumChannels; i < totalNumOutputChannels; i++)
    {
      nodeListeners.call(&ConnectableNodeListener::audioOutputAdded, this, i);
    }
  }else
  {
    for (int i = oldNumChannels-1; i >= totalNumOutputChannels; i--)
    {
      nodeListeners.call(&ConnectableNodeListener::audioOutputRemoved, this, i);
    }
  }

  nodeListeners.call(&ConnectableNodeListener::numAudioOutputChanged,this,num);

  return true;
}




void NodeBase::timerCallback()
{
  ConnectableNode::rmsListeners.call(&ConnectableNode::RMSListener::RMSChanged, this, globalRMSValueIn, globalRMSValueOut);
  for (int i = 0; i < getTotalNumInputChannels(); i++)
  {
    ConnectableNode::rmsChannelListeners.call(&ConnectableNode::RMSChannelListener::channelRMSInChanged, this, rmsValuesIn[i], i);
  }

  for (int i = 0; i < getTotalNumOutputChannels(); i++)
  {
    ConnectableNode::rmsChannelListeners.call(&ConnectableNode::RMSChannelListener::channelRMSOutChanged, this, rmsValuesOut[i], i);
  }
}

//////////////////////////////////   DATA

Data * NodeBase::getInputData(int dataIndex)
{
  if (inputDatas.size() <= dataIndex) return nullptr;
  return inputDatas[dataIndex];
}


Data * NodeBase::getOutputData(int dataIndex)
{
  if (outputDatas.size() <= dataIndex) return nullptr;
  return outputDatas[dataIndex];
}


Data * NodeBase::addInputData(const String & name, Data::DataType dataType)
{
  Data *d = new Data(this, name, dataType, Data::Input);
  inputDatas.add(d);

  d->addDataListener(this);

  nodeListeners.call(&ConnectableNodeListener::dataInputAdded, this, d);
  nodeListeners.call(&ConnectableNodeListener::numDataInputChanged, this, inputDatas.size());
  return d;
}

Data * NodeBase::addOutputData(const String & name, DataType dataType)
{
  Data * d = new Data(this, name, dataType,Data::Output);
  outputDatas.add(d);

  d->addDataListener(this);

  nodeListeners.call(&ConnectableNodeListener::dataOutputAdded, this, d);
  nodeListeners.call(&ConnectableNodeListener::numDataOutputChanged, this, inputDatas.size());

  return d;
}

void NodeBase::removeInputData(const String & name)
{
  Data * d = getInputDataByName(name);
  if (d == nullptr) return;
  d->removeDataListener(this);
  inputDatas.removeObject(d, false);
  nodeListeners.call(&ConnectableNodeListener::dataInputRemoved, this, d);
  nodeListeners.call(&ConnectableNodeListener::numDataInputChanged, this, inputDatas.size());
  delete d;
}

void NodeBase::removeOutputData(const String & name)
{
  Data * d = getOutputDataByName(name);
  if (d == nullptr) return;
  d->removeDataListener(this);
  outputDatas.removeObject(d, false);
  nodeListeners.call(&ConnectableNodeListener::dataOutputRemoved, this, d);
  nodeListeners.call(&ConnectableNodeListener::numDataOutputChanged, this, inputDatas.size());
  delete d;
}

void NodeBase::removeAllInputDatas()
{
  while (inputDatas.size() > 0)
  {
    removeInputData(inputDatas[0]->name);
  }
}

void NodeBase::removeAllOutputDatas()
{
  while (outputDatas.size() > 0)
  {
    removeOutputData(outputDatas[0]->name);
  }
}

void NodeBase::updateOutputData(String & dataName, const float & value1, const float & value2, const float & value3)
{
  Data * d = getOutputDataByName(dataName);
  if (d != nullptr) d->update(value1, value2, value3);
}

int NodeBase::getTotalNumInputData() {
  return inputDatas.size();
}

int NodeBase::getTotalNumOutputData() {
  return outputDatas.size();
}

StringArray NodeBase::getInputDataInfos()
{
  StringArray dataInfos;
  for (auto &d : inputDatas) dataInfos.add(d->name + " (" + d->getTypeString() + ")");
  return dataInfos;
}

StringArray NodeBase::getOutputDataInfos()
{
  StringArray dataInfos;
  for (auto &d : outputDatas) dataInfos.add(d->name + " (" + d->getTypeString() + ")");
  return dataInfos;
}

Data::DataType NodeBase::getInputDataType(const String &dataName, const String &elementName)
{
  for (int i = inputDatas.size(); --i >= 0;)
  {
    Data* d = inputDatas.getUnchecked(i);

    if (d->name == dataName)
    {
      if (elementName.isEmpty())
      {
        return d->type;
      }
      else
      {
        DataElement * e = d->getElement(elementName);
        if (e == nullptr) return DataType::Unknown;
        return e->type;
      }
    }
  }

  return DataType::Unknown;
}

Data::DataType NodeBase::getOutputDataType(const String &dataName, const String &elementName)
{
  for (int i = outputDatas.size(); --i >= 0;)
  {
    Data* d = outputDatas.getUnchecked(i);

    if (d->name == dataName)
    {
      if (elementName.isEmpty())
      {
        return d->type;
      }
      else
      {
        DataElement * e = d->getElement(elementName);
        if (e == nullptr) return DataType::Unknown;
        return e->type;
      }
    }
  }

  return DataType::Unknown;

}

Data * NodeBase::getOutputDataByName(const String & dataName)
{
  for (auto &d : outputDatas)
  {
    if (d->name == dataName) return d;
  }

  return nullptr;
}

Data * NodeBase::getInputDataByName(const String & dataName)
{
  for (auto &d : inputDatas)
  {
    if (d->name == dataName) return d;
  }

  return nullptr;
}

void NodeBase::dataChanged(Data * d)
{
  DBG("Data changed, ioType " << (d->ioType == Data::Input ? "input" : "output"));
  if (d->ioType == Data::Input)
  {
    if (enabledParam->boolValue()) {
      processInputDataChanged(d);
    }
    nodeListeners.call(&ConnectableNodeListener::nodeInputDataChanged, this, d);
  } else
  {
    if (enabledParam->boolValue()) {
      processOutputDataUpdated(d);
    }
    nodeListeners.call(&ConnectableNodeListener::nodeOutputDataUpdated, this, d);
  }
}
