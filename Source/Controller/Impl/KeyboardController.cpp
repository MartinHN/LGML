/*
 ==============================================================================

 KeyboardController.cpp
 Created: 3 Mar 2018 4:04:01pm
 Author:  Martin Hermant

 ==============================================================================
 */

#if !ENGINE_HEADLESS

#include "KeyboardController.h"
#include "../../Scripting/Js/JsHelpers.h"

#include "../ControllerFactory.h"
REGISTER_CONTROLLER_TYPE (KeyboardController,"Keyboard");
Identifier JsKeyboardListener::keyRecievedId("onKey");

KeyboardController::KeyboardController(StringRef name):
Controller(name),
JsEnvironment ("controllers.Keyboard", this)
{

    logIncoming = addNewParameter<BoolParameter> ("logIncoming", "log Incoming keyboard event", false);
    logIncoming->isSavable = false;
    logIncoming->isPresettable =false;

    if(ComponentPeer::getNumPeers()>0){
        Component * mainComp =&ComponentPeer::getPeer(0)->getComponent();
        mainComp->addKeyListener(this);
        isConnected->setValue(true);
    }
    else{
        isConnected->setValue(false);
        LOGE(juce::translate("can't find component for keyboard controller "));
    }
};
KeyboardController::~KeyboardController(){
    if(ComponentPeer::getNumPeers()>0){
        Component * mainComp =&ComponentPeer::getPeer(0)->getComponent();

        mainComp->removeKeyListener(this);
    };
}

void KeyboardController::onContainerParameterChanged ( ParameterBase* p){


}

String getPNameFromKey(const KeyPress & key){
    const char c = key.getTextCharacter();
    return String( "key ")+(char)c;
}
bool KeyboardController::keyPressed (const KeyPress& key, Component* /*originatingComponent*/)
{
    const char ch = key.getTextCharacter();
    const uint32 time = Time::getMillisecondCounter();
    // avoid repetition on holded keys
    for ( auto k:keysDown){if(k->key == key){ return false;}}
    keysDown.add(new KeyPressTime{key,time});

    if(logIncoming->boolValue()){LOG(juce::translate("keyPress : 123").replace("123" ,String(ch)));}

    const String paramName=getPNameFromKey(key);
    if (Controllable* c = userContainer.getControllableByName(paramName))
    {
        (( ParameterBase*)c)->setValue(key.isCurrentlyDown());
    }
    else if(autoAddParams){
        MessageManager::callAsync([this,key,paramName](){
            userContainer.
            addNewParameter<BoolParameter>(
                                           paramName,
                                           "Keyboard Parameter",
                                           key.isCurrentlyDown()
                                           );
        });
    }


    for (auto &l:jsKeyboardListener){
        l->processKeyPress(key);
    }
    return true;
}


bool KeyboardController::keyStateChanged (const bool /*isKeyDown*/, Component* /*originatingComponent*/)
{

    int i = 0;
    while(i < keysDown.size())
    {
        auto keyd = keysDown.getUnchecked(i);

        const bool isDown = keyd->key.isCurrentlyDown();
        auto key  =keyd->key;
        if (!isDown)
        {
            const String paramName=getPNameFromKey(key);
            if (Controllable* c = userContainer.getControllableByName(paramName))
            {
                (( ParameterBase*)c)->setValue(key.isCurrentlyDown());
            }
            keysDown.remove(i);
        }
        i++;
    }


    return true;
}

void KeyboardController::buildLocalEnv()
{
    DynamicObject obj;


    static const Identifier createKeyboardListener ("createKeyboardListener");
    obj.setMethod (createKeyboardListener, &KeyboardController::createJsKeyboardListener);
 
    setLocalNamespace (obj);

};

void KeyboardController::clearNamespace()
{
    JsEnvironment::clearNamespace();
    {
        const ScopedLock lk (jsKeyboardListener.getLock());
        jsKeyboardListener.clear();
    }
}

var KeyboardController::createJsKeyboardListener (const var::NativeFunctionArgs& a)
{
    if (a.numArguments < 1) { return var::undefined();}
    char key = a.arguments[0].toString().getLastCharacter();
    KeyboardController* originEnv = dynamic_cast<KeyboardController*> (a.thisObject.getDynamicObject());

    if (originEnv)
    {
        JsKeyboardListener* ob = new JsKeyboardListener (originEnv, key);
        originEnv->jsKeyboardListener.add (ob);
        return ob->object;
    }
    
    return var::undefined();
}

#endif
