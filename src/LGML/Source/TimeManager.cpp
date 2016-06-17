/*
 ==============================================================================

 TimeManager.cpp
 Created: 2 Mar 2016 8:33:44pm
 Author:  bkupe

 ==============================================================================
 */

#include "TimeManager.h"


juce_ImplementSingleton(TimeManager);

#include "NodeBase.h"
#include "DebugHelpers.h"


TimeManager::TimeManager():
beatTimeInSample(22050),
sampleRate(44100),
ControllableContainer("time"),

_isLocked(false)
{

    BPM = addFloatParameter("bpm","current BPM",120,10,600);
    playState = addBoolParameter("Play_Stop", "play or stop global transport", false);
    isSettingTempo = addBoolParameter("isSettingTempo", "is someone setting tempo (recording first loop)", false);
    currentBar  = addIntParameter("currentBar", "currentBar in transport", 0, 0, 9999999);
    currentBeat  = addIntParameter("currentBeat", "currentBeat in transport", 0, 0, 999999);
    beatPerBar = addIntParameter("beatPerBar", "beat Per Bar", 4, 1, 8);
    playTrigger = addTrigger("play", "trigger play");
    stopTrigger = addTrigger("stop", "trigger stop");
    quantizedBarFraction = addIntParameter("globalQuantization", "Global quantization in fraction of a bar", 1, 0, 16);
}
TimeManager::~TimeManager()
{
}


void TimeManager::incrementClock(int time){
    updateState();
    if(_isLocked)return;
    int lastBeat = getBeat();


    if(timeState.isPlaying){
        timeState.time+=time;
    }

    int newBeat = getBeat();
    if(lastBeat!=newBeat){
        currentBeat->setValue(newBeat);
        if(newBeat%((int)beatPerBar->value) == 0){
            currentBar->setValue(getBar());

        }
    }
 desiredTimeState =timeState;

}


void TimeManager::audioDeviceIOCallback (const float** /*inputChannelData*/,
                                         int /*numInputChannels*/,
                                         float** outputChannelData,
                                         int numOutputChannels,
                                         int numSamples) {
    incrementClock(numSamples);

    for (int i = 0; i < numOutputChannels; ++i)
        zeromem (outputChannelData[i], sizeof (float) * (size_t) numSamples);
}

bool TimeManager::askForBeingMasterCandidate(TimeMasterCandidate * n){
    potentialTimeMasterCandidate.addIfNotAlreadyThere(n);
    return potentialTimeMasterCandidate.getUnchecked(0) == n;
}

bool TimeManager::isMasterCandidate(TimeMasterCandidate * n){
    return potentialTimeMasterCandidate.size()>0 && n==potentialTimeMasterCandidate.getUnchecked(0);
}
bool TimeManager::hasMasterCandidate(){
    return potentialTimeMasterCandidate.size()>0;
}
void TimeManager::releaseMasterCandidate(TimeMasterCandidate * n){
    potentialTimeMasterCandidate.removeFirstMatchingValue(n);
    if(potentialTimeMasterCandidate.size()==0||
       (potentialTimeMasterCandidate.size()==1 && potentialTimeMasterCandidate.getUnchecked(0)==this)  ){
        stopTrigger->trigger();
    }
}

void TimeManager::onContainerParameterChanged(Parameter * p){
    if(p==playState){
        if(!playState->boolValue()){
            releaseMasterCandidate(this);
            shouldStop();
        }
        else{
            if (!hasMasterCandidate()) {
                askForBeingMasterCandidate(this);
            }
            shouldPlay();
        }
    }
    else if(p==BPM){
        setBPMInternal(BPM->doubleValue());
    }
    else if(p==beatPerBar){
        currentBeat->maximumValue = beatPerBar->intValue();
    }

};

void TimeManager::shouldStop(){
    desiredTimeState.isPlaying = false;
    desiredTimeState.time = 0;
}

void TimeManager::shouldRestart(bool playing){
    desiredTimeState.isPlaying=playing;
    desiredTimeState.time = 0;
}
void TimeManager::shouldGoToZero(){
    desiredTimeState.time = 0;
}
void TimeManager::shouldPlay(){
    desiredTimeState.isPlaying = true;
}

