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


#ifndef SHAPESHIFTERMANAGER_H_INCLUDED
#define SHAPESHIFTERMANAGER_H_INCLUDED

#include "ShapeShifterContainer.h"
#include "ShapeShifterWindow.h"
#include "ShapeShifterFactory.h"



class ShapeShifterManager :
    public ShapeShifterPanel::Listener
{
public:
    juce_DeclareSingleton (ShapeShifterManager, true);
    ShapeShifterManager();
    virtual ~ShapeShifterManager();

    ShapeShifterContainer mainShifterContainer;

    File lastFile;
    File defaultFolder;

    OwnedArray<ShapeShifterPanel> openedPanels;
    OwnedArray<ShapeShifterWindow> openedWindows;

    ShapeShifterPanel* currentCandidatePanel;
    void setCurrentCandidatePanel (ShapeShifterPanel*);

    PanelName getPanelNameForContentName (const String& name);
    String getContentNameForPanelName (PanelName panelName);

    ShapeShifterPanel* getPanelForContent (ShapeShifterContent* content);
    ShapeShifterPanel* getPanelForContentName (const String& name);

    ShapeShifterPanel* createPanel (ShapeShifterContent* content, ShapeShifterPanelTab* sourceTab = nullptr);
    void removePanel (ShapeShifterPanel* panel);

    ShapeShifterWindow* showPanelWindow (ShapeShifterPanel* _panel, Rectangle<int> bounds);
    ShapeShifterWindow* showPanelWindowForContent (PanelName panelName);
    void showContent (String contentName);


    void closePanelWindow (ShapeShifterWindow* window, bool doRemovePanel);

    ShapeShifterContent* getContentForName (PanelName contentName);

    ShapeShifterPanel* checkCandidateTargetForPanel (ShapeShifterPanel* panel);
    bool checkDropOnCandidateTarget (WeakReference<ShapeShifterPanel> panel);

    ShapeShifterWindow* getWindowForPanel (ShapeShifterPanel* panel);

    void loadLayout (var layoutObject);
    var getCurrentLayout();
    void loadLayoutFromFile (int fileIndexInLayoutFolder = -1);
    void loadLayoutFromFile (const File& fromFile);
    void loadLastSessionLayoutFile();
    void loadDefaultLayoutFile();
    void saveCurrentLayout();
    void saveCurrentLayoutToFile (const File& toFile);
    Array<File> getLayoutFiles();

    void clearAllPanelsAndWindows();

    const int baseMenuCommandID = 0x31000;
    const int baseSpecialMenuCommandID = 0x32000;
    PopupMenu getPanelsMenu();

    void handleMenuPanelCommand (int commandID);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ShapeShifterManager)
};

#endif  // SHAPESHIFTERMANAGER_H_INCLUDED
