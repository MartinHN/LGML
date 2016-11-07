/*
 ==============================================================================

 MainComponentMenuBar.cpp
 Created: 25 Mar 2016 6:02:02pm
 Author:  Martin Hermant

 ==============================================================================
 */


#include "MainComponent.h"
#include "Rule.h"
#include "FastMap.h"
#include "Engine.h"
#include "Inspector.h"
#include "NodeContainer.h"

namespace CommandIDs
{
  static const int open                   = 0x30000;
  static const int save                   = 0x30001;
  static const int saveAs                 = 0x30002;
  static const int newFile                = 0x30003;
  static const int openLastDocument       = 0x30004;
  static const int playPause              = 0x30010;
  static const int copySelection			= 0x30020;
  static const int cutSelection			= 0x30021;
  static const int pasteSelection			= 0x30022;
  static const int showPluginListEditor   = 0x30100;
  static const int showAudioSettings      = 0x30200;
  static const int aboutBox               = 0x30300;
  static const int allWindowsForward      = 0x30400;
  static const int toggleDoublePrecision  = 0x30500;
  static const int stimulateCPU           = 0x30600;

  // range ids
  static const int lastFileStartID        =100; // 100 to 200 max

}




void MainContentComponent::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)  {
  const String category ("General");

  switch (commandID)
  {
    case CommandIDs::newFile:
      result.setInfo ("New", "Creates a new filter graph file", category, 0);
      result.defaultKeypresses.add(KeyPress('n', ModifierKeys::commandModifier, 0));
      break;

    case CommandIDs::open:
      result.setInfo ("Open...", "Opens a filter graph file", category, 0);
      result.defaultKeypresses.add (KeyPress ('o', ModifierKeys::commandModifier, 0));
      break;

    case CommandIDs::openLastDocument:
      result.setInfo("Open Last Document", "Opens a filter graph file", category, 0);
      result.defaultKeypresses.add(KeyPress('o', ModifierKeys::shiftModifier | ModifierKeys::commandModifier, 0));
      break;

    case CommandIDs::save:
      result.setInfo ("Save", "Saves the current graph to a file", category, 0);
      result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::commandModifier, 0));
      break;

    case CommandIDs::saveAs:
      result.setInfo ("Save As...",
                      "Saves a copy of the current graph to a file",
                      category, 0);
      result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::shiftModifier | ModifierKeys::commandModifier, 0));
      break;

    case CommandIDs::showPluginListEditor:
      result.setInfo ("Edit the list of available plug-Ins...", String::empty, category, 0);
      result.addDefaultKeypress ('p', ModifierKeys::commandModifier);
      break;

    case CommandIDs::showAudioSettings:
      result.setInfo ("Change the audio device settings", String::empty, category, 0);
      result.addDefaultKeypress ('a', ModifierKeys::commandModifier);
      break;

    case CommandIDs::toggleDoublePrecision:
      result.setInfo ("toggles doublePrecision", String::empty, category, 0);
      //updatePrecisionMenuItem (result);
      break;

    case CommandIDs::aboutBox:
      result.setInfo ("About...", String::empty, category, 0);
      break;

    case CommandIDs::allWindowsForward:
      result.setInfo ("All Windows Forward", "Bring all plug-in windows forward", category, 0);
      result.addDefaultKeypress ('w', ModifierKeys::commandModifier);
      break;
    case CommandIDs::playPause:
      result.setInfo ("Play/pause", "Play or pause LGML", category, 0);
      result.addDefaultKeypress (' ',ModifierKeys::noModifiers);
      break;

    case CommandIDs::copySelection:
      result.setInfo("Copy selection", "Copy current selection", category, 0);
      result.addDefaultKeypress('c', ModifierKeys::commandModifier);
      break;

    case CommandIDs::cutSelection:
      result.setInfo("Cut selection", "Cut current selection", category, 0);
      result.addDefaultKeypress('x', ModifierKeys::commandModifier);
      break;

    case CommandIDs::pasteSelection:
      result.setInfo("Paste selection", "Paste previously copied item into selected object if possible", category, 0);
      result.addDefaultKeypress('v', ModifierKeys::commandModifier);
      break;

    case CommandIDs::stimulateCPU:
      updateStimulateAudioItem(result);
      break;


    default:
      break;
  }
}


