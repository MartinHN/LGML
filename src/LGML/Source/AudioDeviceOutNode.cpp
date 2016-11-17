/*
 ==============================================================================

 AudioDeviceOutNode.cpp
 Created: 7 Mar 2016 8:04:38pm
 Author:  Martin Hermant

 ==============================================================================
 */

#include "AudioDeviceOutNode.h"
#include "NodeBaseUI.h"
#include "AudioDeviceOutNodeUI.h"

#include "AudioHelpers.h"
#include "NodeManager.h"

AudioDeviceManager& getAudioDeviceManager();

AudioDeviceOutNode::AudioDeviceOutNode() :
NodeBase("AudioDeviceOut",NodeType::AudioDeviceOutType, false),
AudioGraphIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::IODeviceType::audioOutputNode)
{
  //CanHavePresets = false;



  {
    MessageManagerLock ml;
    getAudioDeviceManager().addChangeListener(this);
  }
  AudioIODevice * ad = getAudioDeviceManager().getCurrentAudioDevice();

  desiredNumAudioOutput = addIntParameter("numAudioOutput", "desired numAudioOutputs (independent of audio settings)",
                                          ad?ad->getActiveInputChannels().countNumberOfSetBits():2, 0, 32);
  lastNumberOfOutputs = 0;

  setPreferedNumAudioInput(desiredNumAudioOutput->intValue());
  setPreferedNumAudioOutput(0);
}

void AudioDeviceOutNode::setParentNodeContainer(NodeContainer * parent){
  NodeBase::setParentNodeContainer(parent);
  jassert(parent == NodeManager::getInstance()->mainContainer);
  AudioGraphIOProcessor::setRateAndBufferSizeDetails(NodeBase::getSampleRate(), NodeBase::getBlockSize());
  updateVolMutes();
}

AudioDeviceOutNode::~AudioDeviceOutNode() {
  MessageManagerLock ml;
  getAudioDeviceManager().removeChangeListener(this);
}

void AudioDeviceOutNode::changeListenerCallback(ChangeBroadcaster*) {
  updateVolMutes();
};

void AudioDeviceOutNode::onContainerParameterChanged(Parameter * p){

  if(p==desiredNumAudioOutput){
    setPreferedNumAudioInput(desiredNumAudioOutput->intValue());
  }
  else{
    int foundIdx = volumes.indexOf((FloatParameter*)p);
    if(foundIdx>=0){
      logVolumes.set(foundIdx, float01ToGain(volumes[foundIdx]->floatValue()));
    }
  }
  NodeBase::onContainerParameterChanged(p);

};

void AudioDeviceOutNode::numChannelsChanged(bool isInput){
  NodeBase::numChannelsChanged(isInput);
  updateVolMutes();
}

void AudioDeviceOutNode::updateVolMutes(){

  while(lastNumberOfOutputs < desiredNumAudioOutput->intValue()){
    addVolMute();
  }
  while (lastNumberOfOutputs>desiredNumAudioOutput->intValue()) {
    removeVolMute();
  }


}

void AudioDeviceOutNode::processBlockInternal(AudioBuffer<float>& buffer, MidiBuffer & midiMessages) {
  if (!enabledParam->boolValue()) return;


  int numChannels = jmin(NodeBase::getTotalNumInputChannels() , AudioProcessorGraph::AudioGraphIOProcessor::getTotalNumInputChannels());
  int numSamples = buffer.getNumSamples();

  for (int i = 0; i < numChannels; i++)
  {
    float gain = i<outMutes.size() ? (outMutes[i]->boolValue() ? 0.f : logVolumes[i]):0.0f;
    buffer.applyGainRamp(i, 0, numSamples,lastVolumes[i],gain);
    lastVolumes.set(i ,gain);
  }

  AudioProcessorGraph::AudioGraphIOProcessor::processBlock(buffer, midiMessages);
}


void AudioDeviceOutNode::addVolMute()
{

  BoolParameter * p = addBoolParameter(String(outMutes.size() + 1), "Mute if disabled", false);
  p->setCustomShortName(String("mute") + String(outMutes.size() + 1));
  p->invertVisuals = true;
  outMutes.add(p);

  FloatParameter * v = addFloatParameter("volume"+String(volumes.size()), "volume", DB0_FOR_01);
  volumes.add(v);
  lastVolumes.add(0);
  logVolumes.add(float01ToGain(DB0_FOR_01));
  lastNumberOfOutputs++;
}

void AudioDeviceOutNode::removeVolMute()
{
  if(outMutes.size()==0)return;

  BoolParameter * b = outMutes[outMutes.size() - 1];
  removeControllable(b);
  outMutes.removeAllInstancesOf(b);

  removeControllable(volumes.getLast());
  lastVolumes.removeLast();
  volumes.removeLast();
  logVolumes.removeLast();
  lastNumberOfOutputs--;
}

ConnectableNodeUI * AudioDeviceOutNode::createUI() {
  NodeBaseUI * ui = new NodeBaseUI(this, new AudioDeviceOutNodeContentUI());
  return ui;

}
