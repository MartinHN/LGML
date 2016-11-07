/*
 ==============================================================================

 NodeContainer.cpp
 Created: 18 May 2016 7:53:56pm
 Author:  bkupe

 ==============================================================================
 */

#include "NodeContainer.h"
#include "NodeManager.h"
#include "NodeConnection.h"
#include "NodeContainerUI.h"


#include "DebugHelpers.h"



extern AudioDeviceManager& getAudioDeviceManager();
bool isEngineLoadingFile();

NodeContainer::NodeContainer(const String &name) :
containerInNode(nullptr),
containerOutNode(nullptr),
NodeBase(name, NodeType::ContainerType,false),
nodeChangeNotifier(10000)
{
  saveAndLoadRecursiveData = false;
  innerGraph = new AudioProcessorGraph();
  innerGraph->releaseResources();
  setPreferedNumAudioOutput(2);
  setPreferedNumAudioInput(2);
#ifdef MULTITHREADED_AUDIO
  graphJob = new GraphJob(innerGraph,name);
  nodeManager = nullptr;
#endif

  //Force non recursive saving of preset as container has only is level to take care, nested containers are other nodes
  presetSavingIsRecursive = false;
}


NodeContainer::~NodeContainer()
{
  //connections.clear();
  clear(false);
    innerGraph->releaseResources();
}
void NodeContainer::clear(){

//    innerGraph->clear();
    clear(false);

  
  innerGraph->releaseResources();

}
void NodeContainer::clear(bool recreateContainerNodes)
{

  while (connections.size() > 0)
  {
    connections[0]->remove();
  }


  
  while (nodes.size() > 0)
  {
    if(nodes[0].get()){
    nodes[0]->remove();
    }
    else{
      jassertfalse;
      nodes.remove(0);
    }
    

  }


  containerInNode = nullptr;
  containerOutNode = nullptr;
  setPreferedNumAudioOutput(2);
  setPreferedNumAudioInput(2);

  if (recreateContainerNodes && parentNodeContainer != nullptr)
  {
    containerInNode = (ContainerInNode *)addNode(new ContainerInNode());
    containerOutNode = (ContainerOutNode *)addNode(new ContainerOutNode());

    containerInNode->xPosition->setValue(150);
    containerInNode->yPosition->setValue(100);
    containerOutNode->xPosition->setValue(450);
    containerOutNode->yPosition->setValue(100);

    //maybe keep it ?
    addConnection(containerInNode, containerOutNode, NodeConnection::ConnectionType::AUDIO);
  }

  while (proxyParams.size() > 0)
  {
    removeParamProxy(proxyParams[0]);
  }
  if(!recreateContainerNodes)ConnectableNode::clear();

  // init with global sample rate and blockSize
  AudioIODevice * ad = getAudioDeviceManager().getCurrentAudioDevice();
  if(ad){
    setRateAndBufferSizeDetails(ad->getCurrentSampleRate(), ad->getCurrentBufferSizeSamples());
  }

}


ConnectableNode * NodeContainer::addNode(NodeType nodeType, const String &nodeName)
{
  ConnectableNode * n = NodeFactory::createNode(nodeType);
  return addNode(n,nodeName);
}

ConnectableNode * NodeContainer::addNodeFromJSONData(var data)
{
	NodeType nodeType = NodeFactory::getTypeFromString(data.getProperty("nodeType", var())); 
	ConnectableNode * n = NodeFactory::createNode(nodeType);
	return addNode(n,n->niceName,data);
}

ConnectableNode * NodeContainer::addNode(ConnectableNode * n, const String &nodeName, var nodeData)
{
  nodes.add(n);
  
  n->setParentNodeContainer(this);

  if (NodeContainer * nc = dynamic_cast<NodeContainer*>(n))
  {
    nodeContainers.add(nc);
    nc->NodeContainer::clear(!isEngineLoadingFile());
    //DBG("Check containerIn Node : " << String(((NodeContainer *)n)->containerInNode != nullptr));
  }


  n->addConnectableNodeListener(this);
  String targetName = (nodeName.isNotEmpty())?nodeName:n->nameParam->stringValue();
  n->nameParam->setValue(getUniqueNameInContainer(targetName));

  addChildControllableContainer(n); //ControllableContainer

  if (!nodeData.isVoid()) n->loadJSONData(nodeData);

  nodeChangeNotifier.addMessage(new NodeChangeMessage(n,true));
//  nodeContainerListeners.call(&NodeContainerListener::nodeAdded, n);
  return n;
}