void TimeManager::updateState(){
    String dbg;
//    if(timeState.isPlaying != desiredTimeState.isPlaying){
//        dbg+="play:"+String(timeState.isPlaying)+"/"+String(desiredTimeState.isPlaying);
//    }
//    if(timeState.time != desiredTimeState.time){
//        dbg+=" ::: time:"+String(timeState.time)+"/"+String(desiredTimeState.time);
//    }
//    if(dbg!=""){
//        LOG(dbg);
//    }

    timeState = desiredTimeState;
}

void TimeManager::onContainerTriggerTriggered(Trigger * t) {
    if(t == playTrigger){
        playState->setValue(false);
        playState->setValue(true);
    }

    if(t==stopTrigger){
        playState->setValue(false);
        isSettingTempo->setValue(false);
    }
}
void TimeManager::togglePlay(){
    if(playState->boolValue()){
        stopTrigger->trigger();
    }
    else
        playTrigger->trigger();
}



void TimeManager::setSampleRate(int sr){
    sampleRate = sr;
    // actualize beatTime in sample
    beatTimeInSample = (int)(sampleRate*60.0 / BPM->doubleValue());
}

void TimeManager::setBPMInternal(double){
    isSettingTempo->setValue(false);
    beatTimeInSample =(float)(sampleRate*60.0 / BPM->doubleValue());
}
uint64 TimeManager::getTimeInSample(){return timeState.time;}


void TimeManager::jump(int amount){
    desiredTimeState.time = timeState.time+amount;
}




int TimeManager::setBPMForLoopLength(int time,int granularity){
    double time_seconds = time* 1.0/ sampleRate;
    double beatTime = time_seconds* 1.0/beatPerBar->intValue();
    float barLength = 1;

    // over 150 bpm
    if(beatTime < .40){beatTime*=2;barLength/=2;}
    // under 60 bpm
    else if(beatTime > 1){beatTime/=2;barLength*=2;}
    if(granularity>0){
		int beatInSample = (int)(beatTime*sampleRate);
        beatInSample = beatInSample - beatInSample%granularity;
        beatTime = beatInSample*1.0/sampleRate;
    }
    


    BPM->setValue(60.0/beatTime);
    shouldGoToZero();
    return (int) (barLength*beatPerBar->intValue());
}

int TimeManager::getNextGlobalQuantifiedTime(){
    return getNextQuantifiedTime(quantizedBarFraction->intValue());
}
int TimeManager::getNextQuantifiedTime(int barFraction){
    if (barFraction==-1){
        barFraction=quantizedBarFraction->intValue();
    }
    if(barFraction==0){
        return (int)timeState.time;
    }

    const int samplesPerUnit = (beatTimeInSample*beatPerBar->intValue()/barFraction);
    return (int) ((floor(timeState.time/samplesPerUnit) + 1)*samplesPerUnit);
}

uint64 TimeManager::getTimeForNextBeats(int beats){
    return (getBeat()+ beats)*beatTimeInSample;

}

int TimeManager::getBeat(){return (int)(floor(timeState.time*1.0/beatTimeInSample));}
double TimeManager::getBeatPercent(){return timeState.time*1.0/beatTimeInSample-getBeat();}

int TimeManager::getBar(){return (int)(floor(getBeat()*1.0/beatPerBar->intValue() ));}

void TimeManager::lockTime(bool s){
    _isLocked = s;
}
bool TimeManager::isLocked(){
    return _isLocked;
}
bool TimeManager::getCurrentPosition (CurrentPositionInfo& result){
    result.bpm = BPM->doubleValue();
    result.isPlaying = playState->boolValue();
    result.isRecording = isSettingTempo->boolValue();
    //TODO: check
    result.ppqPosition = (double)(getBeat()+getBeatPercent())/beatPerBar->intValue();
    result.ppqPositionOfLastBarStart = (double)(getBar()*beatPerBar->intValue());
    result.ppqLoopStart = 0;
    result.ppqLoopEnd = 0;
    result.timeSigNumerator = beatPerBar->intValue();
    result.timeSigDenominator = 4;
    result.timeInSamples = timeState.time;
    result.timeInSeconds = (double)(timeState.time)*sampleRate;
    result.editOriginTime = 0;

    result.isLooping=false;
    return true;
}
