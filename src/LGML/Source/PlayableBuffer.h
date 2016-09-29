/*
 ==============================================================================

 PlayableBuffer.h
 Created: 6 Jun 2016 7:45:50pm
 Author:  Martin Hermant

 ==============================================================================
 */

#ifndef PLAYABLEBUFFER_H_INCLUDED
#define PLAYABLEBUFFER_H_INCLUDED





class PlayableBuffer {

  public :
  PlayableBuffer(int numChannels,int numSamples):
  loopSampleBuffer(numChannels,numSamples),
  recordNeedle(0),
  startJumpNeedle(0),
  playNeedle(0),isJumping(false),
  state(BUFFER_STOPPED),
  lastState(BUFFER_STOPPED),
  stateChanged(false),
  numTimePlayed(0),
  sampleOffsetBeforeNewState(0),
  hasBeenFaded (false),fadeSamples(80)
  {

    jassert(numSamples < std::numeric_limits<int>::max());
    //        for (int j = 0 ; j < numSamples ; j++){int p = 44;float t = (j%p)*1.0/p;float v = t;
    //            for(int i = 0 ; i < numChannels ; i++){loopSample.addSample(i, j, v);}
    //        }
	loopSampleBuffer.clear();
  }

  bool processNextBlock(AudioBuffer<float> & buffer, int trackID){

	  const int channels[3] = { 0, 1, trackID + 2 };

	bool succeeded = true;
    if (isFirstRecordingFrame()){
      succeeded = writeAudioBlock(buffer,sampleOffsetBeforeNewState);
    }
    else if(isRecording() ){
      succeeded = writeAudioBlock(buffer);
    }
	else if (wasLastRecordingFrame()) {
		succeeded = writeAudioBlock(buffer, 0, sampleOffsetBeforeNewState);
		fadeInOut(fadeSamples, 0);

	}

    if(isOrWasPlaying()){
      readNextBlock(buffer,channels,3,sampleOffsetBeforeNewState);
    }
    else{
      buffer.clear();
    }
    
    if(isStopping()){
      buffer.clear(sampleOffsetBeforeNewState, buffer.getNumSamples() - sampleOffsetBeforeNewState);
    }





    return succeeded;
  }


	inline bool writeAudioBlock(const AudioBuffer<float> & buffer, int fromSample = 0,int samplesToWrite = -1){

		samplesToWrite= samplesToWrite==-1?buffer.getNumSamples()-fromSample:samplesToWrite;
		if (recordNeedle + buffer.getNumSamples()> loopSampleBuffer.getNumSamples()) {
			jassertfalse;
			return false;
		}
		else{
			const int maxChannel = jmin(loopSampleBuffer.getNumChannels(),buffer.getNumChannels());
			for (int i = 0; i < maxChannel; i++)
			{
				loopSampleBuffer.copyFrom(i, (int)recordNeedle, buffer, i, fromSample, samplesToWrite); //force to channel 0 for now
			}
			recordNeedle += samplesToWrite;
		}

		return true;
	}


