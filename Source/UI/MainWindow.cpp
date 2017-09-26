/*
 ==============================================================================

 MainWindow.cpp
 Created: 11 Sep 2017 1:48:29pm
 Author:  Martin Hermant

 ==============================================================================
 */

#include "MainWindow.h"
#include "MainComponent.h"
#include "../Engine.h"

#include "JuceHeader.h" // for ProjectInfo


MainContentComponent* createMainContentComponent(Engine* e);



MainWindow::MainWindow (String name,Engine * e)  : DocumentWindow (name,
                                                                   Colours::lightgrey,
                                                                   DocumentWindow::allButtons)
{

  
  mainComponent = createMainContentComponent(e);
  setContentOwned(mainComponent, false);

#ifdef JUCE_LINUX
  // loads of bug on ubuntu \
  - no display            \
  -wrong rebuilding of windows position/size \
  - double clicks sent to titlebar

  setUsingNativeTitleBar(false);

#else
  setUsingNativeTitleBar(true);
#endif
  setResizable(true, false);

#if ! JUCE_MAC
  setMenuBar(mainComponent);
#endif

#if JUCE_OPENGL
  openGLContext.setContinuousRepainting(false);
  openGLContext.attachTo(*getTopLevelComponent());
#endif


  // need to stay after ll init function for linux
#ifndef JUCE_LINUX
  String winSetting ( "fs 0 0 800 600");
#else
  // weird behaviour of fullscreen in ubuntu (can't get out of fs mode and wrong windows coords)
  String winSetting ( "0 0 800 600");
#endif


  if(auto prop = getAppProperties().getCommonSettings(true)){
    winSetting = prop->getValue("winSettings",winSetting);
  }
  ResizableWindow::restoreWindowStateFromString(winSetting);



  setVisible (true);



  ShapeShifterManager::getInstance()->loadLastSessionLayoutFile();
  LGMLDragger::getInstance()->setMainComponent(mainComponent,&mainComponent->tooltipWindow);


  startTimer(1000);

}
void MainWindow::focusGained(FocusChangeType cause){
  //mainComponent->grabKeyboardFocus();
}


void MainWindow::closeButtonPressed()
{
  // This is called when the user tries to close this window. Here, we'll just
  // ask the app to quit when this happens, but you can change this to do
  // whatever you need.

  //@martin added but commented for testing (relou behavior)
  int result = AlertWindow::showYesNoCancelBox(AlertWindow::QuestionIcon, "Save document", "Do you want to save the document before quitting ?");

  if (result == 0)  return; //prevent exit

  if (result == 1)
  {
    juce::FileBasedDocument::SaveResult sr = getEngine()->save(true, true);
    switch (sr)
    {
      case juce::FileBasedDocument::SaveResult::userCancelledSave:
      case juce::FileBasedDocument::SaveResult::failedToWriteToFile:
        return;

      case FileBasedDocument::SaveResult::savedOk:
        break;
    }
  }


  var boundsVar = var(new DynamicObject());
  Rectangle<int> r = getScreenBounds();

  getAppProperties().getCommonSettings(true)->setValue("winSettings",getWindowStateAsString());
  getAppProperties().getCommonSettings(true)->saveIfNeeded();


#if JUCE_OPENGL
  openGLContext.detach();
#endif
  JUCEApplication::getInstance()->systemRequestedQuit();

}

void MainWindow::timerCallback()
{
  setName(getEngine()->getDocumentTitle() +" : LGML "
          + String(ProjectInfo::versionString)+String(" (CPU : ")+
          String((int)(getAudioDeviceManager().getCpuUsage() * 100))+String("%)"));
}


