/*
  ==============================================================================

    AudioInNode.cpp
    Created: 7 Mar 2016 8:03:48pm
    Author:  Martin Hermant

  ==============================================================================
*/

#include "AudioInNode.h"

#if !HEADLESS
#include "NodeBaseUI.h"

NodeBaseUI * AudioInNode::createUI() {
    NodeBaseUI * ui = new NodeBaseUI(this);
    return ui;

}
#endif