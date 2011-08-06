/**
  \file     LH_Text.cpp
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

#include <QtGlobal>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QRect>
#include <QFont>
#include <QFontMetrics>
#include <QTime>
#include <QUrl>

#include "LH_Text.h"

static inline uint PREMUL(uint x) {
    uint a = x >> 24;
    uint t = (x & 0xff00ff) * a;
    t = (t + ((t >> 8) & 0xff00ff) + 0x800080) >> 8;
    t &= 0xff00ff;

    x = ((x >> 8) & 0xff) * a;
    x = (x + ((x >> 8) & 0xff) + 0x80);
    x &= 0xff00;
    x |= t | (a << 24);
    return x;
}

const char *LH_Text::userInit()
{
    if( const char *err = LH_QtCFInstance::userInit() ) return err;

    richtext_ = false;

    setup_text_ = new LH_Qt_QString( this, ("Text"), QString(), LH_FLAG_FOCUS|LH_FLAG_AUTORENDER );
    setup_text_->setOrder(-2);
    setup_text_->setHelp( "<p>The displayed text. Note that this supports "
                          "a limited HTML subset, including images and tables.</p>");
    connect( setup_text_, SIGNAL(changed()), this, SLOT(textChanged()) );

    setup_font_ = new LH_Qt_QFont( this, ("Font"), QFont("Arial",10), LH_FLAG_AUTORENDER );
    setup_font_->setHelp( "<p>Font used to display the text.</p>");
    connect( setup_font_, SIGNAL(changed()), this, SLOT(fontChanged()) );

    setup_fontresize_ = new LH_Qt_bool( this, ("Adjust font size to instance height"), true, LH_FLAG_AUTORENDER | LH_FLAG_BLANKTITLE );
    setup_fontresize_->setHelp( "<p>Rather than using a fixed font size, "
                          "LCDHost will adjust the font height to "
                          "match the instance height.</p>"
                          );
    connect( setup_fontresize_, SIGNAL(changed()), this, SLOT(fontChanged()) );

    setup_pencolor_ = new LH_Qt_QColor( this, ("Color"), Qt::black, LH_FLAG_AUTORENDER );
    setup_pencolor_->setHelp( "<p>The color used to write the text.</p>");
    connect( setup_pencolor_, SIGNAL(changed()), this, SLOT(makeTextImage()) );

    setup_bgcolor_ = new LH_Qt_QColor( this, ("Background"), Qt::transparent, LH_FLAG_AUTORENDER );
    setup_bgcolor_->setHelp( "<p>The color for the background.</p>");
    connect( setup_bgcolor_, SIGNAL(changed()), this, SLOT(makeTextImage()) );

    setup_horizontal_ = new LH_Qt_QStringList( this, ("Horizontal"), QStringList() << ("Center") << ("Left")
                                               << ("Right") << ("Scroll (Marquee)") << ("Reverse scroll (Marquee)")
                                               << ("Scroll (Loop)") << ("Reverse scroll (Loop)")
                                              , LH_FLAG_AUTORENDER );
    setup_horizontal_->setHelp( "<p>How to handle the text if it is wider than "
                                "the available horizontal area. You can either adjust "
                                "the text to the left, center or right, or you can "
                                "have LCDHost scroll it from right to left or "
                                "in the reverse direction, left to right.</p>");
    connect( setup_horizontal_, SIGNAL(changed()), this, SLOT(requestPolling()) );

    setup_vertical_ = new LH_Qt_QStringList( this, ("Vertical"), QStringList() << ("Center") << ("Top")
                                             << ("Bottom") << ("Scroll (Marquee)") << ("Reverse scroll (Marquee)")
                                            << ("Scroll (Loop)") << ("Reverse scroll (Loop)"), LH_FLAG_AUTORENDER );
    setup_vertical_->setHelp( "<p>How to handle the text if it is taller than "
                                "the available vertical area. You can either adjust "
                                "the text to the top, center or bottom, or you can "
                                "have LCDHost scroll it from bottom to top or "
                                "in the reverse direction, top to bottom.</p>");
    connect( setup_vertical_, SIGNAL(changed()), this, SLOT(requestPolling()) );

    setup_scrollrate_ = new LH_Qt_QSlider( this, ("Scroll rate"), 15, 1, 30, LH_FLAG_AUTORENDER );
    setup_scrollrate_->setHelp( "<p>How often the text is scrolled.</p>");
    connect( setup_scrollrate_, SIGNAL(changed()), this, SLOT(requestPolling()) );

    setup_scrollstep_ = new LH_Qt_QSlider( this, ("Scroll step"), 2, 1, 20, LH_FLAG_AUTORENDER );
    setup_scrollstep_->setHelp( "<p>How large each scroll step is.</p>");

    setup_scrollgap_ = new LH_Qt_QSlider( this, ("Scroll gap"), 10, 0, 340, LH_FLAG_AUTORENDER );
    setup_scrollgap_->setHelp( "<p>How large a gap between \"loop\" scrolling text.</p>");

    scrollposx_ = scrollposy_ = 0;

    add_cf_source(setup_text_);
    add_cf_target(setup_pencolor_);
    add_cf_target(setup_bgcolor_);
    add_cf_target(setup_font_);

    font_ = setup_font_->value();
    textimage_ = makeImage();
    setText("LCDHost");
    return 0;
}

void LH_Text::setRenderHints( QPainter& p )
{
    if( isMonochrome() )
    {
        p.setRenderHint( QPainter::Antialiasing, false );
        p.setRenderHint( QPainter::TextAntialiasing, false );
    }
    else
    {
        p.setRenderHint( QPainter::Antialiasing, true );
        p.setRenderHint( QPainter::TextAntialiasing, true );
    }
}

/**
  Create the QImage that contains the text. This may be larger or smaller than the
  target rendering area, and the \c setup_horizontal_ and \c setup_vertical_ values
  will determine how to position and scroll the text within the rendering area.

  This function will respect fontresize() and make sure that the resulting image
  matches \c forheight, or if that's less than 4, the image size, or if there's no
  image, the current text image height.
  */
