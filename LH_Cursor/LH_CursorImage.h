/**
  \file     LH_CursorImage.h
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

#ifndef LH_CURSORIMAGE_H
#define LH_CURSORIMAGE_H

#include <QtGlobal>
#include <QHash>

#include "../LH_Monitoring/LH_QImage/LH_QImage.h"
#include "../LH_Qt_QString.h"
#include "../LH_Qt_QFileInfo.h"

#include "LH_CursorData.h"

class LH_CursorImage : public LH_QImage
{
    Q_OBJECT

    bool updateState();

protected:
    QHash<QString, QStringList> *imageDefinitions;

    LH_Qt_QString *setup_coordinate_;
    LH_Qt_QString *setup_cursor_state_;

public:
    LH_CursorImage();
    const char *userInit();

    int polling();


    static lh_class *classInfo();

};

#endif // LH_CURSORIMAGE_H
