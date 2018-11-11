/*
 ==============================================================================

 Copyright © Organic Orchestra, 2017

 This file is part of LGML. LGML is a software to manipulate sound in realtime

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation (version 3 of the License).

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

 ==============================================================================
 */

#include "ControllerManager.h"
#include "../UI/LGMLDragger.h" // to enable default mapping mode on creation  

juce_ImplementSingleton (ControllerManager);


ControllerManager::ControllerManager() :
    ParameterContainer ("Controllers")
{
    nameParam->isEditable = false;
    
}

ControllerManager::~ControllerManager()
{
    clear();
}


Controller* ControllerManager::addController (Controller* c)
{

    c->nameParam->setValue (getUniqueNameInContainer (c->nameParam->stringValue()));

    controllers.add (c);

    addChildControllableContainer (c);
    listeners.call (&ControllerManager::Listener::controllerAdded, c);
    c->setMappingMode(LGMLDragger::getInstance()->isMappingActive);
    return c;
}

void ControllerManager::removeController (Controller* c)
{

    removeChildControllableContainer (c);
    listeners.call (&ControllerManager::Listener::controllerRemoved, c);
    controllers.removeObject (c);
}

void ControllerManager::clear()
{

    while (controllers.size())
    {
        controllers[0]->remove();
    }
}



ParameterContainer*   ControllerManager::addContainerFromObject (const String& /*name*/, DynamicObject*   ob)
{
    return addController (ControllerFactory::createBaseFromObject (String::empty, ob));

}



