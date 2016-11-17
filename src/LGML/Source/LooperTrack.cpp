/*
 ==============================================================================

 LooperTrack.cpp
 Created: 26 Apr 2016 4:11:41pm
 Author:  bkupe

 ==============================================================================
 */


#include "LooperTrack.h"
#include "TimeManager.h"

#include "LooperNode.h"

#include "DebugHelpers.h"

#define NO_QUANTIZE (uint64)-1 //std::numeric_limits<uint64>::max()

LooperTrack::LooperTrack(LooperNode * looperNode, int _trackIdx) :
ControllableContainer(String(_trackIdx)),
parentLooper(looperNode),
quantizedRecordStart(NO_QUANTIZE),
quantizedRecordEnd(NO_QUANTIZE),
quantizedPlayStart(NO_QUANTIZE),
quantizedPlayEnd(NO_QUANTIZE),
loopSample(2, 44100 * MAX_LOOP_LENGTH_S),
trackState(CLEARED),
desiredState(CLEARED),
trackIdx(_trackIdx),
someOneIsSolo(false),
isSelected (false),
originBPM(0),
lastVolume(0)
{


  selectTrig = addTrigger("Select", "Select this track");
  recPlayTrig = addTrigger("Rec Or Play", "Tells the track to wait for the next bar and then start record or play");
  playTrig = addTrigger("Play", "Tells the track to wait for the next bar and then stop recording and start playing");
  stopTrig = addTrigger("Stop", "Tells the track to stop ");
  togglePlayStopTrig = addTrigger("Toggle Play Stop", "Toggle Play / Stop");

  clearTrig = addTrigger("Clear", "Tells the track to clear it's content if got any");
  volume = addFloatParameter("Volume", "Set the volume of the track", defaultVolumeValue, 0, 1);
  mute = addBoolParameter("Mute", "Sets the track muted (or not.)", false);
  solo = addBoolParameter("Solo", "Sets the track solo (or not.)", false);
  beatLength = addFloatParameter("Length", "length in bar", 0, 0, 200);

  mute->invertVisuals = true;

  stateParameterString = addStringParameter("state", "track state", "");
  stateParameterStringSynchronizer = new AsyncTrackStateStringSynchronizer(stateParameterString);
  addTrackListener(stateParameterStringSynchronizer);
  stateParameterString->isControllableFeedbackOnly = true;
  stateParameterString->isSavable = false;



  // post init
  volume->setValue(defaultVolumeValue);
  logVolume = float01ToGain(volume->value);

}