void LH_Text::makeTextImage( int forheight )
{
    QPainter painter;
    int flags = Qt::AlignTop|Qt::AlignLeft|Qt::TextSingleLine|Qt::TextIncludeTrailingSpaces;

    // make sure forheight is reasonable if given
    if( forheight < 0 ) forheight = 0;
    if( forheight && forheight < 4 )
    {
        if( image_ ) forheight = image_->height();
        else forheight = textimage_.height();
    }

    // select a reasonable font size based on pixel height
    if( fontresize() )
    {
        int targetsize;
        if( isMonochrome() ) targetsize = qMax(forheight,7);
        else targetsize = qMax(forheight,20);
        font_.setPixelSize( targetsize );
        QFontMetrics fm( font_, &textimage_ );
        font_.setPixelSize( targetsize - fm.descent() );
    }

    // Set font antialiasing strategy
    int strat = font_.styleStrategy();
    if( isMonochrome() )
    {
        strat &= ~QFont::PreferAntialias;
        strat |= QFont::NoAntialias;
    }
    else
    {
        strat &= ~QFont::NoAntialias;
        strat |= QFont::PreferAntialias;
    }
    font_.setStyleStrategy( (QFont::StyleStrategy) strat );

    if( richtext_ )
    {
        forheight = 0;
        doc_.setDefaultFont( font_ );
        doc_.setDefaultStyleSheet(
                QString("body { background-color:%1 }")
                .arg( setup_bgcolor_->value().name() )
                );
    }

    // Can't do measurements against a null image
    if( textimage_.isNull() )
        textimage_ = QImage(1,1,QImage::Format_ARGB32_Premultiplied);

    if( painter.begin( &textimage_ ) )
    {
        setRenderHints( painter );
        painter.setFont( font_ );
        if( richtext_ ) textsize_ = doc_.size();
        else
        {
            textsize_ = painter.boundingRect( QRect(), flags, text() ).size();
#ifdef Q_WS_MAC
            // Until the Qt team fixes QTBUG-13561
            // http://bugreports.qt.nokia.com/browse/QTBUG-13561
            QFontMetrics fm( font(), &textimage_ );
            textsize_.rheight() += fm.descent();
            textsize_.rwidth() ++;
#endif
        }
        painter.end();
    }
    else Q_ASSERT(!"LH_Text: painter.begin() failed 1");

    if( textimage_.size() != textsize_ )
    {
        textimage_ = makeImage(textsize_.width(), textsize_.height());
        textimage_.setDotsPerMeterX(3779);
        textimage_.setDotsPerMeterY(3779);
    }

    if( !textimage_.isNull() && !textimage_.size().isEmpty() )
    {
        textimage_.fill( qRgba(0,0,0,0) );
        if( pencolor().alpha() == 255 )
        {
            // Opaque pen, take a shortcut
            if( painter.begin( &textimage_ ) )
            {
                setRenderHints( painter );
                painter.setFont( font() );
                painter.setPen( pencolor() );
                painter.setCompositionMode( QPainter::CompositionMode_SourceOver );
                if( richtext_ )
                {
                    doc_.drawContents( &painter );
                }
                else painter.drawText( textimage_.rect(), flags, text() );
                painter.end();
            }
            else Q_ASSERT(!"LH_Text: painter.begin() failed 2");
        }
        else
        {
            QImage mask = makeImage( textimage_.size() );
            QImage fg = makeImage( textimage_.size() );

            mask.fill( qRgba(0,0,0,0) );
            if( painter.begin( &mask ) )
            {
                setRenderHints( painter );
                painter.setFont( font() );
                painter.setPen( Qt::black );
                painter.setCompositionMode( QPainter::CompositionMode_SourceOver );
                if( richtext_ ) doc_.drawContents( &painter );
                else painter.drawText( textimage_.rect(), flags, text() );
                painter.end();
            }
            else Q_ASSERT(!"LH_Text: painter.begin() failed 3");

            fg.fill( PREMUL(pencolor().rgba()) );
            if( painter.begin( &fg ) )
            {
                painter.setCompositionMode( QPainter::CompositionMode_DestinationIn );
                painter.drawImage( 0,0, mask );
                painter.end();
            }
            else Q_ASSERT(!"LH_Text: painter.begin() failed 4");

            if( painter.begin( &textimage_ ) )
            {
                painter.setCompositionMode( QPainter::CompositionMode_DestinationOut );
                painter.drawImage( 0,0, mask );
                painter.setCompositionMode( QPainter::CompositionMode_Plus );
                painter.drawImage( 0,0, fg );
                painter.end();
            }
            else Q_ASSERT(!"LH_Text: painter.begin() failed 5");
        }
    }

    // If forheight was given, ensure that height
    if( forheight && forheight != textimage_.height() )
    {
        if( forheight < (textimage_.height()-3) && !isMonochrome() )
        {
            // scale the text image if higher by more than 3 pixels
            textimage_ = textimage_.scaledToHeight( forheight, Qt::SmoothTransformation );
        }
        else
        {
            // center the text image
            int dh = textimage_.height() - forheight;
            textimage_ = textimage_.copy( 0, dh/2, textimage_.width(), forheight );
        }
    }
}

