/*
  ==============================================================================

    RingBuffer.h
    Created: 6 Jun 2016 7:45:00pm
    Author:  Martin Hermant

  ==============================================================================
*/

#ifndef RINGBUFFER_H_INCLUDED
#define RINGBUFFER_H_INCLUDED

#include "JuceHeader.h"

class RingBuffer{
public:

    RingBuffer(int size):ringSize(size),writeNeedle(0),contiguousWriteNeedle(0){
        buf.setSize(2, size);
        buf.clear();

    }
    AudioSampleBuffer buf;

    uint32 ringSize;
    uint32 writeNeedle;

    void writeBlock(AudioSampleBuffer & newBuf){
        int numChans =newBuf.getNumChannels();
        int toCopy = newBuf.getNumSamples();
        buf.setSize(numChans,buf.getNumSamples(),true,true);


        if( writeNeedle + toCopy > ringSize){
            int firstSeg = ringSize-writeNeedle ;
            for(int i = numChans-1;i>=0 ;--i){
                buf.copyFrom(i, writeNeedle, newBuf, i, 0, firstSeg);
            }
            for(int i = numChans-1;i>=0 ;--i){
                buf.copyFrom(i,0,newBuf,i,firstSeg,toCopy-firstSeg);
            }
        }
        else{
            for(int i = numChans-1;i>=0 ;--i){
                buf.copyFrom(i, writeNeedle, newBuf,i, 0, toCopy);            }
        }
        writeNeedle+=toCopy;
        writeNeedle%=ringSize;

        // avoid wrapping errors when checking if contiguous need update
        contiguousWriteNeedle+=1;
    }


    const AudioBuffer<float> & getLastBlock(int num){

        if(num!=contiguousBuffer.getNumSamples() || writeNeedle!=contiguousWriteNeedle){
            updateContiguousBuffer(num);
        }
        else{

        }
        return contiguousBuffer;
    }


private:

    void updateContiguousBuffer(int num){

        jassert(num <= (int)ringSize);
        contiguousBuffer.setSize(buf.getNumChannels(),num);
        int startIdx = writeNeedle-num;
        if(startIdx>=0){
            for(int i = buf.getNumChannels()-1;i>=0 ;--i){
                contiguousBuffer.copyFrom(i, 0, buf, i, startIdx, num);
            }
        }
        else{

            for(int i = buf.getNumChannels()-1;i>=0 ;--i){
                contiguousBuffer.copyFrom(i, 0, buf, i, ringSize+startIdx, -startIdx);
            }
            for(int i = buf.getNumChannels()-1;i>=0 ;--i){
                contiguousBuffer.copyFrom(i,-startIdx,buf,i,0,num+startIdx);
            }
        }
        contiguousWriteNeedle = writeNeedle;
    }
    AudioSampleBuffer contiguousBuffer;
    uint32 contiguousWriteNeedle = 0 ;
};



//==============================================================================
//==============================================================================
#if LGML_UNIT_TESTS

class RingBufferTest  : public UnitTest
{
public:
    RingBufferTest() : UnitTest ("Ring buffer Test") {}



    void runTest() override
    {
        beginTest ("RingBufferTest");


        int numBlocks = 2;
        int blockSize = 512;
        int numChannels = 2;

        int ringSize = numBlocks*blockSize;
        int copiedSize = (numBlocks+2)*(blockSize);


        RingBuffer ring(ringSize);

        AudioBuffer<float> tstBuf;
        tstBuf.setSize(numChannels,blockSize);

        AudioBuffer<float> expected;
        expected.setSize(numChannels,ringSize);


        for (int i = 0 ; i < copiedSize ; i++){

            float dstValue = i;
            for (int c = 0 ;  c < numChannels ; c++){
                tstBuf.setSample(c, i%blockSize, dstValue);

            }
            if((i+1)%blockSize ==0){
                ring.writeBlock(tstBuf);
            }

            if(i-(copiedSize - ringSize)>=0){
                for (int c = 0 ;  c < numChannels ; c++){
                    expected.setSample(c, i-(copiedSize - ringSize), dstValue);
                }
            }


        }


        for(int c = 0  ;c < numChannels ; c++){
            const float * ptr = ring.getLastBlock(ringSize,c);
            for(int i = 0 ; i < expected.getNumSamples() ;i++){
                float fexpected =  expected.getSample(c, i);
                float found = ptr[i];
                expect(found == fexpected ,
                       "failed at :"+String(i)+ ",expect: "+String(fexpected)+"found: "+String(found) );


            }
        }
    }
};

static RingBufferTest ringBufferTest;

#endif





//not used atm
/*
 helper class for bipartite buffer
 allowing having constant access to contiguous memory in a circular buffer
 */
//class BipBuffer{
//
//public:
//    BipBuffer(int size){
//        phantomSize = size;
//        buf.setSize(1,3*size);
//        writeNeedle = 0;
//    }
//
//    void writeBlock(AudioSampleBuffer & newBuf){
//
//        buf.setSize(newBuf.getNumChannels(),buf.getNumSamples());
//
//
//        int toCopy = newBuf.getNumSamples();
//
//        if( phantomSize+writeNeedle + toCopy > 3*phantomSize){
//            int firstSeg = 3*phantomSize-(phantomSize+writeNeedle) ;
//            for(int i = newBuf.getNumChannels()-1;i>=0 ;--i){
//                safeCopy(newBuf.getReadPointer(i,0),firstSeg,i);
//            }
//            for(int i = newBuf.getNumChannels()-1;i>=0 ;--i){
//                safeCopy(newBuf.getReadPointer(i,firstSeg),toCopy-firstSeg,i);
//            }
//        }
//        else{
//            for(int i = newBuf.getNumChannels()-1;i>=0 ;--i){
//                safeCopy(newBuf.getReadPointer(i),toCopy,i);
//            }
//        }
//    }
//
//
//    const float* getLastBlock(int num,int channel=0){
//        jassert(num<phantomSize);
//        return buf.getReadPointer(channel, phantomSize + writeNeedle - num);
//    }
//
//    AudioSampleBuffer buf;
//private:
//    void safeCopy(const float * b,int s,int channel){
//        buf.copyFrom(channel, phantomSize+writeNeedle, b, s);
//
//        if(writeNeedle>2*phantomSize){
//            buf.copyFrom(channel, writeNeedle-2*phantomSize,b,s);
//        }
//        writeNeedle+=s;
//        writeNeedle%=2*phantomSize;
//
//    }
//    int writeNeedle;
//    int phantomSize;
//
//
//
//
//};
//


#endif  // RINGBUFFER_H_INCLUDED