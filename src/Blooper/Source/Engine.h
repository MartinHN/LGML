/*
  ==============================================================================

    Engine.h
    Created: 2 Apr 2016 11:03:21am
    Author:  Martin Hermant

  ==============================================================================
*/

#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include "JuceHeader.h"


#include "ControlManager.h"
#include "TimeManager.h"
#include "NodeManager.h"
#include "VSTManager.h"
#include "../../../../../../../Dev/JUCE/modules/juce_gui_extra/documents/juce_FileBasedDocument.h"
#include "../../../../../../../Dev/JUCE/modules/juce_audio_utils/players/juce_AudioProcessorPlayer.h"
#include "../../../../../../../Dev/JUCE/modules/juce_gui_extra/misc/juce_RecentlyOpenedFilesList.h"


class Engine:public FileBasedDocument{
public:
    Engine();
    ~Engine();


    // Audio

    AudioProcessorPlayer graphPlayer;


    
    ScopedPointer<ControllerManager> controllerManager;

    void createNewGraph();
    void clear();
    void initAudio();
    void stopAudio();



    //==============================================================================
    // see EngineFileDocument.cpp

    //  inherited from FileBasedDocument
    String getDocumentTitle()override ;
    Result loadDocument (const File& file)override;
    Result saveDocument (const File& file)override;
    File getLastDocumentOpened() override;
    void setLastDocumentOpened (const File& file) override;
    //    #if JUCE_MODAL_LOOPS_PERMITTED
    //     File getSuggestedSaveAsFile (const File& defaultFile)override;
    //    #endif

    // our Saving methods
    XmlElement* createXml()const;


    void restoreFromXml(const XmlElement &);
    void createNodeFromXml (const XmlElement& xml);
    
    class EngineListener{
        virtual ~EngineListener();
//        virtual void engineStarted() = 0;
        virtual void engineKilled() = 0;
    };

    ListenerList<EngineListener> engineListeners;
    void addEngineListener(EngineListener * e){engineListeners.add(e);}
    void removeEngineListener(EngineListener * e){engineListeners.remove(e);}

};


#endif  // ENGINE_H_INCLUDED