void LH_Text::fontChanged()
{
    int h = image_ ? image_->height() : textimage_.height();
    font_ = QFont( setup_font_->value(), &textimage_ );
    makeTextImage( fontresize() ? h : 0 );
}

void LH_Text::textChanged()
{
    if( image_ )
    {
        delete image_;
        image_ = NULL;
    }
    richtext_ = Qt::mightBeRichText( setup_text_->value() );
    if( richtext_)
    {
        doc_.setDocumentMargin( 0 );
        doc_.setIndentWidth( 20 );
        doc_.setMetaInformation(
                    QTextDocument::DocumentUrl,
                    QUrl::fromLocalFile(layoutPath()).toString()
                    );
        doc_.setHtml( setup_text_->value() );
        setup_fontresize_->setValue( false );
        setup_fontresize_->setFlag( LH_FLAG_READONLY|LH_FLAG_HIDDEN, true );
        setup_pencolor_->setFlag( LH_FLAG_HIDDEN, true );
    }
    else
    {
        setup_fontresize_->setFlag( LH_FLAG_READONLY|LH_FLAG_HIDDEN, false );
        setup_pencolor_->setFlag( LH_FLAG_HIDDEN, false );
    }
    makeTextImage();
}

bool LH_Text::setText( QString newText )
{
    if( newText == setup_text_->value() ) return false;
    setup_text_->setValue( newText );
    textChanged();
    return true;
}

