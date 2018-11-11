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

#include "LGMLLoggerUI.h"
#include "../UI/Style.h"

#include "JuceHeader.h" // for version


void LGMLLoggerUI::newMessage (const String& s)
{
    LogElement* el = new LogElement (s);



    logElements.add (el);
    totalLogRow += el->getNumLines();


    //bool overFlow = false;

    if (totalLogRow > maxNumElement)
    {
        int curCount = 0;
        int idxToRemove = -1;

        for (int i = logElements.size() - 1 ; i >= 0 ; i--)
        {
            curCount += logElements[i]->getNumLines();

            if (curCount >= maxNumElement)
            {
                if (curCount != maxNumElement)
                {
                    logElements[i]->trimToFit (logElements[i]->getNumLines() - (curCount - maxNumElement));
                }

                idxToRemove = i - 1;
                break;
            }

        }

        if (idxToRemove >= 0)logElements.removeRange (0, idxToRemove + 1);

        totalLogRow = maxNumElement;


    }

    //coalesce messages
    triggerAsyncUpdate();

};
void LGMLLoggerUI::handleAsyncUpdate()
{
    //DBG("Handle Async Update");
    logListComponent->updateContent();
    logListComponent->scrollToEnsureRowIsOnscreen (totalLogRow - 1);
    repaint();
}

LGMLLoggerUI::LGMLLoggerUI (const String& contentName, LGMLLogger* l) :
logger (l),
ShapeShifterContentComponent (contentName),
logList (this),
maxNumElement (1000),
totalLogRow (0)
{
    logger->addLogListener (this);
    TableHeaderComponent* thc = new TableHeaderComponent();
    thc->addColumn ("Time", 1, 60);
    thc->addColumn ("Source", 2, 80);
    thc->addColumn ("Content", 3, 400);


    logListComponent = new TableListBox ("LGMLLogger", &logList);
    logListComponent->setRowHeight (13);
    logListComponent->setHeaderHeight (20);
    logListComponent->getViewport()->setScrollBarThickness (10);

    logListComponent->setColour (TableListBox::backgroundColourId, findColour (ResizableWindow::backgroundColourId));
    logListComponent->setHeader (thc);
    addAndMakeVisible (logListComponent);

    LOG ("LGML v" + String (ProjectInfo::versionString) + " : (" + String (Time::getCompilationDate().formatted("%d/%m/%y (%R)")) + ")" + "\nby OrganicOrchestra");
#if USE_FILE_LOGGER
    LOG ("please provide logFile for any bug report :\nlogFile in " + l->fileWriter.getFilePath());
#endif
    clearB.setButtonText ("Clear");
    clearB.addListener (this);
    addAndMakeVisible (clearB);

    copyB.setButtonText("Copy to Clipboard");
    copyB.addListener(this);
    addAndMakeVisible(copyB);

}

LGMLLoggerUI::~LGMLLoggerUI()
{
    handleAsyncUpdate();
    //        logListComponent.setModel(nullptr);
    logger->removeLogListener (this);
}

void LGMLLoggerUI::resized()
{

    Rectangle<int> area = getLocalBounds();
    auto footer =area.removeFromBottom (30).reduced (5);
    clearB.setBounds (footer.removeFromLeft(footer.getWidth()/2).reduced(2));
    copyB.setBounds(footer.reduced(2));
    logListComponent->setBounds (area);
    bool firstVisible  = area.getWidth() > 400;
    logListComponent->getHeader().setColumnVisible (1, firstVisible);
    bool secondVisible = area.getWidth() > 300;
    logListComponent->getHeader().setColumnVisible (2, secondVisible);

    int tw = getWidth();

    if (firstVisible)tw -= logListComponent->getHeader().getColumnWidth (1);

    if (secondVisible)tw -= logListComponent->getHeader().getColumnWidth (2);

    tw -= logListComponent->getViewport()->getScrollBarThickness();
    tw = jmax (tw, 100);
    logListComponent->getHeader().setColumnWidth (3, tw);
}



