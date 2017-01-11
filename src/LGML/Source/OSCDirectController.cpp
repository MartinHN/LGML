/*
 ==============================================================================

 OSCDirectController.cpp
 Created: 8 Mar 2016 10:27:37pm
 Author:  bkupe

 ==============================================================================
 */

#include "OSCDirectController.h"
#include "NodeManager.h"
#include "DebugHelpers.h"
#include "TimeManager.h"
#include "ParameterProxy.h"
#include "ControlManager.h"

OSCDirectController::OSCDirectController() :
OSCDirectController("OSC Direct Controller")
{

}

OSCDirectController::OSCDirectController(const String & name) :
OSCController(name)
{
    NodeManager::getInstance()->addControllableContainerListener(this);
    TimeManager::getInstance()->addControllableContainerListener(this);
}

OSCDirectController::~OSCDirectController()
{
    if(NodeManager * nm = NodeManager::getInstanceWithoutCreating())nm->removeControllableContainerListener(this);
    if(TimeManager * tm = TimeManager::getInstanceWithoutCreating()){tm->removeControllableContainerListener(this);}

}

Result OSCDirectController::processMessageInternal(const OSCMessage & msg)
{
    Result result = Result::ok();

    String addr = msg.getAddressPattern().toString();

    StringArray addrArray;
    addrArray.addTokens(addr,juce::StringRef("/"), juce::StringRef("\""));


    addrArray.remove(0);
    String controller = addrArray[0];


    if (controller == "node" || controller=="time" || controller=="control")
    {
        addrArray.remove(0);
        Controllable * c = nullptr;
        if(controller=="node")c=NodeManager::getInstance()->getControllableForAddress(addrArray);
        else if (controller=="time")c=TimeManager::getInstance()->getControllableForAddress(addrArray);
        else if (controller=="control")c=ControllerManager::getInstance()->getControllableForAddress(addrArray);

        if (c != nullptr)
        {
            if (!c->isControllableFeedbackOnly)
            {
                Controllable::Type targetType = c->type;
                if (targetType == Controllable::Type::PROXY) targetType = ((ParameterProxy *)c)->linkedParam->type;

                switch (targetType)
                {
                    case Controllable::Type::TRIGGER:
                        if (msg.size() == 0) ((Trigger *)c)->trigger();
                        else if (msg[0].isInt32() || msg[0].isFloat32())
                        {
                            float val = msg[0].isInt32() ? msg[0].getInt32() : msg[0].getFloat32();
                            if (val > 0) ((Trigger *)c)->trigger();
                        }
                        break;

                    case Controllable::Type::BOOL:
                        if (msg.size() > 0 && (msg[0].isInt32() || msg[0].isFloat32()))
                        {
                            float val = msg[0].isInt32() ? msg[0].getInt32() : msg[0].getFloat32();
                            ((Parameter *)c)->setValue(val > 0);
                        }
                        break;

                    case Controllable::Type::FLOAT:
                        if (msg.size() > 0 && (msg[0].isInt32() || msg[0].isFloat32()))
                        {
                            float value = msg[0].isInt32() ? msg[0].getInt32() : msg[0].getFloat32();
                            ((Parameter *)c)->setValue((float)value); //normalized or not ? can user decide ?
                        }
                        break;

                    case Controllable::Type::INT:
                        if (msg.size() > 0 && (msg[0].isInt32() || msg[0].isFloat32()))
                        {
                            int value = msg[0].isInt32() ? msg[0].getInt32() : (int)msg[0].getFloat32();
                            ((Parameter *)c)->setValue(value);
                        }
                        break;

                    case Controllable::Type::STRING:
                        if (msg.size() > 0){
                            // cast number to strings
                            if  (msg[0].isInt32() || msg[0].isFloat32()){
                                float value = msg[0].isInt32() ? msg[0].getInt32() : msg[0].getFloat32();
                                ((Parameter *)c)->setValue(String(value));
                            }
                            else if (msg[0].isString()){
                                ((Parameter *)c)->setValue(msg[0].getString());
                            }
                        }
                        break;



                    default:
                        result = Result::fail("Controllable type not handled");
                        break;

                }
            }
        }
        else
        {
            result = Result::fail("Controllable not found");

            DBG("No Controllable for address : " + addr);
        }
    }
    else
    {
        result = Result::fail("address other than /node, not handler for now");
    }

    return result;
}

void OSCDirectController::controllableAdded(Controllable *)
{
}

void OSCDirectController::controllableRemoved(Controllable *)
{

}



void OSCDirectController::controllableFeedbackUpdate(ControllableContainer * /*originContainer*/,Controllable * c)
{
#if JUCE_COMPILER_SUPPORTS_VARIADIC_TEMPLATES && JUCE_COMPILER_SUPPORTS_MOVE_SEMANTICS

    String cAddress = c->controlAddress;
    Controllable::Type targetType = c->type;
    if (targetType == Controllable::Type::PROXY) targetType = ((ParameterProxy *)c)->linkedParam->type;

    switch (targetType)
    {
        case Controllable::Type::TRIGGER:
            sendOSC(cAddress,1);
            break;

        case Controllable::Type::BOOL:
            sendOSC(cAddress,((Parameter *)c)->intValue());
            break;

        case Controllable::Type::FLOAT:
            sendOSC(cAddress, ((Parameter *)c)->floatValue());
            break;

        case Controllable::Type::INT:
            sendOSC(cAddress, ((Parameter *)c)->intValue());
            break;

        case Controllable::Type::STRING:
        case Controllable::Type::ENUM:
            sendOSC(cAddress, ((Parameter *)c)->stringValue());
            break;


        default:
            DBG("Type not supported " << targetType);
            jassertfalse;
            break;
    }






#else

    OSCMessage msg(c->controlAddress);
    switch (c->type)
    {
        case Controllable::Type::TRIGGER:
            msg.addInt32(1);
            break;

        case Controllable::Type::BOOL:
            msg.addInt32(((Parameter *)c)->intValue());
            break;

        case Controllable::Type::FLOAT:
            msg.addFloat32(((Parameter *)c)->floatValue());
            break;
            
        case Controllable::Type::INT:
            msg.addInt32(((Parameter *)c)->intValue());
            break;
            
        case Controllable::Type::STRING:
            msg.addString(((Parameter *)c)->stringValue());
            break;
            
        default:
            DBG("OSC : unknown Controllable");
            jassertfalse;
            break;
    }
    
  sendOSC(msg);
    
#endif
    
    
}

void OSCDirectController::controllableContainerAdded(ControllableContainer *)
{
}

void OSCDirectController::controllableContainerRemoved(ControllableContainer *)
{
}
