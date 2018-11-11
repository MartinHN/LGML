/*
 ==============================================================================

 ParameterContainerSync.cpp
 Created: 12 Oct 2017 10:02:38am
 Author:  Martin Hermant

 ==============================================================================
 */

#include "ParameterContainerSync.h"



ParameterContainerSync::ParameterContainerSync(const String& name,ParameterContainer *_slave=nullptr):root(nullptr),slave(_slave){
    if(!slave)slave = new ParameterContainer(name);
    slave->nameParam->isEditable = false;

}
ParameterContainerSync::~ParameterContainerSync(){
    setRoot(nullptr);
}

void ParameterContainerSync::setRoot(ParameterContainer * _root){
    if(root){
        root->removeControllableContainerListener(this);
        clear();
    }
    root = _root;
    if(root){
        root->addControllableContainerListener(this);
        checkContInSync(root);
    }

}
ParameterContainer * ParameterContainerSync::getSlaveRelatedContainer(ParameterContainer *c,bool tryLastName){
    if(c==root){
        return slave;
    }
 StringArray arr (c->getControlAddressArray(root));
    ParameterContainer * inner = dynamic_cast<ParameterContainer*>(slave->getControllableContainerForAddress(arr));;
    if( tryLastName && !inner &&  arr.size()){
        StringArray lastArr (arr);
        lastArr.getReference(arr.size()-1) = Controllable::toShortName(c->nameParam->lastValue.toString());
        if(auto lastIn =  dynamic_cast<ParameterContainer*>(slave->getControllableContainerForAddress(lastArr))){
            inner = lastIn;
        }
    }
    

    return inner;
}

ParameterContainer * ParameterContainerSync::getRootRelatedContainer(ParameterContainer *c){
    const StringArray arr (c->getControlAddressArray(slave));
    auto inner = dynamic_cast<ParameterContainer*>(root->getControllableContainerForAddress(arr));

    return inner;
}

void ParameterContainerSync::clear () {
//    slave->clearContainer();
};

ParameterContainer * ParameterContainerSync::getSlaveContainer(){
    return slave;

}





void ParameterContainerSync::controllableContainerAdded (ControllableContainer* /*notifier*/, ControllableContainer* cont) {
    checkContExists(dynamic_cast<ParameterContainer*>(cont));
    checkContInSync(dynamic_cast<ParameterContainer*>(cont));
}

void ParameterContainerSync::checkContExists(ParameterContainer * fromRoot){
    ScopedPointer<ParameterContainer> testTarget = createContainerFromContainer(fromRoot);
    if(fromRoot!=root && !getSlaveRelatedContainer(fromRoot)){

        if(!testTarget){
            return;
        }
        auto arr =fromRoot->getControlAddressArray(root);

        StringArray added;
        ParameterContainer * parent = slave;
        // create object path if not exists
        for(auto& a:arr){
            added.add(a);
            ParameterContainer * target =dynamic_cast<ParameterContainer*>(slave->getControllableContainerForAddress(added));



            if( !target ){
                target = testTarget.release();


                if(!target){
                    target  = new ParameterContainer(fromRoot->getNiceName());
                    
                }
                else{
                    target->setNiceName(fromRoot->getNiceName());
                }
                target->nameParam->isEditable = false;


                parent->addChildControllableContainer(target);
                
            }
            else{
                
                
            }
            parent = target;
        }
    }
}
void ParameterContainerSync::checkContInSync(ParameterContainer * fromRoot){
    if(!slave ){jassertfalse;return;}
    if(!fromRoot){jassertfalse;return;} // happens on deletions


    for(auto c:fromRoot->getContainersOfType<ParameterContainer>(false)){
        if(!getSlaveRelatedContainer(c)){
            controllableContainerAdded(nullptr, c);
        }
    }
    
    auto slaveCont = getSlaveRelatedContainer(fromRoot);
    if(slaveCont){
    for(auto c:slaveCont->getContainersOfType<ParameterContainer>(false)){
        if(! getRootRelatedContainer(c)){
            slaveCont->removeChildControllableContainer(c);
        }
    }
    }
};


void ParameterContainerSync::controllableContainerRemoved (ControllableContainer*, ControllableContainer* origin) {
    auto pc=dynamic_cast<ParameterContainer*>(origin);
    if(!pc){jassertfalse;return;}
    if(auto inner = getSlaveRelatedContainer(pc)){
        if(inner!=slave){
        inner->parentContainer->removeChildControllableContainer(inner);
        }
    }
    else{
        jassertfalse;
    }

};



void ParameterContainerSync::childStructureChanged (ControllableContainer* notifier, ControllableContainer* origin,bool /*isAdded*/) {

    if(notifier==root){
        auto pc=dynamic_cast<ParameterContainer*>(origin);
        if(!pc){jassertfalse;return;}
        checkContInSync(pc);
    }




};
void ParameterContainerSync::childAddressChanged (ControllableContainer* /*notifier*/,ControllableContainer* c) {
    if(auto inner = getSlaveRelatedContainer(dynamic_cast<ParameterContainer*>(c),true)){
        inner->setNiceName(c->getNiceName());
    }
    else{
        jassertfalse;// should find controller
    }


};
void ParameterContainerSync::controllableContainerPresetLoaded (ControllableContainer*) {

};
void ParameterContainerSync::containerWillClear (ControllableContainer* c) {
    auto pc=dynamic_cast<ParameterContainer*>(c);
    if(c==root){
        setRoot(nullptr);
        return;
    }
    if(!pc){
        jassertfalse; // hapen on closing file (destructor methods)
        return;
    }
    
        
    
    
    
};
