/**
  \file     LH_GraphNetOut.cpp
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

#include "LH_Graph.h"
#include "../LH_QtNetwork.h"

class LH_GraphNetOut : public LH_Graph
{
    LH_QtNetwork net_;
    int valCount;
    qreal valCache;
    qreal lastVal;

protected:
    LH_Qt_QStringList *setup_units_;

public:
    LH_GraphNetOut() : net_(this)
    {
        QStringList valuesList;
        valuesList.append("kb/s (kilobits per second)");
        valuesList.append("Mb/s (megabits per second)");
        valuesList.append("kB/s (kilobytes per second)");
        valuesList.append("MB/s (megabytes per second)");
        setup_units_ = new LH_Qt_QStringList(this,"Units",valuesList,LH_FLAG_AUTORENDER);
        valCount = 0;
        valCache = 0;
        lastVal = 0;

        setMin(0.0);
        setMax(1000);
        setYUnit("kb/s");

        net_.smoothingHidden(true);
    }
    ~LH_GraphNetOut()
    {}

    static lh_class *classInfo()
    {
        static lh_class classInfo =
        {
            sizeof(lh_class),
            "System/Network/Outbound",
            "SystemNetworkOutboundGraph",
            "Outbound Bandwidth Usage (Graph)",
            48,48,
            lh_object_calltable_NULL,
            lh_instance_calltable_NULL
        };

        return &classInfo;
    }

    int notify(int n, void *p)
    {
        changeUnits();
        if(!n || n&LH_NOTE_SECOND)
        {
            if (valCount!=0) {
                lastVal = valCache/valCount;
                setMax( state()->net_max_out);
                addValue(lastVal);
            }
            valCache = 0;
            valCount = 0;
        } else {
            valCache+=net_.outRate();
            valCount+=1;
        }
        return LH_Graph::notify(n,p) | net_.notify(n,p) | LH_NOTE_SECOND;
    }

    QImage *render_qimage( int w, int h )
    {
        if( LH_Graph::render_qimage(w,h) == NULL ) return NULL;
        drawSingle( );
        return image_;
    }

    void changeUnits()
    {
        switch(setup_units_->value())
        {
        case 0:
            setYUnit("kb/s", 1024 / 8);
            break;
        case 1:
            setYUnit("Mb/s", 1024 * 1024 / 8);
            break;
        case 2:
            setYUnit("kB/s", 1024);
            break;
        case 3:
            setYUnit("MB/s", 1024 * 1024);
            break;
        }
    }
};

LH_PLUGIN_CLASS(LH_GraphNetOut)