bool LH_Text::setNum( qreal value, int mode, bool suffix, qreal max, bool bytes )
{
    QString text;
    qreal tera, giga, mega, kilo;
    QChar suffix_char;
    int prec = 1;

    if( bytes ) kilo = 1024.0;
    else kilo = 1000.0;

    mega = kilo * kilo;
    giga = mega * kilo;
    tera = giga * kilo;

    if( mode == Automatic )
    {
        if( value >= tera ) mode = Tera;
        else if( value >= giga ) mode = Giga;
        else if( value >= mega ) mode = Mega;
        else if( value >= kilo ) mode = Kilo;
        else mode = Unscaled;
    }

    switch( mode )
    {
    case Automatic:
        Q_ASSERT(0);
        break;
    case Percentage:
        if( qFuzzyCompare( max, 0.0 ) )
            text = "0";
        else
        {
            qreal pct;
            if( max < 0.0 ) pct = (-value * 100.0) / -max;
            else pct = value * 100.0 / max;
            if( pct >= 10.0 ) text.setNum( pct, 'f', 0 );
            else text.setNum( pct, 'f', 1 );
        }
        suffix_char = '%';
        break;
    case Unscaled:
        if( qFuzzyCompare( value, qRound64(value) ) )
            text.setNum( value, 'f', 0 );
        else
            text.setNum( value, 'g' );
        break;
    case Kilo:
        if( value >= 10.0 * kilo ) prec = 0;
        text.setNum( value / kilo, 'f', prec );
        suffix_char = 'K';
        break;
    case Mega:
        if( value >= 10.0 * mega ) prec = 0;
        text.setNum( value / mega, 'f', prec );
        suffix_char = 'M';
        break;
    case Giga:
        if( value >= 10.0 * giga ) prec = 0;
        text.setNum( value / giga, 'f', prec );
        suffix_char = 'G';
        break;
    case Tera:
        if( value >= 10.0 * tera ) prec = 0;
        text.setNum( value / tera, 'f', prec );
        suffix_char = 'T';
        break;
    }

    if( suffix && !suffix_char.isNull() ) text.append( suffix_char );
    return setText( text );
}

/**
  polling() should return the number of milliseconds to wait before polling()
  gets called again, or zero to not have polling() called again.
  In this class, we don't need polling, as our image doesn't change over time.
  A typical use of this would be to request a rendering of ourselves at
  non-regular intervals. If you just need to check stuff every second or so,
  use notify(), which is more efficient.

  \return
    Number of milliseconds that LCDHost waits before calling polling() again
  */
int LH_Text::polling()
{
    if( (horizontal() < 3 && vertical() < 3) ) return 0;

    if( image_ )
    {
        int mody =  textimage_.height() + scrollgap();
        int ymax = (mody + image_->height() );
        if( vertical() >= 5 )
            ymax = ( (image_->height() / mody)+1 ) * mody;

        int modx =  textimage_.width() + scrollgap();
        int xmax = (modx + image_->width());
        if( horizontal() >= 5 )
            xmax = ( (image_->width()  / modx)+1 ) * modx;

        if( horizontal() >= 3 )
        {
            scrollposx_ += scrollstep();
            if( scrollposx_ > xmax )
            {
                scrollposx_ = 0;
                if( vertical() >= 3 )
                {
                    scrollposy_ += scrollstep();
                }
            }
        }
        else if( vertical() >= 3 )
        {
            scrollposy_ += scrollstep();
        }

        if( scrollposy_ > ymax)
        {
            scrollposy_ = 0;
        }

        requestRender();
    }

    int delay = QTime::currentTime().msec() % scrollrate();
    if( delay < 20 ) delay += (1000/scrollrate());
    return delay;
}

/**
  notify() is the preferred method to do things with regular intervals, or
  when some system metric (such as CPU load) has been updated. See LH_TextTime
  for a sample that uses LH_NOTE_SECOND.

  \param code
    Zero or more of the LH_NOTE_xxx defines OR'ed together. You will also
    get a zero for code when LCDHost initially creates the instance, or when
    you request a \c notify() call using \c lh_cb_notify.

  \param param
    Reserved for future usage.

  \return
    A bitwise-OR mask of the events we're interested in, or zero to receive
    no events.
  */
int LH_Text::notify(int code,void* param)
{
    Q_UNUSED(param);
    if( code & LH_NOTE_DEVICE )
    {
        makeTextImage();
        requestRender();
    }
    return LH_NOTE_DEVICE | LH_QtCFInstance::notify(code,param);;
}

/**
  prerender() is called during a rendering operation, before calling
  \sa width(), \sa height(), or one of the rendering functions.
  It's a good place to make sure that what you're about to render
  is up-to-date.
  */
void LH_Text::prerender()
{
    return;
}

/**
  width() is called by LCDHost to find out our preferred width,
  given a specific height. If \p forHeight is negative, then
  LCDHost wants to know the standard width given a standard height.
  That means the plugin is being asked to provide both width and
  height, and the user has no preference.

  \param forHeight
    Height, in pixels, to provide the wanted width for, or negative.
  \return
    Recommended width, in pixels.
  */
