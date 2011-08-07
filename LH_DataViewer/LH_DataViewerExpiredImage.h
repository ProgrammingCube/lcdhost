/**
  \file     LH_DataViewerExpiredImage.cpp
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

#ifndef LH_DATAVIEWEREXPIREDIMAGE_H
#define LH_DATAVIEWEREXPIREDIMAGE_H

#include <QtGlobal>

#include "../LH_QtInstance.h"
#include "../LH_Qt_QFileInfo.h"

#include "LH_DataViewerData.h"

class LH_DataViewerExpiredImage:public LH_QtInstance
{
    Q_OBJECT
    LH_DataViewerData data_;
    bool isExpired;

protected:
    LH_Qt_QFileInfo *setup_file_;

public:
    LH_DataViewerExpiredImage() : data_(this) {}
    const char *userInit();

    int polling();
    QImage *render_qimage( int w, int h );

    static lh_class *classInfo();

    QString getImageName();

public slots:
    void fileChanged();
    void updateImage(bool rerender = false);
};

#endif // LH_DATAVIEWEREXPIREDIMAGE_H