  inline void readNextBlock(AudioBuffer<float> & buffer, const int * targetChannels, const int numChannelsToWrite, int fromSample = 0  ){
    if(recordNeedle==0){
      buffer.clear();
      jassertfalse;
      return;
    }
    jassert(isOrWasPlaying());


    int numSamples = buffer.getNumSamples()-fromSample;


    // we want to read Last Block for fade out if stopped
    if(state==BUFFER_STOPPED){
      playNeedle = startJumpNeedle;
    }

    // assert false for now to check alignement
    if(isFirstPlayingFrame())jassert(playNeedle==0);


    // stitch audio jumps by quick fadeIn/Out
    if(isJumping && playNeedle!=startJumpNeedle && state!=BUFFER_STOPPED){
      //LOG("a:jump "<<startJumpNeedle <<","<< playNeedle);

      const int halfBlock =  numSamples/2;
      for (int i = 0; i < numChannelsToWrite; i++) {

		int channel = targetChannels[i];
        //const int maxChannelFromRecorded = jmin(loopSampleBuffer.getNumChannels() - 1, channel);

        buffer.copyFrom(channel, fromSample, loopSampleBuffer, 0, (int)startJumpNeedle, halfBlock);
        buffer.applyGainRamp(channel, fromSample, halfBlock, 1.0f, 0.0f);
        buffer.copyFrom(channel, fromSample+halfBlock, loopSampleBuffer, 0, (int)playNeedle+halfBlock, halfBlock);
        buffer.applyGainRamp(channel, fromSample+halfBlock-1, halfBlock, 0.0f, 1.0f);

      }
    }

    else{
      if ((playNeedle + numSamples) > recordNeedle)
      {


        int firstSegmentLength =(int)(recordNeedle - playNeedle);
        int secondSegmentLength = numSamples - firstSegmentLength;

        if(firstSegmentLength>0 && secondSegmentLength>0){

          //const int maxChannelFromRecorded = jmin(loopSampleBuffer.getNumChannels() , buffer.getNumChannels());
		  for (int i = 0; i < numChannelsToWrite; i++) {
			  int channel = targetChannels[i];
			  const int maxChannelFromRecorded = jmin(loopSampleBuffer.getNumChannels() - 1, channel);

			  buffer.copyFrom(channel, fromSample, loopSampleBuffer, 0, (int)playNeedle, firstSegmentLength);
			  buffer.copyFrom(channel, fromSample, loopSampleBuffer, 0, 0, secondSegmentLength);
          }
          playNeedle = secondSegmentLength;
        }
        else{jassertfalse;}


      }
      else{
        for(int i = 0; i < numChannelsToWrite; i++) {
			int channel = targetChannels[i];
			buffer.copyFrom(channel, fromSample, loopSampleBuffer, 0, (int)playNeedle, numSamples);
        }
      }
    }

    // revert to beginning after reading last block of stopped
    if(state==BUFFER_STOPPED){playNeedle = 0;startJumpNeedle = 0;}
    else{

      playNeedle += numSamples;
      if(playNeedle>=recordNeedle){
        numTimePlayed ++;
      }
      playNeedle %= recordNeedle;

    }

  }


  void setPlayNeedle(int n){
    if(playNeedle!=n){
      if(!isJumping){
        startJumpNeedle = playNeedle;
      }
      isJumping = true;
    }

    playNeedle = n;
    //        std::cout << playNeedle << " : " << std::hex << this << std::endl;


  }

  void cropEndOfRecording(int sampletoRemove){
    jassert(sampletoRemove<recordNeedle);
    recordNeedle-=sampletoRemove;
  }
  void padEndOfRecording(int sampleToAdd){
	  loopSampleBuffer.clear((int)recordNeedle, sampleToAdd);
    recordNeedle+=sampleToAdd;
  }
  void setSizePaddingIfNeeded(uint64 targetSamples){
    jassert(targetSamples<loopSampleBuffer.getNumSamples());
    if(targetSamples>recordNeedle){
      padEndOfRecording((int)(targetSamples - recordNeedle));
    }
    else if (targetSamples<recordNeedle){
      cropEndOfRecording((int)(recordNeedle - targetSamples));
    }

  }

