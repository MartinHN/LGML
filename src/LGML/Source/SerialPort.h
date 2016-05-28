/*
  ==============================================================================

    SerialPort.h
    Created: 25 May 2016 4:47:22pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SERIALPORT_H_INCLUDED
#define SERIALPORT_H_INCLUDED

#include "JuceHeader.h"

#include "serial/serial.h"
#include "QueuedNotifier.h"

using namespace serial;

class SerialPort;

class SerialReadThread :
	public Thread
{
public:

	SerialReadThread(String name, SerialPort * _port);
	virtual ~SerialReadThread();

	SerialPort * port;

	virtual void run() override;

	// ASYNC
	QueuedNotifier<var> queuedNotifier;
	typedef QueuedNotifier<var>::Listener AsyncListener;

	void addAsyncSerialListener(AsyncListener* newListener) { queuedNotifier.addListener(newListener); }
	//void addAsyncCoalescedListener(AsyncListener* newListener) { queuedNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncSerialListener(AsyncListener* listener) { queuedNotifier.removeListener(listener); }

};

class SerialPortInfo
{
public:
	SerialPortInfo(String _port, String _description, String _hardwareID) :
		port(_port), description(_description), hardwareID(_hardwareID)
	{}

	virtual ~SerialPortInfo() {}

	String port;
	String description;
	String hardwareID;


};

class SerialPort : public SerialReadThread::AsyncListener
{
public:
	SerialReadThread thread;

	enum PortMode { LINES, DATA255, RAW, COBS };

	SerialPort(Serial *port, SerialPortInfo * info, PortMode mode = LINES);
	virtual ~SerialPort();

	SerialPortInfo * info;
	ScopedPointer<Serial> port;
	PortMode mode;

	void open();
	void close();

	bool isOpen();

	//write functions
	int writeString(String message, bool endLine = true);
	int writeBytes(Array<uint8_t> data);

	virtual void newMessage(const var &data) override;;

	class SerialPortListener
	{
	public:
		virtual ~SerialPortListener() {}

		//serial data here
		virtual void portOpened(SerialPort  *) {};
		virtual void portClosed(SerialPort  *) {};
		virtual void portRemoved(SerialPort *) {};
		virtual void serialDataReceived(const var &) {};
	};

	ListenerList<SerialPortListener> listeners;
	void addSerialPortListener(SerialPortListener* newListener);
	void removeSerialPortListener(SerialPortListener* listener);
};




#endif  // SERIALPORT_H_INCLUDED