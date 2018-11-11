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

#if !ENGINE_HEADLESS

#include "Style.h"


/////////////////////
/// LGMLUIUtils
/////////////////////

template<class F,class... Args>
struct itercomp{
    static void doit(const F & func,Component *c , int idx,Args... args){
        if(!c ) return;
        func(c,idx,args... );
        for (auto & ch:c->getChildren()){
            doit(func,ch,idx+1,args...);
            jassert(ch->getParentComponent()==c);
        }
    }
};

static void _printComp(Component * c,int idx){
    if(!c->getName().isEmpty()){
        String msg;
        for(int i = 0; i  < idx ; i++){
            msg+="-";
        }

        msg+=c->getName();
        if(c->isVisible()){
            msg+="::";
            msg+=c->getScreenBounds().toString();
        }
        else{
            msg+="////////invisible";
        }
        if(c->getParentComponent() && !c->getScreenBounds().contains(c->getParentComponent()->getScreenBounds())){

            msg+= "/// excluded";
        }
        DBG(msg);
    }
}
static void _drawBounds(Component* c ,int idx,Graphics & g,Component * relativeTo){

    if(c->isVisible()){
        Rectangle<int >r = c->getLocalBounds();
        Component* p = c;
        while(p && p!=relativeTo){
            r+= p->getPosition();
            p = p->getParentComponent();
        }
        static float strk = 0.5f;
        g.drawRect(r.toFloat().reduced(idx*strk),strk*0.5f);
    }

}

static void _repaint(Component* c ,int /*idx*/){
    c->repaint();
}





void LGMLUIUtils::printComp(Component * c) {
    itercomp<decltype(_printComp)>::doit(_printComp,c,0);
}

void LGMLUIUtils::drawBounds(Component * c,Graphics & g) {
    g.setColour(juce::Colours::red);
    itercomp<decltype(_drawBounds),Graphics &,Component*>::doit(_drawBounds,c,0,g,c);
}
void LGMLUIUtils::forceRepaint(Component * c) {
    itercomp<decltype(_repaint)>::doit(_repaint,c,0);
}




////////////////////////
// AddElementButton
///////////////////////

AddElementButton::AddElementButton(): Button ("Add") {

};
AddElementButton::~AddElementButton(){

};
void AddElementButton::paintButton (Graphics& g,
                                    bool isMouseOverButton,
                                    bool isButtonDown)
{

    auto area = getLocalBounds();
    auto bgColor = findColour (TextButton::buttonColourId);

    if((isButtonDown || isMouseOverButton) ){
        bgColor = bgColor.brighter();
    }
    g.setColour (  bgColor);
    const float stroke = 1;
    g.drawEllipse (area.toFloat().reduced (stroke / 2), stroke);
    g.setColour (findColour (TextButton::textColourOffId));
    const float hw = stroke;//area.getHeight()/18.0;
    const float offset = area.getWidth() / 4.0f ;

    const float corner = hw;

    g.fillRoundedRectangle (area.getX() + offset, area.getCentre().getY() - hw, area.getWidth() - 2 * offset, 2 * hw, corner);
    g.fillRoundedRectangle ( area.getCentre().getX() - hw, area.getY() + offset, 2 * hw, area.getHeight() - 2 * offset,  corner);

}


void AddElementButton::setFromParentBounds (const Rectangle<int>& area)
{
    auto fmb = area;
    const int fms = jmin (jmin (36, area.getWidth()), area.getHeight());
    setBounds (fmb.removeFromBottom (fms).removeFromRight (fms));
}


////////////////////////
// RemoveElementButton
///////////////////////


RemoveElementButton::RemoveElementButton(): Button ("Remove") {

};
RemoveElementButton::~RemoveElementButton(){

};
void RemoveElementButton::paintButton (Graphics& g,
                                       bool isMouseOverButton,
                                       bool isButtonDown)
{
    g.addTransform(AffineTransform::rotation(float_Pi/4.0f,getWidth()/2.0f,getHeight()/2.0f));

    auto area = getLocalBounds();
    auto bgColor = findColour (TextButton::buttonColourId);

    if((isButtonDown || isMouseOverButton) ){
        bgColor = bgColor.brighter();
    }
    g.setColour (  bgColor);
    const float stroke = 1;
    g.drawEllipse (area.toFloat().reduced (stroke / 2), stroke);
    g.setColour (Colours::red);
    const float hw = stroke;//area.getHeight()/18.0;
    const float offset = area.getWidth() / 4.0f ;

    const float corner = hw;

    g.fillRoundedRectangle (area.getX() + offset, area.getCentre().getY() - hw, area.getWidth() - 2 * offset, 2 * hw, corner);
    g.fillRoundedRectangle ( area.getCentre().getX() - hw, area.getY() + offset, 2 * hw, area.getHeight() - 2 * offset,  corner);

}


/////////////////////
// CachedGlyph
///////////////


class GlobalFonts: private DeletedAtShutdown{
public:
    ReferenceCountedArray<CachedGlyph::CachedFont> list;
};

auto usedFonts = new GlobalFonts();


CachedGlyph::CachedGlyph(const String &t,const Font & font)://,const String & fontName):
text(t),
useEllipsesIfTooBig(false),
justificationType(Justification::left)
{
    setFont(font);
}

void CachedGlyph::setSize(int w, int h){
    bounds.setSize(w, h);
    updateGlyph();
}

void CachedGlyph::setText(const String & t){
    text = t;
    updateGlyph();

}

void CachedGlyph::setFont(const Font &f){

    for(auto a : usedFonts->list){
        if(a->font==f){
            _font = a;
            return;
        }
    }
    CachedGlyph::CachedFont::Ptr nf ( new CachedGlyph::CachedFont(f));
    usedFonts->list.add(nf);
    _font = nf;
    


}

void CachedGlyph::updateGlyph(){
    if(isReady()){
        auto area = getLocalBounds();
        
        glyphArr.clear();
        glyphArr.addCurtailedLineOfText (*getCurFont(), text, 0.0f, 0.0f,
                                         (float)area.getWidth(), useEllipsesIfTooBig);

        glyphArr.justifyGlyphs (0, glyphArr.getNumGlyphs(),
                                area.getX(), area.getY(), area.getWidth(), area.getHeight(),
                                justificationType);
    }
}
bool CachedGlyph::isReady(){
    return  _font!=nullptr ;
}
Rectangle<int> & CachedGlyph::getLocalBounds(){
    return bounds;

}
Font * CachedGlyph::getCurFont(){
    return &_font->font;
}
void CachedGlyph::paint(Graphics & g){
    if(!isReady()){
        updateGlyph();
    }
    auto area = getLocalBounds();
    
    if (text.isNotEmpty())// && g.context.clipRegionIntersects (area.getSmallestIntegerContainer()))
    {
        glyphArr.draw (g);
    }
}

CachedGlyph::CachedFont::CachedFont(const Font &f):font(f){

}
CachedGlyph::CachedFont::~CachedFont(){

}

#endif
