/**
  \file     LH_QtNetwork.h
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

#ifndef LH_QTNETWORK_H
#define LH_QTNETWORK_H

#include "lh_api6/lh_api6.h"
#include <QQueue>

class LH_QtInstance;

class LH_QtNetwork : public QObject
{
    QQueue<long long> netin_;
    QQueue<long long> netout_;

    void smoothingOrder(int n) { }
    void smoothingHidden(bool hide) {  }

protected:
    lh_setup setup_smoothing_;
    lh_sink sink_netin_;
    lh_sink sink_netout_;

public:
    explicit LH_QtNetwork( LH_QtInstance *parent );
    ~LH_QtNetwork();

    LH_QtInstance *parent() const { return reinterpret_cast<LH_QtInstance *>(QObject::parent()); }

    int samples() { return setup_smoothing_.value().toInt() + 1; }

    long long inRate() const;
    long long outRate() const;
    int inPermille() const;
    int outPermille() const;
    int tpPermille() const;

public slots:
    void netInChanged( const QVariant & );
    void netOutChanged( const QVariant & );

};

#endif // LH_QTNETWORK_H