bool NodeContainer::removeNode(ConnectableNode * n)
{
  Array<NodeConnection *> relatedConnections = getAllConnectionsForNode(n);

  for (auto &connection : relatedConnections) removeConnection(connection);

  if (n == nullptr){jassertfalse; return false;}
  n->removeConnectableNodeListener(this);
  removeChildControllableContainer(n);

  nodeChangeNotifier.addMessage(new NodeChangeMessage(n,false));
//  nodeContainerListeners.call(&NodeContainerListener::nodeRemoved, n);
  nodes.removeAllInstancesOf(n);

  n->clear();


  if (NodeContainer * nc = dynamic_cast<NodeContainer*>(n)) nodeContainers.removeFirstMatchingValue(nc);

  n->removeFromAudioGraph();
  //if(NodeManager::getInstanceWithoutCreating() != nullptr)
//  getAudioGraph()->removeNode(n->audioNode);

  return true;
}

ConnectableNode * NodeContainer::getNodeForName(const String & name)
{
  for (auto &n : nodes)
  {
    if (n->shortName == name) return n;
  }
  return nullptr;
}

void NodeContainer::updateAudioGraph(bool lock) {


  if(!MessageManager::getInstance()->isThisTheMessageThread()){
    if(lock){
      const ScopedLock lk (getAudioGraph()->getCallbackLock());
      getAudioGraph()->suspendProcessing(true);
    }
    else{
      getAudioGraph()->suspendProcessing(true);
    }
    triggerAsyncUpdate();

    return;
  }

  if(lock){
  const ScopedLock lk (getAudioGraph()->getCallbackLock());
  getAudioGraph()->setRateAndBufferSizeDetails(NodeBase::getSampleRate(),NodeBase::getBlockSize());
  getAudioGraph()->prepareToPlay(NodeBase::getSampleRate(),NodeBase::getBlockSize());
  getAudioGraph()->suspendProcessing(false);
  }
  else{
    getAudioGraph()->setRateAndBufferSizeDetails(NodeBase::getSampleRate(),NodeBase::getBlockSize());
    getAudioGraph()->prepareToPlay(NodeBase::getSampleRate(),NodeBase::getBlockSize());
    getAudioGraph()->suspendProcessing(false);
  }

  //  }


}
void NodeContainer::handleAsyncUpdate(){
  if(!isEngineLoadingFile()){
   updateAudioGraph();
  }
  else{
    triggerAsyncUpdate();
    
  }
}

int NodeContainer::getNumConnections() {
  return connections.size();
}

ParameterProxy * NodeContainer::addParamProxy()
{
  ParameterProxy * p = new ParameterProxy();
  ControllableContainer::addParameter(p);
  proxyParams.add(p);
  p->addParameterProxyListener(this);

  nodeContainerListeners.call(&NodeContainerListener::paramProxyAdded, p);

  return p;
}

void NodeContainer::removeParamProxy(ParameterProxy * pp)
{
  pp->removeParameterProxyListener(this);

  nodeContainerListeners.call(&NodeContainerListener::paramProxyRemoved, pp);
  proxyParams.removeAllInstancesOf(pp);
  removeControllable(pp);
}



var NodeContainer::getJSONData()
{
  var data = ConnectableNode::getJSONData();
  var nodesData;

  for (auto &n : nodes)
  {
    nodesData.append(n->getJSONData());
  }

  var connectionsData;

  for (auto &c : connections)
  {
    connectionsData.append(c->getJSONData());
  }

  data.getDynamicObject()->setProperty("nodes", nodesData);
  data.getDynamicObject()->setProperty("connections", connectionsData);

  var proxiesData;
  for (auto &pp : proxyParams)
  {
    proxiesData.append(pp->getJSONData());
  }

  data.getDynamicObject()->setProperty("proxies", proxiesData);

  return data;
}


