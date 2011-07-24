/**
  \file     LH_DialNetIn.cpp
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

#include "LH_Dial.h"
#include "../LH_QtNetwork.h"

class LH_DialNetIn : public LH_Dial
{
    LH_QtNetwork net_;

public:
    LH_DialNetIn() : LH_Dial(), net_(this) {}

    const char *userInit()
    {
        if( const char *err = LH_Dial::userInit() ) return err;
        setMin(0.0);
        setMax(1000.0);
        return 0;
    }

    static lh_class *classInfo()
    {
        static lh_class classInfo =
        {
            sizeof(lh_class),
            "System/Network/Inbound",
            "SystemNetworkInboundDial",
            "Inbound Bandwidth Usage (Dial)",
            48,48,
            lh_object_calltable_NULL,
            lh_instance_calltable_NULL
        };

        return &classInfo;
    }

    int notify(int n, void *p)
    {
        setVal( net_.inPermille() );
        return net_.notify(n,p);
    }
};

LH_PLUGIN_CLASS(LH_DialNetIn)
