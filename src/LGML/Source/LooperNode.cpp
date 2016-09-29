/*
 ==============================================================================

 LooperNode.cpp
 Created: 3 Mar 2016 10:32:16pm
 Author:  bkupe

 ==============================================================================
 */

#include "LooperNode.h"
#include "LooperNodeUI.h"
#include "TimeManager.h"

LooperNode::LooperNode() :
NodeBase("Looper",NodeType::LooperType),
selectedTrack(nullptr),
wasMonitoring(false),
trackGroup(this),
streamAudioBuffer(2,16384)// 16000 ~ 300ms and 256*64
{

  numberOfTracks = addIntParameter("numberOfTracks", "number of tracks in this looper", 8, 1, MAX_NUM_TRACKS);
  exportAudio = addTrigger("exportAudio", "export audio of all recorded Tracks");
  selectAllTrig = addTrigger("Select All",        "Select All tracks, for all clear or main volume for instance");
  selectTrack = addIntParameter("Select track",   "set track selected", 0, -1, 0);
  recPlaySelectedTrig = addTrigger("Rec Or Play","Tells the selected track to wait for the next bar and then start record or play");
  playSelectedTrig = addTrigger("Play",           "Tells the selected track to wait for the next bar and then stop recording and start playing");
  stopSelectedTrig = addTrigger("Stop",           "Tells the selected track to stop ");
  clearSelectedTrig = addTrigger("Clear",         "Tells the selected track to clear it's content if got any");
  volumeSelected = addFloatParameter("Volume",    "Set the volume of the selected track",1, 0, 1);
  clearAllTrig = addTrigger("ClearAll",           "Tells all tracks to clear it's content if got any");
  stopAllTrig = addTrigger("StopAll",             "Tells all tracks to stop it's content if got any");
  isMonitoring = addBoolParameter("monitor",      "do we monitor audio input ? ", false);
  preDelayMs = addIntParameter("Pre Delay MS",    "Pre process delay (in milliseconds)", 0, 0, 250);
  quantization = addIntParameter("quantization",       "quantization for this looper - 1 is global", -1, -1, 32);
  isOneShot =  addBoolParameter("isOneShot", "do we play once or loop track", false);
  firstTrackSetTempo = addBoolParameter("firstTrackSetTempo", "do the first track sets the global tempo or use quantization", true);
  addChildControllableContainer(&trackGroup);

  trackGroup.setNumTracks(numberOfTracks->intValue());

  selectTrack->setValue(0,false,true);
  setPlayConfigDetails(2, numberOfTracks->intValue()+2, 44100, 256);
  TimeManager::getInstance()->playState->addParameterListener(this);
}

LooperNode::~LooperNode()
{
  if (TimeManager::getInstanceWithoutCreating()) {
    TimeManager::getInstance()->playState->removeParameterListener(this);
  }
}





void LooperNode::processBlockInternal(AudioBuffer<float>& buffer, MidiBuffer &midiMessages) {


  streamAudioBuffer.writeBlock(buffer);

  // TODO check if we can optimize copies
  // handle multiples channels outs

  jassert(buffer.getNumChannels()>= jmax(getTotalNumInputChannels(),getTotalNumOutputChannels()));
  bufferIn.setSize(getTotalNumInputChannels(), buffer.getNumSamples());
  bufferOut.setSize(getTotalNumOutputChannels(), buffer.getNumSamples());

  if (isMonitoring->boolValue()) {
    if (!wasMonitoring) {
      for (int i = bufferOut.getNumChannels() - 1; i >= 0; --i) {
        bufferOut.copyFromWithRamp(i, 0, buffer.getReadPointer(i), buffer.getNumSamples(), 0.0f, 1.0f);
      }
      wasMonitoring = true;
    }
    else {
      for (int i = bufferOut.getNumChannels() - 1; i >= 0; --i) {
        bufferOut.copyFrom(i, 0, buffer, i, 0, buffer.getNumSamples());
      }
    }
  }
  else {
    if (wasMonitoring) {
      for (int i = bufferOut.getNumChannels() - 1; i >= 0; --i) {
        bufferOut.copyFromWithRamp(i, 0, buffer.getReadPointer(i), buffer.getNumSamples(), 1.0f, 0.0f);
      }
      wasMonitoring = false;
    }
    else {
      bufferOut.clear();
    }
  }

  for (int i = bufferIn.getNumChannels() - 1; i >= 0; --i) {
    bufferIn.copyFrom(i, 0, buffer, i, 0, buffer.getNumSamples());
  }

  int numOutChannels = bufferOut.getNumChannels();
  int numBufferInChannels = bufferIn.getNumChannels();

  for (auto & t : trackGroup.tracks) {
    t->processBlock(buffer, midiMessages);
	
    for (int i = 0; i < numOutChannels; i++) {
		if (i < numBufferInChannels)
		{
			bufferOut.addFrom(i, 0, buffer, i, 0, buffer.getNumSamples());
			buffer.copyFrom(i, 0, bufferIn, i, 0, buffer.getNumSamples());
		} else
		{
			bufferOut.addFrom(i, 0, buffer, i, 0, buffer.getNumSamples());
		}
    }
  }

  for (int i = bufferOut.getNumChannels() - 1; i >= 0; --i) {
    buffer.copyFrom(i, 0, bufferOut, i, 0, buffer.getNumSamples());
  }
}