void MainContentComponent::getAllCommands (Array<CommandID>& commands) {
  // this returns the set of all commands that this target can perform..
  const CommandID ids[] = {
    CommandIDs::newFile,
    CommandIDs::open,
    CommandIDs::openLastDocument,
    CommandIDs::save,
    CommandIDs::saveAs,
    CommandIDs::showPluginListEditor,
    CommandIDs::showAudioSettings,
    CommandIDs::toggleDoublePrecision,
    CommandIDs::aboutBox,
    CommandIDs::allWindowsForward,
    CommandIDs::playPause,
    CommandIDs::copySelection,
    CommandIDs::cutSelection,
    CommandIDs::pasteSelection,
    CommandIDs::stimulateCPU
  };

  commands.addArray (ids, numElementsInArray (ids));
}


PopupMenu MainContentComponent::getMenuForIndex (int /*topLevelMenuIndex*/, const String& menuName) {
  PopupMenu menu;

  if(menuName == "File")
  {
    // "File" menu
    menu.addCommandItem (&getCommandManager(), CommandIDs::newFile);
    menu.addCommandItem (&getCommandManager(), CommandIDs::open);
    menu.addCommandItem(&getCommandManager(), CommandIDs::openLastDocument);

    RecentlyOpenedFilesList recentFiles;
    recentFiles.restoreFromString (getAppProperties().getUserSettings()
                                   ->getValue (lastFileListKey));

    PopupMenu recentFilesMenu;
    recentFiles.createPopupMenuItems (recentFilesMenu, CommandIDs::lastFileStartID, true, true);
    menu.addSubMenu ("Open recent file", recentFilesMenu);

    menu.addCommandItem (&getCommandManager(), CommandIDs::save);
    menu.addCommandItem (&getCommandManager(), CommandIDs::saveAs);
    menu.addSeparator();
    menu.addCommandItem (&getCommandManager(), StandardApplicationCommandIDs::quit);

  } else if (menuName == "Edit")
  {
    menu.addCommandItem(&getCommandManager(), CommandIDs::playPause);
    menu.addSeparator();
    menu.addCommandItem(&getCommandManager(), CommandIDs::copySelection);
    menu.addCommandItem(&getCommandManager(), CommandIDs::cutSelection);
    menu.addCommandItem(&getCommandManager(), CommandIDs::pasteSelection);

  } else if (menuName == "Plugins")
  {
    // "Plugins" menu
    PopupMenu pluginsMenu;
    //            addPluginsToMenu (pluginsMenu);
    menu.addSubMenu ("Create plugin", pluginsMenu);
    menu.addSeparator();
    menu.addItem (250, "Delete all plugins");
  } else if (menuName == "Options")
  {
    // "Options" menu

    menu.addCommandItem (&getCommandManager(), CommandIDs::showPluginListEditor);
    menu.addCommandItem (&getCommandManager(), CommandIDs::showAudioSettings);
    menu.addCommandItem(&getCommandManager(), CommandIDs::stimulateCPU);
    menu.addCommandItem (&getCommandManager(), CommandIDs::toggleDoublePrecision);

    menu.addSeparator();
    menu.addCommandItem (&getCommandManager(), CommandIDs::aboutBox);
  } else if (menuName == "Windows")
  {
    menu.addCommandItem (&getCommandManager(), CommandIDs::allWindowsForward);
  }
  else if (menuName == "Panels")
  {
    return ShapeShifterManager::getInstance()->getPanelsMenu();
  }

  return menu;
}

