/*
 ==============================================================================

 Controller.cpp
 Created: 2 Mar 2016 8:49:50pm
 Author:  bkupe

 ==============================================================================
 */

#include "Controller.h"
#include "ControllerFactory.h"
#include "ControllerUI.h"
#include "ControlVariable.h"
#include "DebugHelpers.h"


const Identifier Controller::controllerTypeIdentifier("controllerType");
const Identifier Controller::variableNameIdentifier("name");
const Identifier Controller::variableMinIdentifier("min");
const Identifier Controller::variableMaxIdentifier("max");

Controller::Controller(const String &_name) :
ControllableContainer(_name),
variableChangeNotifier(1000)
{
  nameParam = addStringParameter("Name", "Set the name of the controller.", _name);
  enabledParam = addBoolParameter("Enabled","Set whether the controller is enabled or disabled", true);

  activityTrigger = addTrigger("activity", "Activity indicator");

  controllerTypeEnum = 0; //init
}


Controller::~Controller()
{
  //DBG("Remove Controller");
}

var Controller::getJSONData()
{
  var data = ControllableContainer::getJSONData();
  data.getDynamicObject()->setProperty(controllerTypeIdentifier, ControllerFactory::controllerToString(this));

  var vDataArray;
  for (auto &v : variables)
  {
	  if (v->includeInSave)
	  {
		  var vData(new DynamicObject());
		  vData.getDynamicObject()->setProperty(variableNameIdentifier, v->parameter->niceName);
		  vData.getDynamicObject()->setProperty(variableMinIdentifier, v->parameter->minimumValue);
		  vData.getDynamicObject()->setProperty(variableMaxIdentifier, v->parameter->maximumValue);
		  vDataArray.append(vData);
	  }
  }

  data.getDynamicObject()->setProperty("variables", vDataArray);


  return data;
}

void Controller::loadJSONDataInternal(var data)
{
  Array<var>* vDataArray = data.getDynamicObject()->getProperty("variables").getArray();

  if (vDataArray != nullptr)
  {
    for (auto &v : *vDataArray)
    {
      Parameter * p = new FloatParameter("newVar", "variable", 0);
      p->replaceSlashesInShortName = false;
      p->setNiceName(v.getDynamicObject()->getProperty(variableNameIdentifier));

      if(v.getDynamicObject()->hasProperty(variableMinIdentifier)){
        p->minimumValue = v.getDynamicObject()->getProperty(variableMinIdentifier);
      }

      if(v.getDynamicObject()->hasProperty(variableMaxIdentifier)){
        p->maximumValue = v.getDynamicObject()->getProperty(variableMaxIdentifier);
      }
      addVariable(p);
    }
  }
}


String Controller::getUniqueVariableNameFor(const String & baseName, int index)
{
  String tName = baseName + " " + String(index);
  for (auto & v : variables)
  {
    if (v->parameter->niceName == tName)
    {
      return getUniqueVariableNameFor(baseName, index + 1);
    }
  }

  return tName;
}

ControllerUI * Controller::createUI()
{
  return new ControllerUI(this);
}

ControlVariable * Controller::addVariable(Parameter * p)
{
  ControlVariable * v = new ControlVariable(this, p);
  p->replaceSlashesInShortName = false;
  variables.add(v);
  v->addControlVariableListener(this);
  internalVariableAdded(v);
  controllerListeners.call(&ControllerListener::variableAdded, this, v);
  variableChangeNotifier.addMessage(new Controller::VariableChangeMessage(this,v,true));

  return v;
}

void Controller::removeVariable(ControlVariable * v)
{
  v->removeControlVariableListener(this);
  internalVariableRemoved(v);
  controllerListeners.call(&ControllerListener::variableRemoved, this, v);
  variableChangeNotifier.addMessage(new Controller::VariableChangeMessage(this,v,false));
  variables.removeObject(v);
}

ControlVariable * Controller::getVariableForAddress(const String & address)
{
  for (auto &v : variables)
  {
    if (v->parameter->controlAddress == address) return v;
  }

  return nullptr;
}

ControlVariable * Controller::getVariableForName(const String & name)
{
  for (auto &v : variables)
  {
    if (v->parameter->niceName == name) return v;
  }
  return nullptr;
}

void Controller::remove()
{
  controllerListeners.call(&ControllerListener::askForRemoveController, this);
}

void Controller::onContainerParameterChanged(Parameter * p)
{
  if (p == nameParam)
  {
    setNiceName(nameParam->stringValue());
  }
  else if (p == enabledParam)
  {
    if(JsEnvironment * jsEnv = dynamic_cast<JsEnvironment*>(this)){
      jsEnv->setTimerState(jsEnv->onUpdateTimer, enabledParam->boolValue());
    }

    // DBG("set Controller Enabled " + String(enabledParam->boolValue()));
  }
}

void Controller::onContainerTriggerTriggered(Trigger *){}

void Controller::askForRemoveVariable(ControlVariable * variable)
{
  removeVariable(variable);
}
