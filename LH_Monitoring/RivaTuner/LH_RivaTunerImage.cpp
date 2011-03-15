/**
  \file     LH_RivaTunerImage.cpp
  \author   Andy Bridges <triscopic@codeleap.co.uk>
  \legalese Copyright (c) 2010 Andy Bridges
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

#include <QtGlobal>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QStringList>
#include <QString>
#include <QRegExp>
#include <QHash>

#include "LH_RivaTunerImage.h"
#include <windows.h>
#include "../LH_Qt_QStringList.h"

LH_PLUGIN_CLASS(LH_RivaTunerImage)

lh_class *LH_RivaTunerImage::classInfo()
{
    static lh_class classinfo =
    {
        sizeof(lh_class),
        "3rdParty/Monitoring/RivaTuner",
        "RivaTunerStatusImage",
        "RivaTuner Status Image",
        -1, -1,
        lh_instance_calltable_NULL
    };

    return &classinfo;
}

LH_RivaTunerImage::LH_RivaTunerImage( const char *name, LH_QtPlugin *parent ) : LH_MonitoringImage( name, parent )
{
    data_ = new LH_RivaTunerData(this);
    connect_changeType( static_cast<LH_RivaTunerData*>(data_)->setup_value_type_ );
}