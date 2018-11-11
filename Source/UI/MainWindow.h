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

#pragma once
#include "../JuceHeaderUI.h"
#include "Style.h" // for USE_GL


#include "LGMLDragger.h"
class LatestVersionChecker;

class MainContentComponent;
class Engine;

class MainWindow    : public DocumentWindow, private Timer,
private ChangeListener // for undoactions
{
public:
    MainWindow (String name, Engine* e) ;
    ~MainWindow () ;
    void focusGained (FocusChangeType cause)override;


    void closeButtonPressed() override;

    void timerCallback() override;

    void changeListenerCallback (ChangeBroadcaster* source) override;

    /* Note: Be careful if you override any DocumentWindow methods - the base
     class uses a lot of them, so by overriding you might break its functionality.
     It's best to do all your work in your content component instead, but if
     you really have to override any DocumentWindow methods, make sure your
     subclass also calls the superclass's method.
     */
    MainContentComponent* mainComponent;

#if USE_GL
    OpenGLContext openGLContext;
#endif


private:
    ScopedPointer<LatestVersionChecker>  latestVChecker ;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)

};
