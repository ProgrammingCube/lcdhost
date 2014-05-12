/**
  \file     LH_Rectangle.cpp
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2009-2010 Johan Lindh

  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of Link Data Stockholm nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.


  */

#include <QDebug>
#include <QPainter>

#include "LH_Rectangle.h"
#include "LH_QtCFInstance.h"

LH_Rectangle::LH_Rectangle(LH_QtObject* parent)
    : LH_QtCFInstance(parent)
    , setup_penwidth_(0)
    , setup_rounding_(0)
    , setup_pencolor_(0)
    , setup_bgcolor1_(0)
    , setup_bgcolor2_(0)
    , setup_gradient_(0)
    , setup_horizontal_(0)
{
}

const char *LH_Rectangle::userInit() {
    if( const char *err = LH_QtCFInstance::userInit() ) return err;
    setup_penwidth_ = new LH_Qt_QSlider(this,tr("Pen width"),0,0,1000,LH_FLAG_AUTORENDER);
    setup_rounding_ = new LH_Qt_QSlider(this,tr("Corner rounding"),20,0,100,LH_FLAG_AUTORENDER);
    setup_pencolor_ = new LH_Qt_QColor(this,tr("Pen color"),Qt::black,LH_FLAG_AUTORENDER);
    setup_bgcolor1_ = new LH_Qt_QColor(this,tr("Fill color 1"),Qt::white,LH_FLAG_AUTORENDER);
    setup_gradient_ = new LH_Qt_bool(this,tr("^Background is a gradient"),false);
    setup_bgcolor2_ = new LH_Qt_QColor(this,tr("Fill color 2"),Qt::lightGray,LH_FLAG_HIDDEN|LH_FLAG_AUTORENDER);
    setup_horizontal_ = new LH_Qt_bool(this,tr("^Gradient is horizontal"),false,LH_FLAG_HIDDEN|LH_FLAG_AUTORENDER);
    connect( setup_gradient_, SIGNAL(change(bool)), this, SLOT(enableGradient(bool)) );
    return NULL;
}

void LH_Rectangle::enableGradient(bool b)
{
    setup_bgcolor2_->setFlag( LH_FLAG_HIDDEN, !b );
    setup_horizontal_->setFlag( LH_FLAG_HIDDEN, !b );
    requestRender();
}

QImage *LH_Rectangle::render_qimage( int w, int h )
{
    qreal minside;
    qreal rounding;
    qreal penpixels;

    QImage *img = initImage(w,h);
    if(img == 0)
        return 0;

    img->fill( qRgba(0,0,0,0) );

    minside = qMin( img->width(), img->height() ) / 2;
    rounding = ( minside * (qreal) this->rounding() ) / 100.0;

    QPainter painter;
    if( painter.begin( img ) )
    {
        QRectF rect = img->rect();

        if( state()->dev_depth == 1 )
        {
            painter.setRenderHint( QPainter::Antialiasing, false );
            painter.setRenderHint( QPainter::TextAntialiasing, false );
        }
        else
        {
            painter.setRenderHint( QPainter::Antialiasing, true );
            painter.setRenderHint( QPainter::TextAntialiasing, true );
        }

        if( penwidth() )
        {
            QPen pen( pencolor() );
            penpixels = (minside * (qreal) penwidth()) / 1000.0;
            pen.setWidthF( penpixels );
            rect.adjust(+penpixels/2,+penpixels/2,-penpixels/2,-penpixels/2);
            painter.setPen( pen );
        }
        else
        {
            painter.setPen( Qt::NoPen );
            penpixels = 0.0;
        }

        if( gradient() )
        {
            QLinearGradient gradient;
            gradient.setStart(0,0);
            if( horizontal() ) gradient.setFinalStop(img->width(),0);
            else gradient.setFinalStop(0,img->height());
            gradient.setColorAt(0, bgcolor1() );
            gradient.setColorAt(1, bgcolor2() );
            QBrush brush(gradient);
            painter.setBrush( brush );
        }
        else
            painter.setBrush( bgcolor1() );

#if 0
        // There's a bug in Qt 4.5.3 which makes drawRoundedRect fail
        // in the upper-left quadrant. That quadrant becomes too 'small'
        // compared to the others. The simple workaround is to use a
        // QPainterPath with 'addRoundedRect' and then just draw
        // that path instead.
        // see http://bugreports.qt.nokia.com/browse/QTBUG-6073
        QPainterPath path;
        path.addRoundedRect( rect, rounding, rounding, Qt::AbsoluteSize );
        painter.drawPath( path );
#else
        painter.drawRoundedRect( rect, rounding, rounding, Qt::AbsoluteSize );
#endif

        rounding += penpixels;
        qreal dx = (rect.left() + 0.31 * rounding);
        qreal dy = (rect.top() + 0.31 * rounding);
        if( dx >= img->width()/2 ) dx = (img->width()/2)-1;
        if( dy >= img->height()/2 ) dy = (img->height()/2)-1;
        if( dx < 0 ) dx = 0;
        if( dy < 0 ) dy = 0;

        painter.end();
    }

    return img;
}

int LH_Rectangle::notify( int code,void* param )
{
    if( !code || code&LH_NOTE_DEVICE ) requestRender();
    return LH_QtCFInstance::notify(code,param) | LH_NOTE_DEVICE;
}