void LGMLLoggerUI::updateTotalLogRow()
{
    totalLogRow = 0;

    for (auto& l : logElements)
    {
        totalLogRow += l->getNumLines();
    }

}
const String& LGMLLoggerUI::getSourceForRow (const int r) const
{
    if(auto el = getElementForRow(r)){
        return el->source;
    }
    return String::empty;
}
const bool LGMLLoggerUI::isPrimaryRow (const int r) const
{
    int count = 0;
    int idx = 0;

    while (count <= r && idx < logElements.size())
    {
        if (count== r)
        {
            return true;
        }
        count += logElements.getUnchecked(idx)->getNumLines();
        idx++;

    }

    return false;
}

const String&   LGMLLoggerUI::getContentForRow (const int r) const
{
    int count = 0;
    int idx = 0;

    while (idx < logElements.size())
    {

        int nl = logElements.getUnchecked(idx)->getNumLines();

        if (count + nl > r)
        {
            return logElements.getUnchecked(idx)->getLine (r - count);
        }

        count += nl;
        idx++;
    }

    return String::empty;
};

const LogElement* LGMLLoggerUI::getElementForRow(const int r) const{
    int count = 0;
    int idx = 0;

    while (idx < logElements.size())
    {
        auto el = logElements.getUnchecked(idx);

        int nl = el->getNumLines();

        if (count + nl > r)
        {
            return el;
        }

        count += nl;
        idx++;
    }

    return nullptr;

}

const String  LGMLLoggerUI::getTimeStringForRow (const int r) const
{
    if(auto el = getElementForRow(r)){
        return String (el->time.toString (false, true, true, true));
    }

    return String::empty;
};

const Colour& LGMLLoggerUI::getSeverityColourForRow (const int r) const
{

    if(auto el = getElementForRow(r))
    {
        LogElement::Severity s = el->severity;

        switch (s)
        {
            case LogElement::LOG_NONE:
                return Colours::darkgrey;

            case LogElement::LOG_DBG:
                return Colours::black;

            case LogElement::LOG_WARN:
                return Colours::orange;

            case LogElement::LOG_ERR:
                return Colours::red;

            default:
                return Colours::pink;

        }

    }

    return Colours::pink;
};



//////////////
// logList

LGMLLoggerUI::LogList::LogList (LGMLLoggerUI* o) : owner (o)
{
}

int LGMLLoggerUI::LogList::getNumRows()
{

    return owner->totalLogRow;
};

void LGMLLoggerUI::LogList::paintRowBackground (Graphics& g,
                                                int rowNumber,
                                                int width, int height,
                                                bool)
{

    g.setColour (owner->getSeverityColourForRow (rowNumber).withAlpha ((rowNumber % 2 == 0 ? 0.7f : 0.6f)));
    g.fillRect (0, 0, width, height);
};

void LGMLLoggerUI::LogList::paintCell (Graphics& g,
                                       int rowNumber,
                                       int columnId,
                                       int width, int height,
                                       bool)
{
    g.setFont (12);
    g.setColour (owner->findColour (Label::textColourId));
    String text;

    switch (columnId)
    {
        case 1:
            text = owner->isPrimaryRow(rowNumber)?owner->getTimeStringForRow (rowNumber):String::empty;
            break;

        case 2:
            text = owner->isPrimaryRow(rowNumber)?owner->getSourceForRow (rowNumber):String::empty;
            break;

        case 3:
            text = owner->getContentForRow (rowNumber);
            break;
    }

    g.drawFittedText (text, 0, 0, width, height, Justification::left, 1);

};

String LGMLLoggerUI::LogList::getCellTooltip (int rowNumber, int /*columnId*/)
{
    auto el = owner->getElementForRow(rowNumber);

    String sR = el->source;
    return
    (sR.isNotEmpty()?
     sR +" ("+el->time.toString(false, true, true, true)+")"+ "\n":String::empty)
    +(el->getNumLines()< 10?el->content:owner->getSourceForRow(rowNumber));


};

void LGMLLoggerUI::buttonClicked (Button* b)
{
    
    if (b == &clearB)
    {
        logElements.clear();
        totalLogRow = 0;
        logListComponent->updateContent();
        LOG ("Cleared.");
    }

    else if (b == &copyB){
        String s;
        for(auto & el: logElements){
            int leftS = el->source.length() + 3;
            s+=el->source+" : ";
            for(int i = 0 ; i < el->getNumLines() ; i++){
                if (i!=0)for ( int j = 0; j < leftS ; j++) s+=" ";
                s+=el->getLine(i)+"\n";
            }
        }
        SystemClipboard::copyTextToClipboard (s);
    }
}
