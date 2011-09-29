/**
  \file     lh_qobject.cpp
  \brief    Extends the normal QObject to be aware of LCDHost properties.
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

#include <QDebug>
#include "lh_api6/lh_api6.h"

using namespace lh::api6;

void object::lh_init( const id_ptr & id )
{
    lh_id_ = id;
    error_.clear();
    if( !lh_id_ ) return;

    lh_id_->lh_bind( *this );

    QObjectList::const_iterator it;
    for( it = children().constBegin(); it != children().constEnd(); ++ it )
    {
        if( object *child = qobject_cast<object *>(*it) )
            child->lh_create();
    }

    if( !init() && error_.isEmpty() )
    {
        error_ = tr("%1 %2: init() failed")
                .arg(metaObject()->className())
                .arg(objectName());
    }

    lh_id_->lh_init_result( error_ );

    return;
}

void object::customEvent( QEvent *event )
{
    switch( (event::type) event->type() )
    {

    case event::init::type:
    {
        event::init *e = static_cast<event::init *>(event);
        object *dest = 0;
        if( e->childName().isEmpty() )
        {
            // special case for the main plugin object
            dest = this;
        }
        else
        {
            QObjectList::const_iterator it;
            for( it = children().constBegin(); it != children().constEnd(); ++ it )
            {
                if( (*it)->objectName() == e->childName() )
                    dest = qobject_cast<object *>(*it);
                if( dest ) break;
            }
        }

        if( dest )
        {
            dest->lh_init( e->id() );
        }
        else
        {
            qCritical() << metaObject()->className()
                        << objectName()
                        << "event_init: no child named"
                        << e->childName();
        }

        return;
    }

    case event::setproperty::type:
    {
        event::setproperty *e = static_cast<event::setproperty *>(event);
        if( !setProperty( e->name(), e->value() ) )
            qCritical() << metaObject()->className()
                        << objectName()
                        << "event_setproperty: failed to set"
                        << e->name();
        return;
    }

    case event::first_type:
    case event::last_type:
        break;
    }

    qCritical() << metaObject()->className()
                << objectName()
                << ": unhandled event"
                << event->type()
                << event::name( (event::type) event->type() );
    return;
}

QString object::ident( const QString & str, const QObject *obj )
{
    QString retv(
                str.simplified()
                .replace('<','(')
                .replace('/','|')
                .replace('>',')')
                .replace('\"','\'')
                .replace('\\','|')
                );

    if( retv.isEmpty() && obj )
    {
        retv.append( obj->metaObject()->className() );
        retv.append( '@' );
        retv.append( QString::number((qptrdiff)obj,16) );
    }

    return retv;
}
