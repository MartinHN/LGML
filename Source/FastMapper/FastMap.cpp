/*
 ==============================================================================

 Copyright © Organic Orchestra, 2017

 This file is part of LGML. LGML is a software to manipulate sound in real-time

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation (version 3 of the License).

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

 ==============================================================================
 */

#include "FastMap.h"
#include "../Node/Manager/NodeManager.h"

#include "../Engine.h"


FastMap::FastMap() :
referenceIn (nullptr),
referenceOut (nullptr),
fastMapIsProcessing (false),
ParameterContainer ("FastMap")
{

    referenceIn = addNewParameter<ParameterProxy> ("in param", "parameter for input");
    referenceIn->addParameterProxyListener (this);
    referenceOut = addNewParameter<ParameterProxy> ("out param", "parameter for input");
    referenceOut->addParameterProxyListener (this);
    enabledParam = addNewParameter<BoolParameter> ("Enabled", "Enabled / Disable Fast Map", true);

    inputRange = addNewParameter<RangeParameter> ("In Range", "Input Range", 0.0f, 1.0f, 0.0f, 1.0f);
    outputRange = addNewParameter<RangeParameter> ("Out Range", "Out Range", 0.0f, 1.0f, 0.0f, 1.0f);

    invertParam = addNewParameter<BoolParameter> ("Invert", "Invert the output signal", false);
    fullSync = addNewParameter<BoolParameter> ("FullSync", "synchronize source parameter too", true);
}

FastMap::~FastMap()
{
    referenceOut->removeParameterProxyListener (this);
    referenceIn->removeParameterProxyListener (this);

}
void FastMap::onContainerParameterChanged ( ParameterBase* p)
{
    if (p == invertParam || p == inputRange || p == outputRange || p == fullSync)
    {
        if (referenceIn->get() && referenceOut->get())
        {
            process();
        }
    }
}
void FastMap::process (bool toReferenceOut)
{
    if (!enabledParam->boolValue()) return;

    if (!referenceIn->get() || !referenceOut->get()) return;

    if (fastMapIsProcessing) return;


    auto inRef = toReferenceOut ? referenceIn->get() : referenceOut->get();
    auto sourceVal = (float)inRef->floatValue();
    auto inRange = (toReferenceOut ? inputRange : outputRange);

    float minIn = inRange->getRangeMin();
    float maxIn = inRange->getRangeMax();
    bool newIsInRange = (sourceVal > minIn && sourceVal <= maxIn);

    if (invertParam->boolValue()) newIsInRange = !newIsInRange;

    auto outRef = (toReferenceOut ? referenceOut : referenceIn)->get();

    while (auto* prox = dynamic_cast<ParameterProxy*> (outRef))
    {
        outRef = prox->linkedParam;
    }

    if (!outRef) return;

    fastMapIsProcessing = true;
    auto type = outRef->getFactoryTypeId();

    if (type == Trigger::_factoryType)
    {
        if ((newIsInRange != isInRange && newIsInRange) || inRef->getFactoryTypeId() == Trigger::_factoryType) ((Trigger*)outRef)->trigger();
    }
    else
    {
        if (type == BoolParameter::_factoryType)
        {
            if (inRef->getFactoryTypeId() == Trigger::_factoryType)
            {
                ((BoolParameter*)outRef)->setValue (!outRef->boolValue());
            }
            else
            {
                ((BoolParameter*)outRef)->setValue (newIsInRange);
            }
        }
        else
        {
            if ( minIn != maxIn)
            {
                auto outRange = (toReferenceOut ? outputRange : inputRange);
                float minOut = outRange->getRangeMin();
                float maxOut = outRange->getRangeMax();
                float targetVal = juce::jmap<float> (sourceVal, minIn, maxIn, minOut, maxOut);
                targetVal = juce::jlimit<float> (minOut, maxOut, targetVal);

                if (invertParam->boolValue()) targetVal = maxOut - (targetVal - minOut);

                (( ParameterBase*)outRef)->setValue (targetVal);
            }
        }
    }

    isInRange = newIsInRange;
    fastMapIsProcessing = false;

}