void NodeContainer::loadJSONDataInternal(var data)
{
  // do we really need that ???
  clear(false);

  Array<var> * nodesData = data.getProperty("nodes", var()).getArray();
  if(nodesData!=nullptr){
    for (var &nData : *nodesData)
    {
      ConnectableNode * node = addNodeFromJSONData(nData);

	  if (node->type == NodeType::ContainerInType) containerInNode = (ContainerInNode *)node;
	  else if (node->type == NodeType::ContainerOutType) containerOutNode = (ContainerOutNode *)node;
    }
  }

  Array<var> * connectionsData = data.getProperty("connections", var()).getArray();

  if (connectionsData)
  {
    for (var &cData : *connectionsData)
    {

      ConnectableNode * srcNode = (ConnectableNode*)(getNodeForName(cData.getDynamicObject()->getProperty("srcNode").toString())) ;
      ConnectableNode * dstNode = (ConnectableNode*)(getNodeForName(cData.getDynamicObject()->getProperty("dstNode").toString()));

      int cType = cData.getProperty("connectionType", var());

      if (srcNode && dstNode && isPositiveAndBelow(cType, (int)NodeConnection::ConnectionType::UNDEFINED)) {
        NodeConnection * c = addConnection(srcNode, dstNode, NodeConnection::ConnectionType(cType));
        // if c == null connection already exist, should never happen loading JSON but safer to check
        if(c){
          c->loadJSONData(cData);
        }


      }
      else {
        // TODO nicely handle file format errors?

        if(srcNode==nullptr){
          NLOG("loadJSON","no srcnode for shortName : "+cData.getDynamicObject()->getProperty("srcNode").toString());
        }
        if(dstNode==nullptr){
          NLOG("loadJSON","no dstnode for shortName : "+cData.getDynamicObject()->getProperty("dstNode").toString());
        }
        LOG("Available Nodes in "+ shortName+" : ");

#if defined DEBUG
        for (auto &node : nodes)
        {
          DBG(" > " + node->niceName+"//"+ node->shortName);
        }
#endif

        jassertfalse;
      }
    }
  }





  Array<var> * proxiesData = data.getProperty("proxies", var()).getArray();

  if (proxiesData)
  {
    for (var &pData : *proxiesData)
    {
      ParameterProxy * p = addParamProxy();
      p->loadJSONData(pData);
    }
  }


  removeIllegalConnections();
}


NodeConnection * NodeContainer::getConnectionBetweenNodes(ConnectableNode * sourceNode, ConnectableNode * destNode, NodeConnection::ConnectionType connectionType)
{
  if(sourceNode==nullptr || destNode == nullptr){
    DBG("wrong Connection");
    return nullptr;
  }
  ConnectableNode * tSourceNode =  sourceNode;
  ConnectableNode * tDestNode =  destNode;

  for(auto &c: connections)
  {
    if (c->sourceNode == tSourceNode && c->destNode == tDestNode && c->connectionType == connectionType) return c;
  }

  return nullptr;
}

Array<NodeConnection*> NodeContainer::getAllConnectionsForNode(ConnectableNode * node)
{
  Array<NodeConnection*> result;

  ConnectableNode * tSourceNode = node;
  ConnectableNode * tDestNode = node;

  for (auto &connection : connections)
  {
    if (connection->sourceNode == tSourceNode || connection->destNode == tDestNode)
    {
      result.add(connection);
    }
  }



  return result;
}

NodeConnection * NodeContainer::addConnection(ConnectableNode * sourceNode, ConnectableNode * destNode, NodeConnection::ConnectionType connectionType)
{

  ConnectableNode * tSourceNode = sourceNode;
  ConnectableNode * tDestNode =  destNode;

  if (getConnectionBetweenNodes(tSourceNode, tDestNode, connectionType) != nullptr)
  {
    //connection already exists
    DBG("Connection already exists");
    return nullptr;
  }

  NodeConnection * c = new NodeConnection(tSourceNode, tDestNode, connectionType);
  connections.add(c);
  c->addConnectionListener(this);
  //  updateAudioGraph();
  // DBG("Dispatch connection Added from NodeManager");
  nodeChangeNotifier.addMessage(new NodeChangeMessage(c,true));
//  nodeContainerListeners.call(&NodeContainerListener::connectiosnAdded, c);

  return c;
}


bool NodeContainer::removeConnection(NodeConnection * c)
{
  if (c == nullptr) return false;
  c->removeConnectionListener(this);

  connections.removeObject(c);
nodeChangeNotifier.addMessage(new NodeChangeMessage(c,false));
//  nodeContainerListeners.call(&NodeContainerListener::connectionRemoved, c);

  return true;
}