void LooperTrack::processBlock(AudioBuffer<float>& buffer, MidiBuffer &) {


  //bool stateChanged  =
//  updatePendingLooperTrackState(TimeManager::getInstance()->getTimeInSample(), buffer.getNumSamples());


  handleStartOfRecording();



  if(!loopSample.processNextBlock(buffer) && trackState != STOPPED){
    LOG("Stopping, too many audio (more than 1mn)");
    setTrackState(STOPPED);
  }


  if(loopSample.isPlaying()){
    trackStateListeners.call(&LooperTrack::Listener::internalTrackTimeChanged,loopSample.getPlayPos()*1.0/(1+loopSample.getRecordedLength()));
  }

  handleEndOfRecording();


  float newVolume = ((someOneIsSolo && !solo->boolValue()) || mute->boolValue()) ? 0 : logVolume;

  for (int i = buffer.getNumChannels()- 1; i >= 0; --i) {
    buffer.applyGainRamp(i, 0, buffer.getNumSamples(), lastVolume, newVolume);
  }

  lastVolume = newVolume;


  loopSample.endProcessBlock();



}
bool LooperTrack::updatePendingLooperTrackState( uint64 curTime, int blockSize) {

  // the sample act as free running clock when no quantization
  if(getQuantization()==0) curTime = loopSample.getGlobalPlayPos();
  jassert (curTime>=0);



  bool stateChanged = (trackState != desiredState);

  if(shouldWaitFirstOnset() && desiredState == WILL_RECORD){
    if(!hasOnset()){
      quantizedRecordStart=curTime+blockSize;
    }
    else{
      quantizedRecordStart=curTime;
//      int dbg;dbg=0;
    }
  }




  if(stateChanged){
    //        LOG(trackStateToString(trackState) << ":" << trackStateToString(desiredState));
    if(desiredState == WILL_STOP  ){

      loopSample.setState( PlayableBuffer::BUFFER_STOPPED);
      desiredState = STOPPED;
      cleanAllQuantizeNeedles();
      stateChanged = true;
    }

    else if(desiredState==CLEARED  ){
      loopSample.setState( PlayableBuffer::BUFFER_STOPPED);
      cleanAllQuantizeNeedles();
      stateChanged = loopSample.stateChanged;
    }
  }



  ////
  // apply quantization on play / rec
  const uint64 triggeringTime = curTime+blockSize;

  TimeManager * tm = TimeManager::getInstance();

  if (quantizedRecordStart!=NO_QUANTIZE) {
    if (triggeringTime > quantizedRecordStart ) {
      int firstPart = jmax(0, (int)(quantizedRecordStart-curTime));
      int secondPart = (int)(triggeringTime-firstPart);


      if(isMasterTempoTrack() ){
        if(!tm->playState->boolValue()){
          tm->playState->setValue(true);

        }
        // we will handle the block in this call so we notify time to be in sync with what we play
        tm->goToTime(secondPart);

      }

      desiredState = RECORDING;
      loopSample.setState( PlayableBuffer::BUFFER_RECORDING,firstPart);
      startRecBeat = TimeManager::getInstance()->getBeatInNextSamples(firstPart);
      quantizedRecordStart = NO_QUANTIZE;
      stateChanged = true;
    }
    else{
      //int waiting = 0;
    }

  }
  else if (quantizedRecordEnd!=NO_QUANTIZE) {
    if (triggeringTime > quantizedRecordEnd) {
      int firstPart = jmax(0, (int)(quantizedRecordEnd-curTime));
      //      int secondPart = triggeringTime-firstPart;

      if(parentLooper->isOneShot->boolValue()){
        loopSample.setState( PlayableBuffer::BUFFER_STOPPED,firstPart);
        desiredState = STOPPED;
      }
      else{
        if(isMasterTempoTrack() ){
          TimeManager::getInstance()->playState->setValue(true,false,true);

        }

        loopSample.setState( PlayableBuffer::BUFFER_PLAYING,firstPart);
        desiredState = PLAYING;
        quantizedPlayStart = curTime+firstPart;
      }
      quantizedRecordEnd = NO_QUANTIZE;
      stateChanged = true;

    }
  }



  if (quantizedPlayStart!=NO_QUANTIZE) {
    if (triggeringTime > quantizedPlayStart) {
      int firstPart = jmax(0, (int)(quantizedPlayStart-curTime));
      //      int secondPart = triggeringTime-firstPart;

      desiredState =  PLAYING;
      loopSample.setState( PlayableBuffer::BUFFER_PLAYING,firstPart);
      startPlayBeat = TimeManager::getInstance()->getBeatInNextSamples(firstPart);

      // stop oneShot if needed
      if(parentLooper->isOneShot->boolValue() ){
        quantizedPlayEnd = quantizedPlayStart + loopSample.getRecordedLength() - 2*loopSample.getNumSampleFadeOut();
      }

      quantizedPlayStart = NO_QUANTIZE;
      stateChanged = true;


    }
  }
  else if (quantizedPlayEnd!=NO_QUANTIZE) {
    if (triggeringTime > quantizedPlayEnd) {
      int firstPart = jmax(0, (int)(quantizedPlayEnd-curTime));
      //      int secondPart = triggeringTime-firstPart;
      desiredState = STOPPED;
      loopSample.setState( PlayableBuffer::BUFFER_STOPPED,firstPart);
      quantizedPlayEnd = NO_QUANTIZE;
      stateChanged = true;
    }
  }




  stateChanged|=loopSample.stateChanged;

  trackState = desiredState ;




  //    DBG(playNeedle);
  if(stateChanged){

    trackStateListeners.call(&LooperTrack::Listener::internalTrackStateChanged, trackState);
    // DBG("a:"+trackStateToString(trackState));

  }
//  if(getQuantization()>0 && !isMasterTempoTrack() && trackState == PLAYING) {
//    TimeManager * tm = TimeManager::getInstance();
//    loopSample.checkTimeAlignment(curTime,tm->beatTimeInSample/getQuantization());
//  }

  return stateChanged;





}

