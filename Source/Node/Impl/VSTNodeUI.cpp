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

#if !ENGINE_HEADLESS

#include "VSTNodeUI.h"
#include "../../Controllable/Parameter/UI/TriggerBlinkUI.h"
#include "../../Audio/VSTManager.h"
#include "../UI/ConnectableNodeUI.h"
#include "../../Controllable/Parameter/UI/ParameterUIFactory.h"
#include "../../UI/PluginWindow.h"


VSTNodeContentUI::VSTNodeContentUI():
    VSTListShowButton ("VSTs"),
    showPluginWindowButton ("showWindow"),
    isDirty (false)
{


}
VSTNodeContentUI::~VSTNodeContentUI()
{
    closePluginWindow();
    vstNode->removeVSTNodeListener (this);
    vstNode->removeControllableContainerListener (this);

}


void  VSTNodeContentUI::createPluginWindow()
{
    if (PluginWindow* const w = PluginWindow::getWindowFor (vstNode))
        w->toFront (true);
}

void VSTNodeContentUI::closePluginWindow()
{
    PluginWindow::closeCurrentlyOpenWindowsFor (vstNode);
}

void VSTNodeContentUI::init()
{

    vstNode = (VSTNode*)node.get();
    addAndMakeVisible (midiDeviceChooser);

    VSTListShowButton.addListener (this);
    showPluginWindowButton.addListener (this);

    addAndMakeVisible (showPluginWindowButton);
    addAndMakeVisible (VSTListShowButton);

    activityBlink = ParameterUIFactory::createDefaultUI (vstNode->midiActivityTrigger);
    activityBlink->showLabel = false;
    addAndMakeVisible (activityBlink);

    midiDeviceChooser = ParameterUIFactory::createDefaultUI(vstNode->midiChooser.getDeviceInEnumParameter());
    addAndMakeVisible(midiDeviceChooser);
    jassert(midiDeviceChooser);
    updateVSTParameters();
    setDefaultSize (250, 100);


    vstNode->addVSTNodeListener (this);
    vstNode->addControllableContainerListener (this);

    //DBG("Set Node and ui -> " << vstNode->midiPortNameParam->stringValue());

}

void VSTNodeContentUI::updateVSTParameters()
{
    for (auto& p : paramSliders)
    {
        removeChildComponent (p);
    }

    paramSliders.clear();

    int maxParameter = 20;
    int pCount = 0;

    for (auto& p : vstNode->VSTParameters)
    {
        FloatSliderUI* slider = new FloatSliderUI (p);
        paramSliders.add (slider);
        addAndMakeVisible (slider);
        pCount++;

        if (pCount > maxParameter)
        {
            break;
        }
    }

    resized();
}

void VSTNodeContentUI::controllableAdded (ControllableContainer*, Controllable*) {};
void VSTNodeContentUI::controllableRemoved (ControllableContainer*, Controllable* c)
{
    for (auto& p : paramSliders)
    {
        if (p->parameter == c)removeChildComponent (p);
    }

    if (isDirty) return;

    postCommandMessage (0);
    isDirty = true;
}
void VSTNodeContentUI::controllableContainerAdded (ControllableContainer*, ControllableContainer*) {};
void VSTNodeContentUI::controllableContainerRemoved (ControllableContainer*, ControllableContainer*) {};




//Listener From VSTNode
void VSTNodeContentUI::newVSTSelected()
{
    if (isDirty) return;

    postCommandMessage (0);
    isDirty = true;
}

void VSTNodeContentUI::handleCommandMessage (int /*cId*/)
{
    updateVSTParameters();
    isDirty = false;
}


void VSTNodeContentUI::resized()
{
    Rectangle<int> area = getLocalBounds().reduced (2);
    Rectangle<int> midiR = area.removeFromTop (25);
    activityBlink->setBounds (midiR.removeFromRight (midiR.getHeight()).reduced (2));
    midiDeviceChooser->setBounds (midiR);

    area.removeFromTop (2);
    Rectangle<int> headerArea = area.removeFromTop (25);
    VSTListShowButton.setBounds (headerArea.removeFromLeft (headerArea.getWidth() / 2));
    showPluginWindowButton.setBounds (headerArea);
    layoutSliderParameters (area.reduced (2));

}

void VSTNodeContentUI::layoutSliderParameters (Rectangle<int> pArea)
{
    if (paramSliders.size() == 0) return;

    int maxLines = 4;

    int numLines = jmin (maxLines, paramSliders.size());
    int numCols = (paramSliders.size() - 1) / maxLines + 1;

    int w = pArea.getWidth() / numCols;
    int h = pArea.getHeight() / numLines;
    int idx = 0;

    for (int i = 0 ; i < numCols ; i ++)
    {
        Rectangle<int> col = pArea.removeFromLeft (w);

        for (int j = 0 ; j < numLines ; j++)
        {
            paramSliders.getUnchecked (idx)->setBounds (col.removeFromTop (h).reduced (1));
            idx++;

            if (idx >= paramSliders.size())
            {
                break;
            }
        }

        if (idx >= paramSliders.size())
        {
            break;
        }
    }
}


void VSTNodeContentUI::vstSelected (int modalResult, Component*   originComp)
{
    int index = VSTManager::getInstance()->knownPluginList.getIndexChosenByMenu (modalResult);

    if (index >= 0 )
    {
        VSTNodeContentUI* originVSTNodeUI =  dynamic_cast<VSTNodeContentUI*> (originComp);

        if (originVSTNodeUI)
        {
            originVSTNodeUI->vstNode->identifierString->setValue (VSTManager::getInstance()->knownPluginList.getType (index)->createIdentifierString());
            //            originVSTNodeUI->owner->generatePluginFromDescription(VSTManager::getInstance()->knownPluginList.getType (index));
        }
    }
}



void VSTNodeContentUI::buttonClicked (Button* button)
{
    if (button == &VSTListShowButton)
    {
        PopupMenu  VSTList;
        VSTManager::getInstance()->knownPluginList.addToMenu (VSTList, KnownPluginList::SortMethod::sortByCategory);
        closePluginWindow();
        VSTList.showAt (&VSTListShowButton, 0, 0, 0, 0, ModalCallbackFunction::forComponent (&VSTNodeContentUI::vstSelected, (Component*)this));

    }

    if (button == &showPluginWindowButton)
    {
        createPluginWindow();
    }
}



#endif
