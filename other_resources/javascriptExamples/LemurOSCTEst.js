/// LGML 
/// this is an example script for Javascript and osc controller



///access to LGML environment:
/// in any javascripted object we have two accessible namespaces :
/// "lgml" that represent the whole LGML environment and let you change about any parameters
/// "local" that is a shortcut to local object class 
/// example the send method of javascriptOSCController named myController can be accessed via local.send(args) or lgml.OSC.myController(args)


/// example of accessible functions from LGML

/// local code :
/// each object can define callback function for different types of events so the API is less fixed
///  control callback functions should start by onCtl....()
///  then corresponding object are adresses splited by "_" underscore
/// i.e /foo/bar 1 2 -> onCtl_foo_bar(arg1,arg2)

/// a special method onAnyMsg can react to anything and parse address in javascript
/// special functions
//// lgml.post : post a message in LGML Logger



/// lgml parameter Changes
/// any function (starting with on) that can be parsed to a valid lgml controllable address is called on corresponding controllable change
/// i.e on_time_play 


onCtl_AnyMsg = function (adress , args){
	lgml.post(adress+ " /  "+JSON.stringify(args));
};