//From NodeBase Listener
void NodeContainer::askForRemoveNode(ConnectableNode * node)
{
  if(    removeNode(node)){
    // TODO we should give a pre_delete() function to nodes for unregistering all listener and stuffs,
    // for now re opening a complex session gives crash on the second open as they are not being deleted by us but the AudioGraph
    //        if(NodeBase *nb=dynamic_cast<NodeBase *>(node)){
    //         nb->stopTimer();
    //        }
  }
}

void NodeContainer::askForRemoveProxy(ParameterProxy * p)
{
  removeParamProxy(p);
}


void NodeContainer::askForRemoveConnection(NodeConnection *connection)
{
  removeConnection(connection);
}

void NodeContainer::RMSChanged(ConnectableNode * node, float _rmsInValue, float _rmsOutValue)
{
  if (node == containerInNode) rmsInValue = _rmsOutValue;
  else if (node == containerOutNode) rmsOutValue = _rmsInValue;

  rmsListeners.call(&ConnectableNode::RMSListener::RMSChanged, this, rmsInValue, rmsOutValue);
}

void NodeContainer::onContainerParameterChanged(Parameter * p)
{

    NodeBase::onContainerParameterChanged(p);

}




void NodeContainer::onContainerParameterChangedAsync(Parameter * p  ,const var & v) {
  NodeBase::onContainerParameterChangedAsync(p ,v);
    if (p == enabledParam)
    {
        
        triggerAsyncUpdate();
    }

};

void NodeContainer::bypassNode(bool /*bypass*/){}

ConnectableNodeUI * NodeContainer::createUI()
{
  return new NodeContainerUI(this);
}


bool NodeContainer::hasDataInputs()
{
  return containerInNode != nullptr ? containerInNode->hasDataInputs() : false;
}

bool NodeContainer::hasDataOutputs()
{
  return containerOutNode != nullptr ? containerOutNode->hasDataOutputs() : false;
}


void NodeContainer::numChannelsChanged(bool isInput){

  const ScopedLock lk(getCallbackLock());
  removeIllegalConnections();
  getAudioGraph()->setPlayConfigDetails(getTotalNumInputChannels(), getTotalNumOutputChannels(), getSampleRate(), getBlockSize());
  if(isInput && containerInNode)containerInNode->numChannels->setValue(getTotalNumInputChannels());//containerInNode->setPreferedNumAudioOutput(getTotalNumInputChannels());
  else if(!isInput && containerOutNode)containerOutNode->numChannels->setValue(getTotalNumOutputChannels());
}
void NodeContainer::prepareToPlay(double d, int i) {
  NodeBase::prepareToPlay(d, i);


  jassert(getSampleRate());
  jassert(getBlockSize());
  //    numChannelsChanged();
  {
    const ScopedLock lk(innerGraph->getCallbackLock());
    getAudioGraph()->setRateAndBufferSizeDetails(NodeBase::getSampleRate(),NodeBase::getBlockSize());
    getAudioGraph()->prepareToPlay(d, i);
  }




};




void NodeContainer::processBlockInternal(AudioBuffer<float>& buffer , MidiBuffer& midiMessage ) {
#ifdef MULTITHREADED_AUDIO
  if(!nodeManager){
    nodeManager = NodeManager::getInstance();
  }
  if(parentNodeContainer &&(nodeManager->getNumJobs()<4)){
    const ScopedLock lk(innerGraph->getCallbackLock());
    graphJob->setBuffersToReferTo(buffer, midiMessage);
    nodeManager->addJob(graphJob,false);
    while(nodeManager->contains(graphJob)){}
  }
  else{
    const ScopedLock lk(innerGraph->getCallbackLock());
    getAudioGraph()->processBlock(buffer,midiMessage);
  }
#else
  const ScopedLock lk(innerGraph->getCallbackLock());
  getAudioGraph()->processBlock(buffer,midiMessage);
#endif
};
void NodeContainer::processBlockBypassed(AudioBuffer<float>& /*buffer*/, MidiBuffer& /*midiMessages*/){
  //    getAudioGraph()->processBlockBypassed(buffer,midiMessages);
  
}



void NodeContainer::removeIllegalConnections() {
  getAudioGraph()->removeIllegalConnections();
  for(auto & c:nodeContainers){
    c->removeIllegalConnections();
  }
  
}
