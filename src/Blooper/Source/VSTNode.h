/*
 ==============================================================================

 VSTNode.h
 Created: 2 Mar 2016 8:37:24pm
 Author:  bkupe

 ==============================================================================
 */

#ifndef VSTNODE_H_INCLUDED
#define VSTNODE_H_INCLUDED


#include "VSTManager.h"
#include "NodeBase.h"

#if !HEADLESS
#include "PluginWindow.h"
#endif

AudioDeviceManager& getAudioDeviceManager();


class VSTNode : public NodeBase,public ChangeBroadcaster,public AudioProcessorListener
{

public:
    StringParameter *  identifierString;
    Array<FloatParameter *> VSTParameters;


    VSTNode(NodeManager * nodeManager,uint32 nodeId);
    ~VSTNode();

    void generatePluginFromDescription(PluginDescription * desc);


    class PluginWindowParameters : public ControllableContainer{
    public:
        PluginWindowParameters():ControllableContainer("PluginWindow Parameters")
        {
            x = addFloatParameter("x","x position of plugin window", (float)Random::getSystemRandom().nextInt(500),0.f,1000.f);
            y = addFloatParameter("y","y position of plugin window", (float)Random::getSystemRandom().nextInt (500),0.f,1000.f);
            isDisplayed = addBoolParameter("isDisplayed","is the plugin window displayed",false);
        }

        FloatParameter * x;
        FloatParameter * y;
        BoolParameter * isDisplayed;
    };
    PluginWindowParameters pluginWindowParameter;
#if !HEADLESS
    void createPluginWindow();
    void closePluginWindow();
#endif

    void parameterValueChanged(Parameter * p) override;

    void audioProcessorParameterChanged (AudioProcessor* processor,
                                         int parameterIndex,
                                         float newValue) override;

    void audioProcessorChanged (AudioProcessor*) override{};



    void initParameterFromProcessor(AudioProcessor * p);



    class VSTProcessor : public NodeAudioProcessor{

    public:
        VSTProcessor(VSTNode * _owner):owner(_owner){

        }
        ~VSTProcessor(){}

        AudioProcessorEditor * createEditor()override{
#if !HEADLESS
            if(innerPlugin)return innerPlugin->createEditor();
                else return nullptr;
#else
                    return nullptr;
#endif
        }

        void generatePluginFromDescription(PluginDescription * desc){
            delete innerPlugin.release();
            String errorMessage;
            AudioDeviceManager::AudioDeviceSetup result;

            // set max channels to this
            // TODO check that it actually works
            desc->numInputChannels=jmin(desc->numInputChannels,getMainBusNumInputChannels());
            desc->numOutputChannels=jmin(desc->numOutputChannels,getMainBusNumOutputChannels());


            getAudioDeviceManager().getAudioDeviceSetup (result);
            if (AudioPluginInstance* instance = VSTManager::getInstance()->formatManager.createPluginInstance
                (*desc, result.sampleRate, result.bufferSize, errorMessage)){
                // try to align the precision of the processor and the graph
                instance->setProcessingPrecision (singlePrecision);
                instance->setPreferredBusArrangement (true,  0, AudioChannelSet::canonicalChannelSet (getMainBusNumInputChannels()));
                instance->setPreferredBusArrangement (false,  0, AudioChannelSet::canonicalChannelSet (getMainBusNumOutputChannels()));
                int numIn=instance->getMainBusNumInputChannels();
                int numOut = instance->getMainBusNumOutputChannels();
                setPlayConfigDetails(numIn,numOut,result.sampleRate, result.bufferSize);
                instance->prepareToPlay (result.sampleRate, result.bufferSize);
                // TODO check if scoped pointer deletes old innerPlugin
                innerPlugin=instance;
                owner->initParameterFromProcessor(instance);
            }

            else{

                DBG(errorMessage);
                jassertfalse;
            }
        }


        void numChannelsChanged()override;
        void prepareToPlay(double _sampleRate,int _blockSize)override  {if(innerPlugin){innerPlugin->prepareToPlay(_sampleRate,_blockSize);}}
        void releaseResources() override    {if(innerPlugin){innerPlugin->releaseResources();}};
        bool hasEditor() const override     {if(innerPlugin){return innerPlugin->hasEditor();}return false;};
        void getStateInformation(MemoryBlock & destData)override    {if(innerPlugin){innerPlugin->getStateInformation(destData);};}
        void setStateInformation (const void* data, int sizeInBytes)override    {if(innerPlugin){innerPlugin->setStateInformation(data,sizeInBytes);};};
        void processBlockInternal(AudioBuffer<float>& buffer,MidiBuffer& midiMessages)override{
            if(innerPlugin){
                if( buffer.getNumChannels() >= jmax(innerPlugin->getTotalNumInputChannels(),innerPlugin->getTotalNumOutputChannels()))
                {innerPlugin->processBlock(buffer, midiMessages);}
                else{
                    static int numFrameDropped = 0;
                    DBG("dropAudio " + String(numFrameDropped++));
                }
            }
        };

        VSTNode * owner;
        ScopedPointer<AudioPluginInstance> innerPlugin;
    };

#if !HEADLESS
    NodeBaseUI * createUI()override;
#endif
    bool blockFeedback;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VSTNode)
};



#endif  // VSTNODE_H_INCLUDED
