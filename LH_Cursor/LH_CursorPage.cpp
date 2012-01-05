/**
  \file     LH_CursorPage.cpp
  \author   Andy Bridges <triscopic@codeleap.co.uk>
  \legalese Copyright (c) 2010,2011 Andy Bridges
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

 **/

#include "LH_CursorPage.h"

LH_PLUGIN_CLASS(LH_CursorPage)

lh_class *LH_CursorPage::classInfo()
{
    static lh_class classinfo =
    {
        sizeof(lh_class),
        "Cursor",
        "CursorPage",
        "Cursor Page",
        -1, -1,
    };

    return &classinfo;
}

const char *LH_CursorPage::userInit()
{
    if( const char *err = LH_QtInstance::userInit() ) return err;
    hide();
    rcvr_ = new LH_CursorReceiver(this, SLOT(stateChangeAction(bool,bool)));
    return 0;
}

QImage *LH_CursorPage::render_qimage(int w, int h)
{
    delete image_;
    uchar *data = new uchar[4];
    data[0] = 255;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
    image_ = new QImage(data,1,1,QImage::Format_ARGB32);
    return image_;
}
void LH_CursorPage::stateChangeAction(bool newSelected, bool newActive)
{
    Q_UNUSED(newActive);
    setVisible(newSelected);
}
