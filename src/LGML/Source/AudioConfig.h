/*
  ==============================================================================

    AudioConfig.h
    Created: 16 Feb 2017 8:11:16pm
    Author:  Martin Hermant

  ==============================================================================
*/

#ifndef AUDIOCONFIG_H_INCLUDED
#define AUDIOCONFIG_H_INCLUDED

#include "JuceHeader.h"


// TODO change when windows / linux support
#ifdef JUCE_MAC
#define BUFFER_CAN_STRETCH 1
#else
#define BUFFER_CAN_STRETCH 0
#endif



// TODO change when window support
// @ ben header only so should be easy
#if JUCE_MAC
#define LINK_SUPPORT 1
#else
#define LINK_SUPPORT 0
#endif



#endif  // AUDIOCONFIG_H_INCLUDED
