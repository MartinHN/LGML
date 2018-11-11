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

#include "LooperNodeUI.h"
#include "../../Controllable/Parameter/UI/ParameterUIFactory.h"
#include "../../Controllable/ControllableUIHelpers.h"




LooperNodeContentUI::LooperNodeContentUI()
{

}

LooperNodeContentUI::~LooperNodeContentUI()
{
    looperNode->removeLooperListener (this);
}

void LooperNodeContentUI::init()
{

    looperNode = (LooperNode*)node.get();
    looperNode->addLooperListener (this);

    recPlaySelectedButton = ParameterUIFactory::createDefaultUI (looperNode->recPlaySelectedTrig);
    clearSelectedButton = ParameterUIFactory::createDefaultUI (looperNode->clearSelectedTrig);
    stopSelectedButton = ParameterUIFactory::createDefaultUI (looperNode->stopSelectedTrig);

    clearAllButton = ParameterUIFactory::createDefaultUI (looperNode->clearAllTrig);
    stopAllButton = ParameterUIFactory::createDefaultUI (looperNode->stopAllTrig);

    volumeSelectedSlider = ParameterUIFactory::createDefaultUI (looperNode->volumeSelected);
    monitoringButton = ParameterUIFactory::createDefaultUI (looperNode->isMonitoring);

    headerContainer.addAndMakeVisible (recPlaySelectedButton);
    headerContainer.addAndMakeVisible (clearSelectedButton);
    headerContainer.addAndMakeVisible (stopSelectedButton);
    headerContainer.addAndMakeVisible (clearAllButton);
    headerContainer.addAndMakeVisible (stopAllButton);
    headerContainer.addAndMakeVisible (monitoringButton);


    addAndMakeVisible (headerContainer);
    addAndMakeVisible (trackContainer);

    setDefaultSize (350, 180);



    trackNumChanged (looperNode->numberOfTracks->intValue());

    if (looperNode->selectTrack->intValue() >= 0)looperNode->trackGroup.tracks.getUnchecked (looperNode->selectTrack->intValue())->setSelected (true);
}

void LooperNodeContentUI::resized()
{
    Rectangle<int> area = getLocalBounds();

    headerContainer.setBounds (area.removeFromTop (30));
    trackContainer.setBounds (area);
    reLayoutHeader();
    reLayoutTracks();

}


void LooperNodeContentUI::reLayoutHeader()
{

    float selectedW = .4f;
    Rectangle<int> area = headerContainer.getBounds().reduced (4, 0);
    int pad = 2;
    Rectangle<int> selTrackArea = area.removeFromLeft ((int) (selectedW * area.getWidth()));
    recPlaySelectedButton->setBounds (selTrackArea.removeFromLeft ((int) (.5f * selTrackArea.getWidth())).reduced (pad));
    stopSelectedButton->setBounds (selTrackArea.removeFromTop ((int) (.5f * selTrackArea.getHeight())).reduced (pad));
    clearSelectedButton->setBounds (selTrackArea.reduced (pad));

    stopAllButton->setBounds (area.removeFromLeft (area.getWidth() / 3).reduced (pad));
    clearAllButton->setBounds (area.removeFromLeft (area.getWidth() / 2).reduced (pad));
    monitoringButton->setBounds (area.removeFromTop (area.getHeight()).reduced (pad));

}
void LooperNodeContentUI::reLayoutTracks()
{
    if (tracksUI.size() == 0) return;



    int numCol = jmin (8, tracksUI.size());
    int numRow = (int)ceil ((tracksUI.size()) * 1.f / numCol);



    float gap = 2;
    float margin = 5;

    Rectangle<int> innerTrackR = trackContainer.getLocalBounds().reduced (margin);
    float trackWidth =   innerTrackR.getWidth() * 1.0f / numCol;
    float trackHeight = innerTrackR.getHeight() * 1.0f / numRow;

    int trackIndex = 0;

    for (int j = 0 ; j < numRow ; j++)
    {

        Rectangle<int> rowRect = innerTrackR.removeFromTop ((int)trackHeight);

        for (int i = 0 ; i < numCol ; i++)
        {

            if (trackIndex >= tracksUI.size()) break;

            tracksUI.getUnchecked (trackIndex)->setBounds (rowRect.removeFromLeft ((int)trackWidth).reduced (gap));

            trackIndex++;

        }

        if (j < numRow - 1) innerTrackR.removeFromTop ((int)gap);

    }
}

