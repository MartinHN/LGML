/*
  ==============================================================================

    JsGlobalEnvironment.cpp
    Created: 9 May 2016 5:17:33pm
    Author:  Martin Hermant

  ==============================================================================
*/

#include "JsGlobalEnvironment.h"
#include "DebugHelpers.h"
#include "TimeManager.h"
#include "NodeManager.h"


juce_ImplementSingleton(JsGlobalEnvironment);


JsGlobalEnvironment::JsGlobalEnvironment(){
    env = new DynamicObject();
    getEnv()->setMethod("post", JsGlobalEnvironment::post);

    // default in global namespace
    linkToControllableContainer("time",TimeManager::getInstance());
    linkToControllableContainer("node",NodeManager::getInstance());

}

void JsGlobalEnvironment::removeNamespace(const String & ns){removeNamespaceFromObject(ns,getEnv());}

DynamicObject * JsGlobalEnvironment::getNamespaceObject(const String & ns){return getNamespaceFromObject(ns,getEnv());}

DynamicObject * JsGlobalEnvironment::getEnv(){return env.getDynamicObject();}

var JsGlobalEnvironment::post(const juce::var::NativeFunctionArgs& a){
    DBG("posting : "+a.thisObject.toString());
    for(int i = 0 ; i < a.numArguments ;i++){
        LOG(a.arguments[i].toString());
    }
    return var();
}


