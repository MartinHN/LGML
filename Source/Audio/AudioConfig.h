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

#ifndef AUDIOCONFIG_H_INCLUDED
#define AUDIOCONFIG_H_INCLUDED


#include "../JuceHeaderCore.h"

// TODO change when windows / linux support
#if (defined JUCE_MAC || defined JUCE_LINUX || defined JUCE_WINDOWS)
    #define BUFFER_CAN_STRETCH 1
#else
    #define BUFFER_CAN_STRETCH 0
#endif







#endif  // AUDIOCONFIG_H_INCLUDED