void LooperNodeContentUI::trackNumChanged (int num)
{

    execOrDefer ([ = ]()
    {
        if (num < tracksUI.size())
        {
            tracksUI.removeRange (num, tracksUI.size() - num);
        }
        else
        {

            int safe_num = jmin (looperNode->trackGroup.tracks.size(), num);

            for (int i = tracksUI.size() ; i < safe_num ; i++)
            {
                TrackUI* t = new TrackUI (looperNode->trackGroup.tracks.getUnchecked (i));
                tracksUI.add (t);
                trackContainer.addAndMakeVisible (t);
            }
        }

        resized();
    }
                );

};



//////////////
// Track UI
////////////////


LooperNodeContentUI::TrackUI::TrackUI (LooperTrack* track) :InspectableComponent(track), track (track),
    isSelected (false), timeStateUI (track)
{
    recPlayButton = ParameterUIFactory::createDefaultUI (track->recPlayTrig);
    recPlayButton->setCustomText (">");
    clearButton = ParameterUIFactory::createDefaultUI (track->clearTrig);
    clearButton->setCustomText ("x");
    stopButton = ParameterUIFactory::createDefaultUI (track->stopTrig);
    stopButton->setCustomText (CharPointer_UTF8 ("■"));
    muteButton = ParameterUIFactory::createDefaultUI (track->mute);
    soloButton = ParameterUIFactory::createDefaultUI (track->solo);
    sampleChoiceDDL = (EnumParameterUI*)ParameterUIFactory::createDefaultUI (track->sampleChoice);
    selectMeButton = ParameterUIFactory::createDefaultUI(track->selectTrig);
    selectMeButton->setCustomText("_");
    selectMeButton->setColour(TextButton::buttonColourId, Colours::white.withAlpha (0.f));


    track->addTrackListener (this);

    addAndMakeVisible (recPlayButton);
    addAndMakeVisible (clearButton);
    volumeSlider = new FloatSliderUI (track->volume);
    volumeSlider->orientation = FloatSliderUI::VERTICAL;
    addAndMakeVisible (volumeSlider);
    addAndMakeVisible (stopButton);
    addAndMakeVisible (muteButton);
    addAndMakeVisible (soloButton);
    addAndMakeVisible (timeStateUI);
    addAndMakeVisible (sampleChoiceDDL);
    addAndMakeVisible(selectMeButton);
    selectMeButton->toBack();
}

LooperNodeContentUI::TrackUI::~TrackUI()
{
    track->removeTrackListener (this);
}

void LooperNodeContentUI::TrackUI::paint (Graphics& g)
{
    g.setColour (findColour (ResizableWindow::backgroundColourId).brighter().withAlpha (0.5f));
    g.fillRoundedRectangle (getLocalBounds().toFloat(), 2.f);

}

void LooperNodeContentUI::TrackUI::paintOverChildren (Graphics& g)
{

    g.setColour ((track->isEmpty() || track->trackState == LooperTrack::TrackState::STOPPED) ? findColour (Label::textColourId) : Colours::black);
    g.setFont (12);
    g.drawText ( String (track->trackIdx), timeStateUI.getBounds(), Justification::centred);

    if (isSelected)
    {
        g.setColour (Colours::yellow);
        g.drawRoundedRectangle (getLocalBounds().reduced (1).toFloat(), 2.f, 1.f);
    }
}