int LooperTrack::getQuantization(){
  return parentLooper->getQuantization();
}

void LooperTrack::handleStartOfRecording(){
  TimeManager * tm = TimeManager::getInstance();
  if (loopSample.stateChanged) {
    //    process changed internalState

    if (loopSample.isFirstRecordedFrame()) {
      if (isMasterTempoTrack()) {
        int samplesToGet = (int)(parentLooper->preDelayMs->intValue()*0.001f*parentLooper->getSampleRate());
        //        we need to advance because pat of the block may have be processed
        tm->advanceTime(samplesToGet);
        if(samplesToGet>0){ loopSample.writeAudioBlock(parentLooper->streamAudioBuffer.getLastBlock(samplesToGet));}
        startRecBeat = 0;
      }
      else{
        startRecBeat = TimeManager::getInstance()->getBeatInNextSamples(loopSample.getSampleOffsetBeforeNewState());

      }

    }
  }
}

void LooperTrack::handleEndOfRecording(){

  if (loopSample.stateChanged) {
    if (loopSample.wasLastRecordingFrame() ){
      //            DBG("a:firstPlay");
      // get howMuch we have allready played in loopSample
      int offsetForPlay = (int)loopSample.getPlayPos();
      if (isMasterTempoTrack()) {
        //                DBG("release predelay : "+String (trackIdx));
        const int sampleToRemove = (int)(parentLooper->preDelayMs->intValue()*0.001f*parentLooper->getSampleRate());
        if(sampleToRemove>0){loopSample.cropEndOfRecording(sampleToRemove);}
        double actualLength = TimeManager::getInstance()->setBPMForLoopLength(loopSample.getRecordedLength());
        uint64 desiredSize = (uint64)(actualLength*TimeManager::getInstance()->beatTimeInSample);

        //        DBG("resizing loop : " << (int)(desiredSize-loopSample.getRecordedLength()));

        loopSample.setSizePaddingIfNeeded(desiredSize);
        beatLength->setValue(loopSample.getRecordedLength()*1.0/TimeManager::getInstance()->beatTimeInSample);
        TimeManager::getInstance()->goToTime(offsetForPlay);



      }
      else{
        beatLength->setValue(loopSample.getRecordedLength()*1.0/TimeManager::getInstance()->beatTimeInSample);
      }
      if(getQuantization()>0)originBPM = TimeManager::getInstance()->BPM->doubleValue();
      //      loopSample.setPlayNeedle(offsetForPlay);

      parentLooper->lastMasterTempoTrack =nullptr;
    }
  }


  if( loopSample.isFirstPlayingFrame()){
    startPlayBeat = TimeManager::getInstance()->getBeatInNextSamples(loopSample.getSampleOffsetBeforeNewState());
  }


}


String LooperTrack::trackStateToString(const TrackState & ts) {

  switch (ts) {

    case CLEARED:
      return "cleared";
    case PLAYING:
      return "playing";
    case RECORDING:
      return "recording";
    case WILL_PLAY:
      return "willPlay";
    case WILL_RECORD:
      return "willRecord";
    case STOPPED:
      return "stopped";
    case WILL_STOP:
      return "willStop";


    default:
      jassertfalse;
      break;
  }

  return "[noState]";
}

