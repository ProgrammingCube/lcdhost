/**
  \file     LH_QtPlugin_Dial.h
  \author   Andy Bridges <andy@bridgesuk.com>
  \legalese Copyright (c) 2011 Andy Bridges

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

#ifndef LH_QTPLUGIN_DIAL_H
#define LH_QTPLUGIN_DIAL_H

#include "../LH_QtPlugin.h"

#define VERSION 1.03

class LH_QtPlugin_Dial : public LH_QtPlugin
{
    Q_OBJECT

public:
    LH_QtPlugin_Dial() : LH_QtPlugin() {}

    const char * lh_name() { return "Dial"; }
    const char * lh_shortdesc() { return "Displays dials"; }
    const char * lh_author() { return "Andy \"Triscopic\" Bridges"; }
    const char * lh_homepage() { return "<a href=\"http://www.codeleap.co.uk\">CodeLeap</a>"; }
    const char * lh_longdesc()
    {
        return  "<p>Displays data items such as CPU, memory and network load in the form of dials. </p>";
    }
    const char *lh_load();
    void lh_unload();
};

#endif // LH_QTPLUGIN_DIAL_H
