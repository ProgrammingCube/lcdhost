/**
  \file     LH_QtPlugin_TS3.h
  \author   Andy Bridges <triscopic@codeleap.co.uk>
  \legalese
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

#ifndef LH_QTPLUGIN_TS3_H
#define LH_QTPLUGIN_TS3_H

#include "../LH_QtPlugin.h"
#include "../LH_Qt_QString.h"
#include "../LH_Qt_bool.h"
#include "../LH_Qt_html.h"
#include "../LH_Qt_QStringList.h"

#include "LH_TS3_ChannelList.h"
#include "LH_TS3_ClientList.h"

#include <QDebug>
#include <QTcpSocket>
#include <QRegExp>
#include <QTime>
#include <QTimer>

#define VERSION 1.00

enum server_action {
    sa_none,
    sa_disconnected,
    sa_connecting,
    sa_eventregister_pending,
    sa_whoami_pending,
    sa_reconnect,
    sa_channellist_pending,
    sa_clientlist_pending,
    sa_listening
};

struct responseResult
{
    bool isResult;
    bool isValid;
    int errorCode;
    QString errorMessage;
    QString extraMessage;
};

class LH_QtPlugin_TS3 : public LH_QtPlugin
{
    Q_OBJECT

    QTime tryConnectTimer_;
    QTcpSocket *socket_;
    server_action server_action_;

    channellist channels_;
    clientlist clients_;
    clientlist speakers_;

    int myclid_;

    void openConnection();
    void talkChanged(QString params);
    responseResult parseResult(QString msg);
    void updateTalking(bool force = false);
    void updateStatus(bool isRunning, bool isConnected = false, bool showChannels = false, bool showClients = false);
protected:
    LH_Qt_QString *setup_connection_details_;
    LH_Qt_QString *setup_talking_;
    LH_Qt_QString *setup_talking_details_;
    //LH_Qt_QString *setup_nickname_expression_;
    LH_Qt_QString *setup_nickname_;
    LH_Qt_QString *setup_channelname_;
    LH_Qt_QStringList *setup_connection_status_;
    LH_Qt_QStringList *setup_microphone_status_;
    LH_Qt_QStringList *setup_speakers_status_;
    LH_Qt_QString *setup_user_detail_;

public:
    const char *userInit();
    int notify( int code, void *param );

    int sendMessage(QString);
public slots:
    void TS3Connected();
    void TS3Disconnected();
    void TS3ConnectionError(QAbstractSocket::SocketError);
    void TS3DataReceived();
    void updateMyDetails();

signals:
    void talkingChanged(QString);
    void myDetailsChanged();
};

#endif // LH_QTPLUGIN_TS3_H
