/*
  ==============================================================================

    JsNode.h
    Created: 28 May 2016 2:00:46pm
    Author:  Martin Hermant

  ==============================================================================
*/

#ifndef JSNODE_H_INCLUDED
#define JSNODE_H_INCLUDED
#include "JsEnvironment.h"
#include "NodeBase.h"
#include "JsHelpers.h"
#include "DebugHelpers.h"



class JsNode : public NodeBase,public JsEnvironment{
    public :
    JsNode():JsEnvironment("JsNode"){
        scriptPath = NodeBase::addStringParameter("ScriptPath","path for js script","");
    };

    StringParameter* scriptPath;

    void clearNamespace() override{
        JsEnvironment::clearNamespace();
        for(auto & p:jsParameters){
            removeControllable(p);
        }


    }

    void buildLocalEnv() override{
        static  Identifier addIntParameterIdentifier ("addIntParameter");
        static Identifier addFloatParameterIdentifier("addFloatParameter");
        static Identifier addStringParameterIdentifier("addStringParameter");

        DynamicObject d;
        d.setProperty(jsPtrIdentifier, (int64)this);
        d.setMethod(addIntParameterIdentifier, JsNode::addIntParameter);
        d.setMethod(addFloatParameterIdentifier, JsNode::addFloatParameter);
        d.setMethod(addStringParameterIdentifier, JsNode::addStringParameter);


        setLocalNamespace(d);

    }

    void newJsFileLoaded()override{
        scriptPath->setValue(currentFile.getFullPathName());
    }


    static var addIntParameter(const var::NativeFunctionArgs & a){

        JsNode * jsNode = getObjectPtrFromJS<JsNode>(a);
        if(a.numArguments<3){
            LOG("wrong number of arg for addIntParameter");
            return var::undefined();
        };
        jsNode->jsParameters.add(jsNode->ControllableContainer::addIntParameter(a.arguments[0], a.arguments[1], a.arguments[2], a.arguments[3], a.arguments[4]));

        return var::undefined();
    };

    void onContainerParameterChanged(Parameter * p) override{
        if(p==scriptPath){
            loadFile(scriptPath->stringValue());
        }
    }

    static var addFloatParameter(const var::NativeFunctionArgs & a){

        JsNode * jsNode = getObjectPtrFromJS<JsNode>(a);
        if(a.numArguments<5){
            LOG("wrong number of arg for addFloatParameter");
            return var::undefined();
        };
        
        jsNode->jsParameters.add(jsNode->ControllableContainer::addFloatParameter(a.arguments[0], a.arguments[1], a.arguments[2], a.arguments[3], a.arguments[4]));

        return var::undefined();
    };
    static var addStringParameter(const var::NativeFunctionArgs & a){

        JsNode * jsNode = getObjectPtrFromJS<JsNode>(a);
        if(a.numArguments<3){
            LOG("wrong number of arg for addStringParameter");
            return var::undefined();
        };
        jsNode->jsParameters.add(jsNode->ControllableContainer::addStringParameter(a.arguments[0], a.arguments[1], a.arguments[2]));

        return var::undefined();
    };

    virtual ConnectableNodeUI * createUI() override;

    Array<Parameter * > jsParameters;


};



#endif  // JSNODE_H_INCLUDED
