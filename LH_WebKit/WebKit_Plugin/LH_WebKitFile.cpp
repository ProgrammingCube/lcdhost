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

#include "LH_WebKitFile.h"

LH_PLUGIN_CLASS(LH_WebKitFile)

lh_class *LH_WebKitFile::classInfo()
{
    static lh_class classinfo =
    {
        sizeof(lh_class),
        "Dynamic",
        "DynamicWebKitFile",
        "WebKit local file",
        96, 32,
        lh_instance_calltable_NULL
    };

    return &classinfo;
}

LH_WebKitFile::LH_WebKitFile(const char *name) : LH_WebKit(name)
{
    setup_file_ = new LH_Qt_QFileInfo(this,"File",QFileInfo(),LH_FLAG_FOCUS);
    connect( setup_file_, SIGNAL(changed()), this, SLOT(fileChanged()) );
}

void LH_WebKitFile::fileChanged()
{
    url_ = QUrl::fromLocalFile( setup_file_->value().canonicalFilePath() );
    if( url_.isValid() ) sendRequest( url_ );
    return;
}
