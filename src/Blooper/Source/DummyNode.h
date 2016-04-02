/*
  ==============================================================================

    DummyNode.h
    Created: 3 Mar 2016 12:31:33pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef DUMMYNODE_H_INCLUDED
#define DUMMYNODE_H_INCLUDED

#include "NodeBase.h"


class DummyNode : public NodeBase
{
public:
    class DummyAudioProcessor : public NodeBase::NodeAudioProcessor
    {
    public:
        DummyAudioProcessor():NodeBase::NodeAudioProcessor(){}
        int step = 0;
        int period = (int)(44100 *1.0f/300);
        float amp = 1.f;
        void processBlockInternal(AudioBuffer<float>& buffer, MidiBuffer&) {

            for(int i = 0 ; i < buffer.getNumSamples() ; i++){
                buffer.addSample(0, i, (float)(amp*cos(2.0*double_Pi*step*1.0/period)));
                step++;
                if(step>period){step = 0;}

            }

        }

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DummyAudioProcessor)
    };


    class DummyDataProcessor : public NodeBase::NodeDataProcessor
    {
    public:
        DummyDataProcessor() :NodeBase::NodeDataProcessor() {}

        virtual void processData(Data *, const String &, const String &) {}

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DummyDataProcessor)
    };



    DummyNode(NodeManager * nodeManager,uint32 nodeId);
    ~DummyNode();

    //parameters
    FloatParameter * testFloatParam;
    Trigger * testTrigger;

    void parameterValueChanged(Parameter * p) override;
#if !HEADLESS
    virtual NodeBaseUI * createUI() override;
#endif

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DummyNode)
};



#endif  // DUMMYNODE_H_INCLUDED