void LooperTrack::onContainerParameterChanged(Parameter * p)
{

  if (p == volume)
  {
    if (parentLooper->selectedTrack == this) parentLooper->volumeSelected->setValue(volume->floatValue());
    logVolume = float01ToGain(volume->value);

  }
  if(p==solo){
    someOneIsSolo = false;
    for(auto &t:parentLooper->trackGroup.tracks){
      someOneIsSolo |= t->solo->boolValue();
    }
    for(auto &t:parentLooper->trackGroup.tracks){
      t->someOneIsSolo = someOneIsSolo;
    }
  }
}

void LooperTrack::onContainerTriggerTriggered(Trigger * t) {
  if (t == selectTrig)
  {
    parentLooper->selectMe(this);

  }
  else if (t == recPlayTrig) {

    if (desiredState == CLEARED) {
      setTrackState(WILL_RECORD);
    }
    else  if(desiredState!=WILL_RECORD){
      if(parentLooper->isOneShot->boolValue() && desiredState==RECORDING){
        setTrackState(WILL_STOP);
      }
      else{setTrackState(WILL_PLAY);}
    }

  }
  else if (t == playTrig) {
    if(!isEmpty()) setTrackState(WILL_PLAY); //avoid playing if empty;
	
  }
  else if (t == clearTrig) {
    setTrackState(CLEARED);
    volume->setValue(DB0_FOR_01);
    mute->setValue(false);
  }
  else if (t == stopTrig) {
    setTrackState(WILL_STOP);
  } else if (t == togglePlayStopTrig)
  {
	  setTrackState(trackState != PLAYING ? WILL_PLAY : WILL_STOP);
  }
}

bool LooperTrack::askForBeingMasterTempoTrack() {
  if(getQuantization()>0){

    if(TimeManager::getInstance()->askForBeingMasterCandidate(parentLooper))
    {
      if(parentLooper->askForBeingMasterTrack(this)){
        return true;
      }
    }
    return false;
  }
  else return false;
}

bool LooperTrack::isMasterTempoTrack(){
  return TimeManager::getInstance()->isMasterCandidate(parentLooper)
  && parentLooper->lastMasterTempoTrack  == this;
}

void LooperTrack::setSelected(bool _isSelected) {
  isSelected=_isSelected;
  trackStateListeners.call(&LooperTrack::Listener::internalTrackSetSelected, isSelected);
}

bool LooperTrack::isEmpty()
{
	return trackState == TrackState::CLEARED;
}


void LooperTrack::askForSelection(bool) {
  selectTrig->trigger();
}