bool MainContentComponent::perform(const InvocationInfo& info) {

  switch (info.commandID)
  {


    case CommandIDs::newFile:
    {
      int result = AlertWindow::showYesNoCancelBox(AlertWindow::QuestionIcon, "Save document", "Do you want to save the document before creating a new one ?");
      if (result != 0)
      {
        if (result == 1) engine->save(true, true);
        engine->createNewGraph();

      }
    }
      break;

    case CommandIDs::open:
    {
      int result = AlertWindow::showYesNoCancelBox(AlertWindow::QuestionIcon, "Save document", "Do you want to save the document before opening a new one ?");
      if (result != 0)
      {
        if (result == 1) engine->save(true, true);
        engine->loadFromUserSpecifiedFile(true);
      }
    }
      break;

    case CommandIDs::openLastDocument:
    {
      // TODO implement the JUCE version calling change every time something is made (maybe todo with undomanager)
      //			int result = engine->saveIfNeededAndUserAgrees();
      int result = AlertWindow::showYesNoCancelBox(AlertWindow::QuestionIcon, "Save document", "Do you want to save the document before opening the last one ?");
      if (result != 0)
      {
        if (result == 1) engine->save(true, true);
        engine->loadFrom(engine->getLastDocumentOpened(),true);
      }
    }
      break;

    case CommandIDs::save:
      engine->save (true, true);
      break;

    case CommandIDs::saveAs:
      engine->saveAs (File::nonexistent, true, true, true);
      break;
    case CommandIDs::stimulateCPU:
      engine->stimulateAudio(engine->stimulator==nullptr);
    {
      ApplicationCommandInfo cmdInfo (info.commandID);
      updateStimulateAudioItem (cmdInfo);
      menuItemsChanged();
    }
      break;

    case CommandIDs::toggleDoublePrecision:
      DBG("double precision not supported yet...");
      break;


    case CommandIDs::showPluginListEditor:
      VSTManager::getInstance()->createPluginListWindowIfNeeded();
      break;

    case CommandIDs::showAudioSettings:
      showAudioSettings();
      break;


    case CommandIDs::allWindowsForward:
    {
      Desktop& desktop = Desktop::getInstance();

      for (int i = 0; i < desktop.getNumComponents(); ++i)
        // doesn't work on windows
        if (desktop.getComponent(i)->getParentComponent() != nullptr)
        {
          desktop.getComponent(i)->toBehind(this);
        }
      break;
    }
    case CommandIDs::playPause:
      TimeManager::getInstance()->togglePlay();
      break;

    case CommandIDs::copySelection:
    case CommandIDs::cutSelection:
    {
      InspectableComponent * ic = Inspector::getInstance()->currentComponent;
      if (ic != nullptr)
      {
        ControllableContainer * cc = Inspector::getInstance()->currentComponent->relatedControllableContainer;
        if (cc != nullptr)
        {

          var data(new DynamicObject());
          data.getDynamicObject()->setProperty("type", ic->inspectableType);
          data.getDynamicObject()->setProperty("data", cc->getJSONData());


          if (info.commandID == CommandIDs::cutSelection)
          {
            if (ic->inspectableType == "node") ((ConnectableNode *)cc)->remove();
            else if (ic->inspectableType == "controller") ((Controller *)cc)->remove();
            else if (ic->inspectableType == "rule") ((Rule *)cc)->remove();
            else if (ic->inspectableType == "fastMap") ((FastMap *)cc)->remove();
          }

          SystemClipboard::copyTextToClipboard(JSON::toString(data));
        }
      }
    }
      break;

    case CommandIDs::pasteSelection:
    {
      String clipboard = SystemClipboard::getTextFromClipboard();

      var data = JSON::parse(clipboard);
      if (data != var::null)
      {
        DynamicObject * d = data.getDynamicObject();
        if (d != nullptr && d->hasProperty("type"))
        {

          String type = d->getProperty("type");
          if (Inspector::getInstance()->currentComponent != nullptr)
          {
            if (type == "node" && Inspector::getInstance()->currentComponent->inspectableType == "node")
            {
              ConnectableNode * cn = dynamic_cast<ConnectableNode *>(Inspector::getInstance()->currentComponent->relatedControllableContainer);
              NodeContainer * container = (cn->type == ContainerType) ? dynamic_cast<NodeContainer *>(cn) : cn->parentNodeContainer;
              if (cn != nullptr)
              {
                ConnectableNode * n = container->addNodeFromJSONData(d->getProperty("data"));
                // ensure to have different uuid than the one from JSON
                if(n){n->uid = Uuid();
                n->xPosition->setValue(n->xPosition->intValue() + 100, true);
                n->yPosition->setValue(n->xPosition->intValue() + 50);
                }
              }
            }
          }
        }
      }
    }
      break;

    default:
      DBG("no command found");
      return false;
  }

  return true;
}

void MainContentComponent::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{

  String menuName = getMenuBarNames()[topLevelMenuIndex];
  if (menuName == "Panels")
  {
    ShapeShifterManager::getInstance()->handleMenuPanelCommand(menuItemID);
  }
  else if (isPositiveAndBelow(menuItemID-CommandIDs::lastFileStartID, 100)){
    RecentlyOpenedFilesList recentFiles;
    recentFiles.restoreFromString (getAppProperties().getUserSettings()
                                   ->getValue (lastFileListKey));
    engine->loadFrom(recentFiles.getFile(menuItemID-CommandIDs::lastFileStartID),true);
  }
}


StringArray MainContentComponent::getMenuBarNames() {
  const char* const names[] = { "File", "Edit", "Plugins", "Options", "Windows","Panels", nullptr };
  return StringArray (names);
}

void MainContentComponent::updateStimulateAudioItem (ApplicationCommandInfo& info)
{
  info.setInfo ("stimulate CPU", "Simulate heavy CPU Usage", "General", 0);
  info.setTicked (engine->stimulator!=nullptr);
}