int LH_Text::width( int forHeight )
{
    Q_UNUSED(forHeight);

    if( fontresize() && textimage_.height() != forHeight )
        makeTextImage( forHeight );

    return textimage_.width();
}

/**
  height() is called by LCDHost to find out our preferred height,
  given a specific width. If \p forWidth is negative, then
  LCDHost wants to know the standard height given a standard width.
  That means the plugin is being asked to provide both width and
  height, and the user has no preference.

  \param forWidth
    Width, in pixels, to provide the wanted height for, or negative.
  \return
    Recommended height, in pixels.
  */
int LH_Text::height( int forWidth )
{
    Q_UNUSED(forWidth);
    return textimage_.height();
}

/**
  Internal function to let subclasses who do their own
  rendering have an easier time.
  */
bool LH_Text::prepareForRender(int w, int h)
{
    if( (image_ = initImage(w,h)) == NULL ) return false;
    image_->fill( PREMUL( bgcolor().rgba() ) );

    if( richtext_ )
    {
        if( w != textimage_.width() )
        {
            doc_.setTextWidth( w );
            makeTextImage();
        }
    }
    else if( fontresize() && textimage_.height() != h )
    {
        makeTextImage( h );
    }

    return true;
}

/**
  render_qimage() is the function to use if you're using Qt to render.
  In this plugin, that's what we're doing, so we implement it.
  Return a pointer to the QImage rendered, or NULL if we can't make a
  rendering (for whatever reason).

  If you return NULL, LCDHost will remember the last image size you
  did, and show a placeholder image on the editing view. In the
  LCD output, nothing will be shown on a NULL return.

  \param w Width, in pixels, of the image to render
  \param h Height, in pixels, of the image to render
  \return A pointer to the image, or NULL
  */
QImage *LH_Text::render_qimage(int w, int h)
{
    if( !prepareForRender(w,h) ) return NULL;

    QPainter painter;
    if( painter.begin(image_) )
    {
        QRectF target;
        QRectF source = textimage_.rect();

        target.setSize( textimage_.size() );

        switch( horizontal() )
        {
        case 0: // center
            target.moveLeft( image_->width()/2 - textimage_.width()/2 );
            break;
        case 1: // left
            target.moveLeft( 0 );
            break;
        case 2: // right
            target.moveLeft( image_->width() - textimage_.width() );
            break;
        case 3: // scroll
        case 5: // scroll
            target.moveLeft( image_->width() - scrollposx_ );
            break;
        case 4: // reverse scroll
        case 6: // reverse scroll
            target.moveLeft( scrollposx_ - textimage_.width() );
            break;
        }

        switch( vertical() )
        {
        case 0: // center
            target.moveTop( image_->height()/2 - textimage_.height()/2 );
            break;
        case 1: // left
            target.moveTop( 0 );
            break;
        case 2: // right
            target.moveTop( image_->height() - textimage_.height() );
            break;
        case 3: // scroll
        case 5: // scroll
            target.moveTop( image_->height() - scrollposy_ );
            break;
        case 4: // reverse scroll
        case 6: // reverse scroll
            target.moveTop( scrollposy_ - textimage_.height() );
            break;
        }

        painter.drawImage( target, textimage_, source );

        if(horizontal()>=3)
        {
            if(horizontal()>=5)
            {
                int pos;
                int mod =  textimage_.width() + scrollgap();

                pos = ( horizontal()==5? image_->width() - scrollposx_ : scrollposx_ - textimage_.width());
                while(pos < image_->width())
                {
                    pos += mod;
                    target.moveLeft( pos );
                    painter.drawImage( target, textimage_, source );
                }

                pos = ( horizontal()==5? image_->width() - scrollposx_ : scrollposx_ - textimage_.width() );
                while(pos > 0)
                {
                    pos -= mod;
                    target.moveLeft( pos );
                    painter.drawImage( target, textimage_, source );
                }
            }
        } else
            if(vertical()>=5)
            {
                int pos;
                int mod =  textimage_.height() + scrollgap();
                pos = ( vertical()==5? image_->height() - scrollposy_ : scrollposy_ - textimage_.height() );
                while(pos < image_->height())
                {
                    pos += mod;
                    target.moveTop( pos );
                    painter.drawImage( target, textimage_, source );
                }

                pos = ( vertical()==5? image_->height() - scrollposy_ : scrollposy_ - textimage_.height() );
                while(pos > 0)
                {
                    pos -= mod;
                    target.moveTop( pos );
                    painter.drawImage( target, textimage_, source );
                }
            }

        painter.end();
    }

    return image_;
}