void LooperNode::TrackGroup::addTrack() {
  LooperTrack * t = new LooperTrack(owner, tracks.size());
  tracks.add(t);
  owner->selectTrack->setRange(-1,tracks.size()-1);
  addChildIndexedControllableContainer(t);
}

void LooperNode::TrackGroup::removeTrack(int i) {
  removeChildControllableContainer(tracks[i]);
  tracks.remove(i);
  owner->selectTrack->setRange(-1,tracks.size()-1);}


void LooperNode::TrackGroup::setNumTracks(int numTracks) {
  int oldSize = tracks.size();
  if (numTracks>oldSize) {
    for (int i = oldSize; i< numTracks; i++) { addTrack(); }
    owner->looperListeners.call(&LooperListener::trackNumChanged, numTracks);
  }
  else {
    owner->looperListeners.call(&LooperListener::trackNumChanged, numTracks);
    for (int i = oldSize - 1; i >= numTracks; --i) { removeTrack(i); }
  }

}


void LooperNode::checkIfNeedGlobalLooperStateUpdate() {
  if(TimeManager::getInstance()->hasMasterCandidate()){
    bool needToReleaseMasterTempo = true;
    for (auto & t : trackGroup.tracks) {
      needToReleaseMasterTempo &= (t->desiredState == LooperTrack::TrackState::CLEARED);

    }

    if (needToReleaseMasterTempo) {
      TimeManager::getInstance()->releaseMasterCandidate(this);
    }
//    if (!isOneShot->boolValue() && needToStop){
//      TimeManager::getInstance()->stopTrigger->trigger();
//    }
  }
}


bool LooperNode::askForBeingMasterTrack(LooperTrack * t) {
  bool res = firstTrackSetTempo->boolValue() && areAllTrackClearedButThis(t);
  if (res)lastMasterTempoTrack = t;
  return res;
}

bool LooperNode::askForBeingAbleToPlayNow(LooperTrack * _t) {
  if(isOneShot->boolValue())return true;
  bool result = true;
  for (auto & t : trackGroup.tracks) {
    if (t != _t)result &=
      (t->trackState == LooperTrack::TrackState::STOPPED) ||
      (t->trackState == LooperTrack::TrackState::CLEARED) ;
  }
  return result;
}