void FastMap::linkedParamValueChanged (ParameterProxy* p)
{
    if (p == referenceIn)
    {
        process();
        return;
    }
    else if (p == referenceOut && fullSync->boolValue())
    {
        process (false);
        return;
    }
};

void FastMap::linkedParamRangeChanged(ParameterProxy* p ) {
    float newMin = 0;
    float newMax = 1;
    float newVmin =0;
    float newVmax = 1;
    RangeParameter * rangeToModify(nullptr);
    if(p==referenceIn){
        if(auto mmp = dynamic_cast<MinMaxParameter*> (referenceIn->linkedParam.get())){
            newMin =  (float)mmp->minimumValue ;
            newMax =  (float)mmp->maximumValue ;
        }
        rangeToModify =  inputRange;
    }
    else if (p==referenceOut){
        if(auto mmp = dynamic_cast<MinMaxParameter*> (referenceOut->linkedParam.get())){
            newMin =  (float)mmp->minimumValue ;
            newMax =  (float)mmp->maximumValue ;
        }
        rangeToModify =  outputRange;
    }
    else{
        jassertfalse;
        return;
    }
    bool remapRange =rangeToModify->hasFiniteRange();
    if(remapRange){
        newVmin = rangeToModify->getNormalizedRangeMin();
        newVmax = rangeToModify->getNormalizedRangeMax();
    }
    rangeToModify->setMinMax (newMin, newMax);
    if(remapRange){
        rangeToModify->setNormalizedRangeMinMax(newVmin,newVmax);
    }

};

void FastMap::linkedParamChanged (ParameterProxy* p)
{

    if (p == referenceIn )
    {
        if (p->linkedParam == referenceOut->linkedParam)
        {
            if (p->linkedParam)
            {

                LOGW(juce::translate("Can't map a parameter to itself"));
                // ignore assert for loopBacks
                //                referenceIn->isSettingValue = false;
                MessageManager::callAsync([this](){
                    referenceIn->setParamToReferTo (nullptr);
                });
            }
        }
        else
        {

            auto* lpar = referenceIn->linkedParam.get();

            while (auto* prox = dynamic_cast<ParameterProxy*> (lpar))
            {
                lpar = prox->linkedParam;
            }

            auto mmp = dynamic_cast<MinMaxParameter*> (lpar);
            float newMin = mmp ? (float)mmp->minimumValue : 0;
            float newMax = mmp ? (float)mmp->maximumValue : 1;
            inputRange->setMinMax (newMin, newMax);

            inputRange->setValue (jmax<float> (inputRange->getRangeMin(),newMin),
                                  jmin<float> (inputRange->getRangeMax(),newMax));



        }

    }
    else if (p == referenceOut)
    {
        if (p->linkedParam == referenceIn->linkedParam)
        {
            if (p->linkedParam)
            {
                LOGW(juce::translate("Can't map a parameter to itself"));
                // ignore assert for loopBacks
                //                referenceOut->isSettingValue = false;
                MessageManager::callAsync([this](){
                    referenceOut->setParamToReferTo (nullptr);
                });
            }
        }
        else if (p->linkedParam && !p->linkedParam->isEditable)
        {
            LOGW(juce::translate("Parameter non editable"));
            // ignore assert for loopBacks
            //            referenceOut->isSettingValue = false;
            MessageManager::callAsync([this](){
                referenceOut->setParamToReferTo (nullptr);
            });
        }
        else
        {

            auto* lpar = referenceOut->linkedParam.get();

            while (auto* prox = dynamic_cast<ParameterProxy*> (lpar))
            {
                lpar = prox->linkedParam;
            }

            auto mmp = dynamic_cast<MinMaxParameter*> (lpar);
            float newMin = mmp ? (float)mmp->minimumValue : 0;
            float newMax = mmp ? (float)mmp->maximumValue : 1;
            outputRange->setMinMax (newMin, newMax);
            outputRange->setValue (jmax<float> (outputRange->getRangeMin(),newMin),
                                   jmin<float> (outputRange->getRangeMax(),newMax));
            
        }
        
    }
    
    
};