  void fadeInOut(int fadeNumSamples,double mingain){
    if (fadeNumSamples>0 ){
      if(recordNeedle<2 * fadeNumSamples -1) {fadeNumSamples = (int)recordNeedle/2 - 1;}
      for (int i = loopSampleBuffer.getNumChannels() - 1; i >= 0; --i) {
		  loopSampleBuffer.applyGainRamp(i, 0, fadeNumSamples, (float)mingain, 1);
		  loopSampleBuffer.applyGainRamp(i, (int)recordNeedle - fadeNumSamples, fadeNumSamples, 1, (float)mingain);
      }
    }
  }
  inline bool isFirstPlayingFrameAfterRecord()const{return lastState == BUFFER_RECORDING && state == BUFFER_PLAYING;}
  inline bool isFirstStopAfterRec()const{return lastState == BUFFER_RECORDING && state == BUFFER_STOPPED;}
  inline bool isFirstPlayingFrame()const{return lastState!=BUFFER_PLAYING && state == BUFFER_PLAYING;}
  inline bool isFirstRecordingFrame()const{return lastState!=BUFFER_RECORDING && state == BUFFER_RECORDING;}
  inline bool wasLastRecordingFrame()const{return lastState==BUFFER_RECORDING && state != BUFFER_RECORDING;}
  inline bool isStopping() const{return (lastState != BUFFER_STOPPED  ) && (state==BUFFER_STOPPED);}
  inline bool isStopped() const{return (state==BUFFER_STOPPED);}
  inline bool isRecording() const{return state == BUFFER_RECORDING;}
  inline bool isPlaying() const{return state == BUFFER_PLAYING;}
  inline bool isFirstRecordedFrame() const{return state == BUFFER_RECORDING && (lastState!=BUFFER_RECORDING);}
  inline bool isOrWasPlaying() const{return (state==BUFFER_PLAYING || lastState==BUFFER_PLAYING) &&  recordNeedle>0 && loopSampleBuffer.getNumSamples();}
  inline bool isOrWasRecording() const{return (state==BUFFER_RECORDING || lastState==BUFFER_RECORDING) && loopSampleBuffer.getNumSamples();}


  void startRecord(){recordNeedle = 0;playNeedle=0;}
  inline void startPlay(){setPlayNeedle(0);}

  bool checkTimeAlignment(uint64 curTime,const int minQuantifiedFraction){

    if(state == BUFFER_PLAYING  && recordNeedle>0){


      int globalPos =(curTime%minQuantifiedFraction);
      int localPos =(playNeedle%minQuantifiedFraction);
      if(globalPos!=localPos){
        if(!isJumping)startJumpNeedle = playNeedle;
        playNeedle = (playNeedle - localPos) + globalPos;
        isJumping = true;
        jassertfalse;


      }
    }

    return !isJumping;
  }




  enum BufferState {
    BUFFER_STOPPED = 0,
    BUFFER_PLAYING,
    BUFFER_RECORDING

  };

  void setState(BufferState newState,int _sampleOffsetBeforeNewState=0){
    //        lastState = state;
    stateChanged |=newState!=state;
    switch (newState){
      case BUFFER_RECORDING:
        hasBeenFaded = false;
        recordNeedle = 0;
        numTimePlayed = 0;
        startJumpNeedle = 0;
        setPlayNeedle(0);
        break;
      case BUFFER_PLAYING:
        setPlayNeedle( 0);
        break;
      case BUFFER_STOPPED:
        numTimePlayed = 0;
        setPlayNeedle(0);
        break;
    }
    state = newState;
    sampleOffsetBeforeNewState = _sampleOffsetBeforeNewState;
  }

  void endProcessBlock(){
    lastState = state;
    stateChanged =false;
    isJumping = false;
    startJumpNeedle=0;
    sampleOffsetBeforeNewState = 0;
  }

  BufferState getState() const{
    return state;
  }

  BufferState getLastState() const{
    return lastState;
  }


  uint64 getRecordedLength() const{return recordNeedle;}

  uint64 getPlayPos() const{return playNeedle;}


  bool stateChanged;

  uint64 getStartJumpPos() const{return startJumpNeedle;}




  int numTimePlayed;
  AudioSampleBuffer loopSampleBuffer;

  int getSampleOffsetBeforeNewState(){return sampleOffsetBeforeNewState;};



#if !LGML_UNIT_TESTS
private:
#endif

  int sampleOffsetBeforeNewState;
  BufferState state;
  BufferState lastState;
  bool isJumping;
  bool hasBeenFaded;
  int fadeSamples;


  
  uint64 recordNeedle,playNeedle,startJumpNeedle;

};



#endif  // PLAYABLEBUFFER_H_INCLUDED
