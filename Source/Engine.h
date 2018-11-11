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

#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#define MULTITHREADED_LOADING


#include "JuceHeaderAudio.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_utils/players/juce_AudioProcessorPlayer.h>
#include "MIDI/MIDIManager.h"
#include "Controller/ControllerManager.h"
#include "Node/Manager/NodeManager.h"
#include "Preset/PresetManager.h"
#include "Utils/AudioFucker.h"
#include "Time/TimeManager.h"
#include "FastMapper/FastMapper.h"
#include "Audio/VSTManager.h"
#include "Utils/ProgressNotifier.h"
#include "Utils/CommandLineElements.hpp"
class AudioFucker;



#if ENGINE_HEADLESS
#include "Utils/HeadlessWrappers.h"
#endif

class Engine:
    public FileBasedDocument,
    NodeManager::NodeManagerListener,
    AsyncUpdater,
    public ProgressNotifier,
    public ParameterContainer
{
public:
    Engine();
    ~Engine();

    // Audio
    AudioProcessorPlayer graphPlayer;

    void createNewGraph();
    void clear();
    void initAudio();
    bool fadeAudioOut();
    void closeAudio();
    ParameterBase* saveSession,*loadSession,*closeEngine;
    class EngineStats : public ParameterContainer ,public Timer{
    public:
        EngineStats(Engine *);
        void activateGlobalStats(bool);
        float getAudioCPU() const;
        Point2DParameter<floatParamType> * audioCpu;
        bool isListeningGlobal;
        typedef OwnedFeedbackListener<EngineStats> GlobalListener;
        typedef HashMap<String, Array<int>,DefaultHashFunctions,CriticalSection> CountMapType;
         CountMapType modCounts;
    private:
        void timerCallback()override;
        Engine * engine;
        int timerTicks;
        ScopedPointer< GlobalListener > globalListener;
    };
    ScopedPointer< EngineStats> engineStats;
    bool hasDefaultOSCControl;
    void onContainerParameterChanged( ParameterBase*)override;
    void onContainerTriggerTriggered(Trigger *t)override;
    void suspendAudio (bool shouldSuspend);

    void parseCommandline (const CommandLineElements& );

    //==============================================================================
    // see EngineFileDocument.cpp

    //  inherited from FileBasedDocument
    String getDocumentTitle()override ;
    // do not call this, call loadFrom instead (empowers FileBasedDocument behaviour)
    Result loadDocument (const File& file)override;
    Result saveDocument (const File& file)override;
    File getLastDocumentOpened() override;
    void setLastDocumentOpened (const File& file) override;

    // helpers for file document
    File getCurrentProjectFolder();
    // return absolute Path if out of project directory
    String getNormalizedFilePath (const File& f);
    File getFileAtNormalizedPath (const String& path);

    //    #if JUCE_MODAL_LOOPS_PERMITTED
    //     File getSuggestedSaveAsFile (const File& defaultFile)override;
    //    #endif

    // our Saving methods
    DynamicObject* getObject() override;
    void loadJSONData (const var& data, ProgressTask* loadingTask);

    bool checkFileVersion (DynamicObject* metaData);

    String getMinimumRequiredFileVersion();

    void  stimulateAudio (bool);
    ScopedPointer<AudioFucker> stimulator;

    class MultipleAudioSettingsHandler : public ChangeListener, public Timer
    {
    public:
        MultipleAudioSettingsHandler(): oldSettingsId ("oldAudioSettings") {}
        Identifier oldSettingsId;
        void changeListenerCallback (ChangeBroadcaster* )override;
        void saveCurrent();
        String getConfigName();

        String lastConfigName;
        void timerCallback()override;

    };
    MultipleAudioSettingsHandler audioSettingsHandler;

    int64 engineStartTime;
    int loadingStartTime;
    const int getElapsedMillis()const ;

    void managerEndedLoading() override;
    void managerProgressedLoading (float progress) override;


    void fileLoaderEnded();
    bool allLoadingThreadsAreEnded();
    void loadDocumentAsync (const File& file);

    class FileLoader : public Thread, private Timer
    {
    public:
        FileLoader (Engine* e, File f): Thread ("EngineLoader"), owner (e), fileToLoad (f)
        {
            //startTimerHz(4);
            //fakeProgress = 0;
            isEnded = false;
        }
        ~FileLoader()
        {

        }

        void timerCallback()override
        {
            //fakeProgress+=getTimerInterval()/5000.0;
            //fakeProgress = jmin(1.0f,fakeProgress);
            //owner->engineListeners.call(&EngineListener::fileProgress,fakeProgress, 0);
        }

        void run() override
        {
            owner->loadDocumentAsync (fileToLoad);
            isEnded = true;
            owner->fileLoaderEnded();
        }

        //float fakeProgress ;
        Engine* owner;
        File fileToLoad;
        bool isEnded;


    };

    ScopedPointer<FileLoader> fileLoader;



    class EngineListener
    {
    public:
        virtual ~EngineListener() {};
        virtual void startEngine() {};
        virtual void stopEngine() {};
        virtual void startLoadFile() {};
        // TODO implement progression
        virtual void fileProgress (float percent, int state) {};
        virtual void endLoadFile() {};
    };

    ListenerList<EngineListener> engineListeners;
    void addEngineListener (EngineListener* e) {engineListeners.add (e);}
    void removeEngineListener (EngineListener* e) {engineListeners.remove (e);}

    bool isLoadingFile;
    var jsonData;

    void handleAsyncUpdate()override;

    ThreadPool threadPool;

    RecentlyOpenedFilesList getLastOpenedFileList();

    static void setLanguage(const String & l);
    static StringArray getAvailableLanguages();

    static int versionNumber;
    static const char* versionString;
    static const char* projectName;

};




extern Engine* getEngine();
extern bool isEngineLoadingFile();
extern AudioDeviceManager& getAudioDeviceManager();
extern ThreadPool* getEngineThreadPool();

#endif  // ENGINE_H_INCLUDED