void LooperTrack::setTrackState(TrackState newState) {

  if (newState == trackState) return;

  int quantizeTime = getQuantization();
  TimeManager * timeManager = TimeManager::getInstance();

  if(newState==desiredState)return;

  if (newState == WILL_RECORD && (!shouldWaitFirstOnset() || hasOnset())) {
    // are we able to set the tempo

    if (askForBeingMasterTempoTrack()) {
      timeManager->isSettingTempo->setValue(true);
      // start As soon as possible
      quantizedRecordStart = 0;
    }

    else if (!timeManager->isSettingTempo->boolValue()) {
      if(parentLooper->askForBeingAbleToPlayNow(this) && !timeManager->playState->boolValue()) {
        if(getQuantization()>0)
          timeManager->playTrigger->trigger();
        quantizedRecordStart = 0;

      }
      else{
        quantizedRecordStart = timeManager->getNextQuantifiedTime(quantizeTime);

      }
    }
    //            Record per default if triggering other rec while we are current master and we are recording

    else if (timeManager->isMasterCandidate(parentLooper)) {
      timeManager->isSettingTempo->setValue(false);
      jassert(parentLooper->lastMasterTempoTrack);
      parentLooper->lastMasterTempoTrack->setTrackState(WILL_PLAY);
      quantizedRecordStart = 0;
    }
    // ignore in other cases (ask recording while another is setting tempo)
    else{
      newState = desiredState;
    }
  }


  // on should play
  else if (newState == WILL_PLAY) {


    // end of first track
    if ( trackState == RECORDING ){
      if(isMasterTempoTrack() ) {
        quantizedRecordEnd = 0;
        timeManager->isSettingTempo->setValue(false);
        //            timeManager->lockTime(true);

        //        int minRecordTime = (int)(parentLooper->getSampleRate()*0.1f);
        //        minRecordTime-= minRecordTime%parentLooper->getBlockSize();
        //        if(loopSample.getRecordedLength()< minRecordTime){
        //          // avoid feedBack when trigger play;
        //          newState = WILL_RECORD;
        //          desiredState = WILL_RECORD;
        //          quantizedRecordEnd =minRecordTime;
        //        }
        //        else{
        newState=WILL_PLAY;
        quantizedPlayStart = 0;
        //        }
      }
      else{
        if(getQuantization()>0)
          quantizedRecordEnd = timeManager->getNextQuantifiedTime(quantizeTime);
        else
          quantizedRecordEnd = 0;

      }

    }
    // if every one else is stopped
    else if(parentLooper->askForBeingAbleToPlayNow(this) && !loopSample.isOrWasPlaying()) {
      quantizedRecordEnd = NO_QUANTIZE;

      if(timeManager->isMasterCandidate(parentLooper)){
        newState=WILL_PLAY;
        bool wasLocked = timeManager->isLocked();
        if(!wasLocked)timeManager->lockTime(true);
        timeManager->playTrigger->trigger();
        quantizedPlayStart = 0;
        if(!wasLocked)timeManager->lockTime(false);
      }
      else{
        quantizedPlayStart = 0;
      }
    }
    // a cleared track can't be played
    else  if(trackState==CLEARED){
      newState=CLEARED;
    }



    // on ask for play
    else{// if(timeManager->playState->boolValue()){
      cleanAllQuantizeNeedles();
      if(getQuantization()==0) quantizedPlayStart=0;
      else quantizedPlayStart = timeManager->getNextQuantifiedTime(quantizeTime);
      //            quantizedPlayStart = timeManager->getNextQuantifiedTime(1.0/beatLength->intValue());
      //            quantizedPlayStart = timeManager->getTimeForNextBeats(beatLength->value);

    }
  }


  // on should clear
  if (newState == CLEARED) {


    if (parentLooper->currentPreset != nullptr)
    {
      volume->setValue(parentLooper->getPresetValueFor(volume));
      mute->setValue(parentLooper->getPresetValueFor(mute));
      solo->setValue(parentLooper->getPresetValueFor(solo));
    }else
    {
      volume->resetValue();
      mute->resetValue();
      solo->resetValue();
    }
  }


  if (newState == WILL_STOP) {
    // force a track to stay in cleared state if stop triggered
    if (trackState == CLEARED || desiredState==CLEARED) { newState = CLEARED; }
  }
  //DBG(newState <<","<<trackState );
  //    DBG(trackStateToString(trackState));

  if(desiredState!=newState){
    desiredState = newState;
    trackStateListeners.call(&LooperTrack::Listener::internalTrackStateChanged, desiredState);


    if((desiredState==CLEARED  || desiredState==STOPPED || desiredState==WILL_STOP) ){
      parentLooper->checkIfNeedGlobalLooperStateUpdate();
    }
  }
};

void LooperTrack::cleanAllQuantizeNeedles() {
  quantizedPlayEnd = NO_QUANTIZE;
  quantizedPlayStart = NO_QUANTIZE;
  quantizedRecordEnd = NO_QUANTIZE;
  quantizedRecordStart = NO_QUANTIZE;
}


bool LooperTrack::shouldWaitFirstOnset(){
  return (trackState == WILL_RECORD || desiredState==WILL_RECORD) && parentLooper->waitForOnset->boolValue();
}

bool LooperTrack::hasOnset(){
  return parentLooper->hasOnset();
}

void LooperTrack::setNumChannels(int numChannels){
  loopSample.setNumChannels(numChannels);
}


//Component * LooperTrack::createDefaultUI(Component * ) {
//    return nullptr;
//}