bool LooperNode::areAllTrackClearedButThis(LooperTrack * _t) {
  bool result = true;
  for (auto & t : trackGroup.tracks) {
    if (t != _t)result &= t->trackState == LooperTrack::TrackState::CLEARED;
  }
  return result;
}
int LooperNode::getQuantization(){
  return quantization->intValue()>=0?quantization->intValue():TimeManager::getInstance()->quantizedBarFraction->intValue();
}
void LooperNode::onContainerTriggerTriggered(Trigger * t) {
  if (t == recPlaySelectedTrig) {

    if (selectedTrack != nullptr) selectedTrack->recPlayTrig->trigger();

  }
  else if (t == playSelectedTrig) {

    if (selectedTrack != nullptr) selectedTrack->playTrig->trigger();

  }
  else if (t == clearSelectedTrig) {

    if (selectedTrack != nullptr) selectedTrack->clearTrig->trigger();
    else clearAllTrig->trigger();

  }
  else if (t == stopSelectedTrig) {

    if (selectedTrack != nullptr) selectedTrack->stopTrig->trigger();
    else stopAllTrig->trigger();
  }

  if (t == clearAllTrig) {
    for (int i = trackGroup.tracks.size() - 1; i >= 0; --i) {
      trackGroup.tracks[i]->clearTrig->trigger();
    }
    selectTrack->setValue(0);
    outputVolume->setValue(DB0_FOR_01);
  }
  if (t == stopAllTrig) {
    for (int i = trackGroup.tracks.size() - 1; i >= 0; --i) {
      trackGroup.tracks[i]->stopTrig->trigger();
    }
  }
  if (t == selectAllTrig)
  {
    selectTrack->setValue(-1);

  }
#if !LGML_UNIT_TESTS
  if(t==exportAudio){
    FileChooser myChooser("Please select the directory for exporting audio ...");

    if (myChooser.browseForDirectory())
    {
      File folder(myChooser.getResult());
      WavAudioFormat format;



      for(auto & tr:trackGroup.tracks){
        if(tr->loopSample.getRecordedLength()){
        File f(myChooser.getResult().getChildFile(nameParam->stringValue()+"_"+String(tr->trackIdx)+".wav"));
        ScopedPointer<FileOutputStream> fp;
        if((fp = f.createOutputStream())){
          ScopedPointer<AudioFormatWriter> afw= format.createWriterFor(fp,
                                                         getSampleRate(),
                                                         tr->loopSample.loopSampleBuffer.getNumChannels(),
                                                         24,
                                                         StringPairArray(),0);
          if(afw){
          fp.release();
          afw->writeFromAudioSampleBuffer(tr->loopSample.loopSampleBuffer,0,(int)tr->loopSample.getRecordedLength());
          afw->flush();

          }
        }
        }
      }

    }
  }
#endif

}

void LooperNode::selectMe(LooperTrack * t) {
  if (t != nullptr) {
    for(auto &tt:trackGroup.tracks){
      if(tt->isSelected)
        tt->setSelected(false);
    };
  }

  selectedTrack = t;

  if (selectedTrack != nullptr) {
    selectedTrack->setSelected(true);
    volumeSelected->setValue(selectedTrack->volume->floatValue());
    selectTrack->setValue(selectedTrack->trackIdx);
  }
}

void LooperNode::onContainerParameterChanged(Parameter * p) {
  NodeBase::onContainerParameterChanged(p);

  if (p == numberOfTracks) {

    trackGroup.setNumTracks(numberOfTracks->intValue());
	setPreferedNumAudioOutput(numberOfTracks->intValue() + 2); //Stereo + 1 output per track
  }

  else if (p == volumeSelected)
  {
    if (selectedTrack != nullptr)
    {
      selectedTrack->volume->setValue(volumeSelected->floatValue());
    }
    else
    {
      //define master volume, or all volume ?
    }
  }

  else if (p == TimeManager::getInstance()->playState) {
    if (!TimeManager::getInstance()->playState->boolValue()) {
      for (auto &t : trackGroup.tracks) {
        t->stopTrig->trigger();
      }
    }
    else if(!isOneShot->boolValue()){
      // prevent time manager to update track internal state before all tracks are updated
      TimeManager::getInstance()->lockTime(true);
      for (auto &t : trackGroup.tracks) {
        if(t->trackState!=LooperTrack::CLEARED) t->setTrackState(LooperTrack::TrackState::WILL_PLAY);
      }
      TimeManager::getInstance()->lockTime(false);
    }
  }
  else if(p==selectTrack){
    bool changed = true;
    if( selectedTrack!=nullptr)changed  = selectedTrack->trackIdx!=p->intValue();

    if(changed){
      if(selectTrack->intValue()>=0){
        if(selectTrack->intValue() < trackGroup.tracks.size()){
          trackGroup.tracks.getUnchecked(selectTrack->intValue())->selectTrig->trigger();

        }

      }
      else{
        selectMe(nullptr);
        for(auto & t:trackGroup.tracks){
          t->setSelected(true);
        }

      }
    }
  }
}

void LooperNode::clearInternal(){
  // get called after deletion of TimeManager on app exit
  TimeManager * tm = TimeManager::getInstanceWithoutCreating();
  if (tm != nullptr)
  {
    tm->releaseMasterCandidate(this);
  }
  
}
