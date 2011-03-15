/**
  \file     LH_CoreTempDial.cpp
  \author   Andy Bridges <triscopic@codeleap.co.uk>
  \legalese
    This module is based on original work by Johan Lindh.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.

  */

#include "LH_CoreTempDial.h"

lh_class *LH_CoreTempDial::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "3rdParty/Monitoring/CoreTemp",
        "CoreTempDial",
        "CoreTemp Dial",
        48,48,
        lh_instance_calltable_NULL
    };

    return &classInfo;
}

LH_CoreTempDial::LH_CoreTempDial( const char *name, LH_QtPlugin *parent = 0 ) : LH_MonitoringDial( name, parent )
{
    data_ = new LH_CoreTempData( this, mdmNumbers, true );
    connect_refresh( static_cast<LH_CoreTempData*>(data_)->setup_value_type_ );
    connect_refresh( static_cast<LH_CoreTempData*>(data_)->setup_value_index_ );
}

LH_PLUGIN_CLASS(LH_CoreTempDial)