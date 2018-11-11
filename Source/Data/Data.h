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

#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#include "../JuceHeaderCore.h"//keep
class NodeBase;

class Data
{
public:
    enum IOType
    {
        Input, Output
    };

    enum DataType
    {
        Unknown, Float, Number, Boolean, Position, Orientation, Color
    };

    class DataElement
    {
    public:
        DataElement (String _name);

        String name;
        DataType type;

        float value;

        bool isTypeCompatible (const DataType& targetType);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DataElement)
    };



    String name;
    DataType type;
    IOType ioType;

    NodeBase* node;

    int numConnections;

    OwnedArray<DataElement> elements;

    Data (NodeBase* node, String _name, DataType _type, IOType _ioType);
    ~Data();

    void addElement (const String& _name);

    DataElement* getElement (const String& elementName);

    void updateFromSourceData (Data* sourceData);

    void update (const float& value1, const float& value2 = 0, const float& value3 = 0);

    bool isComplex() { return elements.size() > 1; }

    bool isTypeCompatible (const DataType& targetType);

    int getNumElementsForType (const DataType& _type);

    String getTypeString();

    class  DataListener
    {
    public:
        /** Destructor. */
        virtual ~DataListener() {}

        virtual void dataChanged (Data*) = 0;
    };

    ListenerList<DataListener> listeners;
    void addDataListener (DataListener* newListener) { listeners.add (newListener); }
    void removeDataListener (DataListener* listener) { listeners.remove (listener); }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Data)
};


#endif  // DATA_H_INCLUDED
