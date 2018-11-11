/* Copyright © Organic Orchestra, 2017
 *
 * This file is part of LGML.  LGML is a software to manipulate sound in realtime
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation (version 3 of the License).
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */


#include "ControllerManagerUI.h"

#include "../../UI/Style.h"
#include "../ControllerFactory.h"
#include "../../UI/Inspector/Inspector.h"
#include "../../UI/ShapeShifter/ShapeShifterManager.h"
#include "../../Utils/FactoryUIHelpers.h"
#include "../../Controllable/ControllableUIHelpers.h"

//==============================================================================
ControllerManagerUI::ControllerManagerUI (ControllerManager* manager):
    manager (manager)
{

    manager->addControllerListener (this);

    for (auto& c : manager->controllers)
    {
        addControllerUI (c);
    }

}

ControllerManagerUI::~ControllerManagerUI()
{
    manager->removeControllerListener (this);
    clear();
}

void ControllerManagerUI::clear()
{
    while (controllersUI.size() > 0)
    {
        removeControllerUI (controllersUI[0]->controller);
    }
}


void ControllerManagerUI::controllerAdded (Controller* c)
{
    execOrDefer ([ = ]() { addControllerUI (c);});

}

void ControllerManagerUI::controllerRemoved (Controller* c)
{
    execOrDefer ([ = ]() { removeControllerUI (c);});

}

ControllerUI* ControllerManagerUI::addControllerUI (Controller* controller)
{
    if (getUIForController (controller) != nullptr)
    {
        DBG ("Controller already exists");
        return nullptr;
    }

    ControllerUI* cui = new ControllerUI(controller);
    controllersUI.add (cui);

    {
        MessageManagerLock ml;
        addAndMakeVisible (cui);
        resized();

    }
    cui->selectThis();

    notifyParentViewPort();
    return cui;
}

void ControllerManagerUI::removeControllerUI (Controller* controller)
{
    ControllerUI* cui = getUIForController (controller);

    if (cui == nullptr)
    {
        DBG ("Controller dont exist");
        return;
    }

    {
        MessageManagerLock ml;
        removeChildComponent (getIndexOfChildComponent (cui));
        controllersUI.removeObject (cui);

        resized();
    }

    notifyParentViewPort();

}
void ControllerManagerUI::notifyParentViewPort() {
    if (auto* p = findParentComponentOfClass<ControllerManagerUIViewport>())
        p->resized();
}

ControllerUI* ControllerManagerUI::getUIForController (Controller* controller)
{
    for (auto& cui : controllersUI)
    {
        if (cui->controller == controller) return cui;
    }

    return nullptr;
}

const int elemGap = 5;

void ControllerManagerUI::resized()
{
    Rectangle<int> r = getLocalBounds().reduced (4,0);

    for (auto& cui : controllersUI)
    {
        r.removeFromTop (elemGap);
        cui->setBounds (r.removeFromTop (cui->getHeight()));

    }
}

void ControllerManagerUI::paint (Graphics&)
{
    //ContourComponent::paint(g);
}

int ControllerManagerUI::getContentHeight() const
{
    int totalHeight = 0;
    for(auto & c : controllersUI){
        totalHeight+= c->getHeight() +  elemGap;
    }
    return totalHeight;

}

void ControllerManagerUI::mouseDown (const MouseEvent& event)
{
    if (event.eventComponent == this)
    {
        if (event.mods.isRightButtonDown())
        {

            ScopedPointer<PopupMenu> menu ( new PopupMenu());
            ScopedPointer<PopupMenu> addNodeMenu ( FactoryUIHelpers::getFactoryTypesMenu<ControllerFactory>());
            menu->addSubMenu ("Add Controller", *addNodeMenu);

            int result = menu->show();

            if (result > 0 )
            {
                if (auto c = FactoryUIHelpers::createFromMenuIdx<Controller> (result))
                {
                    manager->addController (c);
                }
                else
                    jassertfalse;
            }
        }

    }
}


void ControllerManagerUIViewport::buttonClicked (Button* b )
{
    if (b == &addControllerBt)
    {
        ScopedPointer<PopupMenu> addNodeMenu ( FactoryUIHelpers::getFactoryTypesMenu<ControllerFactory>());

        int result = addNodeMenu->show();

        if (result > 0 )
        {
            if (auto c = FactoryUIHelpers::createFromMenuIdx<Controller> (result))
            {
                controllerManagerUI->manager->addController (c);
            }
            else
                jassertfalse;
        }
    }

}
