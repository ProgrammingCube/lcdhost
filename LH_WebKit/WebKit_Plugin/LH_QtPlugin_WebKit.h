/**
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

#ifndef LH_QTPLUGIN_WEBKIT_H
#define LH_QTPLUGIN_WEBKIT_H

#include <QProcess>
#include <QTime>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include "../../LH_QtPlugin.h"
#include "../../LH_Qt_QString.h"

class LH_QtPlugin_WebKit : public LH_QtPlugin
{
    Q_OBJECT

    QTime last_start_;
    QNetworkAccessManager *nam_;

    void refreshExternalAddress();
    void refreshInternalAddress();
protected:
    LH_Qt_QString *setup_external_ip_;
    LH_Qt_QString *setup_internal_ip_;

public:
    LH_QtPlugin_WebKit() : LH_QtPlugin() {}

    virtual void term();
    const char *userInit();

    bool startServer();
    bool sendQuit();

public slots:
    void refreshAddresses();
    void finished( QNetworkReply* reply );

};

#endif // LH_QTPLUGIN_WEBKIT_H
