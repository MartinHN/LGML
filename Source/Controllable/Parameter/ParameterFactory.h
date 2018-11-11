/* Copyright © Organic Orchestra, 2017
 *
 * This file is part of LGML.  LGML is a software to manipulate sound in realtime
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation (version 3 of the License).
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */


#pragma once

#include "../../Utils/FactoryBase.h"

class ParameterBase;
class  ParameterFactory: public FactoryBase<ParameterBase>
{
public:
    static void registerExtraTypes(){
        // legacy param
        jassert(getFactory().contains("t_NumericParameter_floatParamType"));
        getFactory().set("t_NumericParameter_double",getFactory()["t_NumericParameter_floatParamType"]);
    }
    //  default creation for simple types
    static ParameterBase* createBaseFromVar (StringRef name, const var&);
};


#define REGISTER_PARAM_TYPE(T) REGISTER_OBJ_TYPE(ParameterBase,T,#T)


#define REGISTER_PARAM_TYPE_TEMPLATED(T,TT,NICENAME) REGISTER_OBJ_TYPE_TEMPLATED(ParameterBase,T,TT,NICENAME)



