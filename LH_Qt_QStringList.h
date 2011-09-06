/**
  \file     LH_Qt_QStringList.h
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2009-2011, Johan Lindh

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

#ifndef LH_QT_QSTRINGLIST_H
#define LH_QT_QSTRINGLIST_H

#include <QStringList>
#include "LH_QtSetupItem.h"

class LH_Qt_QStringList : public LH_QtSetupItem
{
    QStringList list_;
    QByteArray encodedlist_;

public:
	// Reasonable subtypes:
	//  lh_type_integer_list - Yields a dropdown box
	//  lh_type_integer_listbox - Yields a listbox

    LH_Qt_QStringList( LH_QtObject *parent, QString name, QStringList value, int flags = 0, lh_setup_type subtype = lh_type_integer_list  )
        : LH_QtSetupItem( parent, name, subtype, flags )
    {
        list_ = value;
        encodedlist_ = list_.join("\t").toUtf8();
        item_.param.list = encodedlist_.constData();
        item_.data.i = 0;
    }
    
    QStringList& list()
    {
        return list_;
    }

    void setList( const QStringList& newlist )
    {
        list_ = newlist;
        refreshList();
    }

    void refreshList()
    {
        encodedlist_ = list_.join("\t").toUtf8();
        item_.param.list = encodedlist_.constData();
        if( item_.data.i >= list_.size() ) item_.data.i = list_.size() - 1;
        refresh();
    }

    int value() const
    {
        return item_.data.i;
    }

    int index() const
    {
        return item_.data.i;
    }

    QString valueText() const
    {
        if( value() < 0 || value() >= list_.size() ) return QString();
        return list_.at( value() );
    }

    virtual void setup_change()
    {
        emit change( value() );
        LH_QtSetupItem::setup_change();
    }

    void setIndex(int i)
    {
        if( i < -1 ) i = -1;
        if( i >= list_.size() ) i = list_.size();
        if( item_.data.i != i )
        {
            item_.data.i = i;
            refresh();
            emit set();
        }
    }

    void setValue(int i)
    {
        setIndex(i);
    }

    void setValue(QString str)
    {
        setValue( list_.indexOf(str) );
    }
};

#endif // LH_QT_QSTRINGLIST_H