void LooperNodeContentUI::TrackUI::resized()
{
    Rectangle<int> r = getLocalBounds().reduced (2);


    const int timeUISize = 16;
    //  Rectangle<int>  hr = r.removeFromTop(timeUISize+gap);

    selectMeButton->setBounds(r.withBottom(timeUISize));
    timeStateUI.setBounds (r.removeFromTop (timeUISize).withSize (timeUISize, timeUISize).reduced (2)); //header
    sampleChoiceDDL->setBounds (r.removeFromTop (20).reduced (1));

    volumeSlider->setBounds (r.removeFromRight (r.getWidth() / 3).reduced (1));
    r.reduce (4, 0);

    int step = r.getHeight() / 6 ;

    muteButton->setBounds (r.removeFromTop (step).reduced (0, 1));
    soloButton->setBounds (r.removeFromTop (step).reduced (0, 1));
    r.removeFromTop (2);
    clearButton->setBounds (r.removeFromTop (step).reduced (0, 1));
    stopButton->setBounds (r.removeFromTop (step).reduced (0, 1));

    recPlayButton->setBounds (r);



}





///////////////////////
// TimeStateUI



LooperNodeContentUI::TrackUI::TimeStateUI::TimeStateUI (LooperTrack* _track): track (_track)
{
    track->addTrackListener (this);
    setTrackTimeUpdateRateHz (10);
    trackStateChangedAsync (_track->trackState);
    
}
LooperNodeContentUI::TrackUI::TimeStateUI::~TimeStateUI()
{
    track->removeTrackListener (this);
}
void LooperNodeContentUI::TrackUI::TimeStateUI::paint (Graphics& g)
{
    Path p;
    Rectangle<int> r = getLocalBounds();

    // For a circle, we can avoid having to generate a stroke

    float angle = 2.0f * float_Pi * trackPosition;

    g.setColour (track->isBusy()?mainColour.darker():mainColour);
    g.fillEllipse (r.toFloat());

    if (track->trackState != LooperTrack::TrackState::PLAYING) return;

    //Draw play indic
    g.setColour (Colours::yellow.withAlpha (.6f));
    p.startNewSubPath (r.getCentreX(), r.getCentreY());

    p.addArc (r.getX(), r.getY(), r.getWidth(), r.getHeight(), 0, angle);
    //p.setUsingNonZeroWinding (false);
    p.closeSubPath();
    g.fillPath (p);

    angle -= float_Pi / 2;
    g.setColour (Colours::orange.withAlpha (.8f));
    g.drawLine (r.getCentreX(), r.getCentreY(), r.getCentreX() + cosf (angle)*r.getWidth() / 2, r.getCentreX() + sinf (angle)*r.getHeight() / 2, 2);

    g.setColour(Colours::white.withAlpha(0.5f));
    for(auto  o:track->getNormalizedOnsets()){
        float oAngle = o*2*float_Pi - float_Pi/2;
        g.drawLine (r.getCentreX(), r.getCentreY(),
                    r.getCentreX() + cosf (oAngle)*r.getWidth() / 2, r.getCentreX() + sinf (oAngle)*r.getHeight() / 2,
                    .5);
    }


}
void LooperNodeContentUI::TrackUI::TimeStateUI::trackStateChangedAsync (const LooperTrack::TrackState& state)
{
    trackPosition = 0;

    switch (state)
    {
        case LooperTrack::TrackState::RECORDING:
            mainColour = Colours::red;
            break;

        case LooperTrack::TrackState::PLAYING:
            mainColour = Colours::green.brighter (.3f);
            break;

        case LooperTrack::TrackState::WILL_RECORD:
            mainColour = Colours::orange;
            break;

        case LooperTrack::TrackState::WILL_PLAY:
            mainColour = Colours::cadetblue;
            break;

        case LooperTrack::TrackState::CLEARED:
            mainColour = Colours::grey;
            break;

        case LooperTrack::TrackState::STOPPED:
        case LooperTrack::TrackState::WILL_STOP:
            mainColour = Colours::grey.darker();
            break;

        default:
            jassertfalse;
            break;
    }

    repaint();
}
void LooperNodeContentUI::TrackUI::TimeStateUI::trackTimeChangedAsync (double /*beat*/)
{
    repaint();
}

#endif
