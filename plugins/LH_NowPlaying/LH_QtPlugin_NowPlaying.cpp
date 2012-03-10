/**
  \file     LH_QtPlugin_NowPlaying.cpp
  \author   Johan Lindh <johan@linkdata.se>
  \author   Andy Bridges <andy@bridgesuk.com>
  \legalese Copyright (c) 2010 Johan Lindh, Andy Bridges

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

  **/

#include "LH_QtPlugin_NowPlaying.h"
#include <QFileInfo>
#include <QDebug>

LH_PLUGIN(LH_QtPlugin_NowPlaying)

char __lcdhostplugin_xml[] =
"<?xml version=\"1.0\"?>"
"<lcdhostplugin>"
  "<id>Now Playing</id>"
  "<rev>" STRINGIZE(REVISION) "</rev>"
  "<api>" STRINGIZE(LH_API_MAJOR) "." STRINGIZE(LH_API_MINOR) "</api>"
  "<ver>" STRINGIZE(VERSION) "\nr" STRINGIZE(REVISION) "</ver>"
  "<versionurl>http://www.linkdata.se/lcdhost/version.php?arch=$ARCH</versionurl>"
  "<author>Andy \"Triscopic\" Bridges</author>"
  "<homepageurl><a href=\"http://www.codeleap.co.uk\">CodeLeap</a></homepageurl>"
  "<logourl></logourl>"
  "<shortdesc>"
  "Displays the currently playing track from various music players."
  "</shortdesc>"
  "<longdesc>"
"This plugin provides displays information about the currently playing track in one of several popular "
"3rd-party music players: "
"<style type='text/css'>"
"  .yes  {background-color:#E0FFE0; padding: 2px 4px 2px 4px; color: darkgreen}"
"  .no   {background-color:#FFE0E0; padding: 2px 4px 2px 4px; color: red}"
"  .part {background-color:#F8F8D0; padding: 2px 4px 2px 4px; color: darkorange}"
"  .api  {background-color:#E0E0FF; padding: 2px 4px 2px 4px}"
"  .info {background-color:#EBEBEB; padding: 2px 4px 2px 4px}"
"  .head {background-color:#707070; padding: 2px 4px 2px 4px; color: white;}"
"</style>"
"<br/><table>"
"<tr>"
"<th class='head'>Player</th>"
"<th class='head'>API</th>"
"<th class='head'>Separate Album, <br/>Artist &amp; Track</th>"
"<th class='head'>Progress Bar &amp; <br/>Played Times</th>"
"<th class='head'>Album<br/>Artwork</th>"
"</tr>"

"<tr>"
"<td class='info' rowspan='2'><a href=\"http://www.apple.com/itunes/\">iTunes</a></td>"
"<td align='center' class='api'>iTunes</td>"
"<td align='center' class='yes'>Yes</td>"
"<td align='center' class='yes'>Yes</td>"
"<td align='center' class='yes'>Yes</td>"
"</tr>"
"<tr>"
"<td class='info' colspan='4'>Full, native support. Just run iTunes and it should work immediately.</td>"
"</tr>"

"<tr>"
"<td class='info' rowspan='2'><a href=\"http://www.winamp.com/\">Winamp</a></td>"
"<td align='center' class='api'>Winamp</td>"
"<td align='center' class='yes'>Yes</td>"
"<td align='center' class='yes'>Yes</td>"
"<td align='center' class='part'>Partial<sup>1</sup></td>"
"</tr>"
"<tr>"
"<td class='info' colspan='4'>Full, native support. Just run Winamp and it should work immediately.</td>"
"</tr>"

"<tr>"
"<td class='info' rowspan='2'><a href=\"http://www.foobar2000.org/\">foobar2000</a></td>"
"<td align='center' class='api'>Winamp</td>"
"<td align='center' class='no'>No</td>"
"<td align='center' class='yes'>Yes</td>"
"<td align='center' class='no'>No</td>"
"</tr>"
"<tr>"
"<td class='info' colspan='4'>Requires the Foobar_Winamp_Spam plugin (no specific home page but v0.98 is located <a href=\"http://home.comcast.net/~selyb/\">here</a>)</td>"
"</tr>"

"<tr>"
"<td class='info' rowspan='2'><a href='http://windows.microsoft.com/en-US/windows/products/windows-media-player'>Windows Media <br/>Player</a></td>"
"<td align='center' class='api'>MSN<sup>2</sup></td>"
"<td align='center' class='yes'>Yes</td>"
"<td align='center' class='no'>No<sup>3</sup></td>"
"<td align='center' class='no'>No<sup>3</sup></td>"
"</tr>"
"<tr>"
"<td class='info' colspan='4'>Requires the \"Windows Live Messenger Music Plugin\", found under Plugins &gt; Background.</td>"
"</tr>"

"<tr>"
"<td class='info' rowspan='2'><a href=\"http://www.videolan.org/vlc/\">VLC Player</a></td>"
"<td align='center' class='api'>MSN<sup>2</sup></td>"
"<td align='center' class='yes'>Yes</td>"
"<td align='center' class='no'>No<sup>3</sup></td>"
"<td align='center' class='no'>No<sup>3</sup></td>"
"</tr>"
"<tr>"
"<td class='info' colspan='4'>Requires the \"MSN Now Playing\" interface to be enabled, found under Preferences &gt; All &gt; Interface &gt; Control Interfaces.</td>"
"</tr>"

"<tr>"
"<td class='info' rowspan='2'><a href=\"http://www.spotify.com/\">Spotify</a></td>"
"<td align='center' class='api'>MSN<sup>2</sup></td>"
"<td align='center' class='yes'>Yes</td>"
"<td align='center' class='no'>No<sup>3</sup></td>"
"<td align='center' class='no'>No<sup>3</sup></td>"
"</tr>"
"<tr>"
"<td class='info' colspan='4'>Partial, native support. Just run Spotify and it should work immediately.</td>"
"</tr>"

"</table>"
"<br/><br/>"
"...and any other player that supports the MSN Now Playing interface, such as <a href=\"http://www.last.fm/download/\">Last.fm</a>, <a href=\"http://getopenpandora.appspot.com/\">OpenPandora</a>, <a href=\"http://getsongbird.com/\">Songbird</a>(+<a href=\"http://addons.songbirdnest.com/addon/1204\">LiveTweeter</a>), <a href=\"http://www.zune.net\">Zune</a> and others."
"<br/><br/><sup>1</sup> Album artwork is only available from Winamp if the music's folder contains &quot;folder.jpg&quot;."
"<br/><br/><sup>2</sup> Tests suggest the MSN interface does not function under 64-bit Windows."
"<br/><br/><sup>3</sup> The MSN interface only supports the seperate Artist, Album &amp; Track details, hence players using this system cannot display the progress bar or album art."
"</longdesc>"
"</lcdhostplugin>";

LH_NowPlaying_Reader* currentTrack;

bool get_itunes_info(TrackInfo &ti, QString artworkPath, artworkDescription &cachedArtwork, bool &updatedArtwork);
bool get_winamp_info(TrackInfo &ti);
bool get_msn_compat_info(struct TrackInfo &ti);

bool get_folder_artwork(TrackInfo newInfo, QString artworkCachePath, artworkDescription &cachedArtwork)
{
    QFileInfo newItemFile(newInfo.file);
    QString newItemFilePath = newItemFile.absolutePath();
    QString folderImage = newItemFilePath+"/folder.jpg";
    QString artworkCacheFileName = QString("%0%1art.%2").arg(artworkCachePath).arg(artworkCachePath.endsWith("/")? "" : "/").arg("jpg");
    if(QFile::exists(folderImage))
    {
        if(QFile::exists(artworkCacheFileName))
        {
            #ifdef Q_OS_WIN
                SetFileAttributes((LPCTSTR)artworkCacheFileName.utf16(), 0);
            #endif
            QFile::remove(artworkCacheFileName);
        }
        if(QFile::copy(folderImage, artworkCacheFileName))
        {
            #ifdef Q_OS_WIN
                SetFileAttributes((LPCTSTR)artworkCacheFileName.utf16(), 0);
            #endif
            cachedArtwork.cacheMode = amFileFolder;
            cachedArtwork.album = newInfo.album;
            cachedArtwork.artist = newInfo.artist;
            cachedArtwork.sourcefolder = newItemFilePath;
            cachedArtwork.fileName = artworkCacheFileName;
            return true;
        } else {
            qWarning() << "NowPlaying: Unable to copy album artwork from " << folderImage;
        }
    }
    return false;
}

void close_itunes_connection();

LH_NowPlaying_Reader::~LH_NowPlaying_Reader() {
    close_itunes_connection();
}

void LH_NowPlaying_Reader::refresh()
{
    playerFound_ = false;
    TrackInfo newInfo;
    bool updatedArtwork = false;

    playerFound_ = playerFound_ || get_itunes_info(newInfo, artworkCachePath_, cachedArtwork_, updatedArtwork);
    playerFound_ = playerFound_ || get_winamp_info(newInfo);
    playerFound_ = playerFound_ || get_msn_compat_info(newInfo);

    if(storeInfo(newInfo))
        emit changed();

    if(playerFound_ && !updatedArtwork)
    {
        QFileInfo newItemFile(newInfo.file);
        QString newItemFilePath = newItemFile.absolutePath();
        bool validArtwork = false;
        switch(cachedArtwork_.cacheMode)
        {
        case amArtistAndAlbumName:
            validArtwork = (cachedArtwork_.album == newInfo.album && cachedArtwork_.artist == newInfo.artist);
            break;
        case amFileFolder:
            validArtwork = (cachedArtwork_.sourcefolder == newItemFilePath);
            break;
        case amTrack:
            validArtwork = (cachedArtwork_.track == newInfo.track && cachedArtwork_.album == newInfo.album && cachedArtwork_.artist == newInfo.artist);
            break;
        case amNone:
            validArtwork = false;
            break;
        }
        if(!validArtwork)
        {
            currentTrack->clearArtwork();
            updatedArtwork = get_folder_artwork(newInfo, artworkCachePath_, cachedArtwork_);
            cachedArtwork_.album = newInfo.album;
            cachedArtwork_.artist = newInfo.artist;
            if(!updatedArtwork)
            {
                cachedArtwork_.sourcefolder = newItemFilePath;
                updatedArtwork = true;
            } else {
                cachedArtwork_.track == newInfo.track;
                cachedArtwork_.cacheMode = amTrack;
            }
        }
    }

    if(!playerFound_ && currentTrack->artworkFileName()!="")
    {
        currentTrack->clearArtwork();
        updatedArtwork = true;
    }
    if(updatedArtwork)
        emit artworkChanged();
}

bool LH_NowPlaying_Reader::storeInfo(TrackInfo newInfo)
{
    bool dirty = false;
    dirty = dirty || (info_.album != newInfo.album);
    dirty = dirty || (info_.artist != newInfo.artist);
    dirty = dirty || (info_.currentSecs != newInfo.currentSecs);
    dirty = dirty || (info_.player != newInfo.player);
    dirty = dirty || (info_.status != newInfo.status);
    dirty = dirty || (info_.totalSecs!= newInfo.totalSecs);
    dirty = dirty || (info_.track!= newInfo.track);

    info_ = newInfo;
    return dirty;
}

const char *LH_QtPlugin_NowPlaying::userInit() {
    currentTrack = new LH_NowPlaying_Reader(this);
    timer_.setInterval(500);
    timer_.start();
    connect(&timer_, SIGNAL(timeout()), this, SLOT(refresh_data()));
    return NULL;
}

void LH_QtPlugin_NowPlaying::userTerm() {
    //currentTrack->clearArtwork();
    timer_.stop();
    while( currentTrack->isRunning() )
    {
        currentTrack->quit();
        if( currentTrack->wait(1000) ) break;
        qCritical() << "LH_NowPlaying_Reader not responding";
    }
    currentTrack->deleteLater();
}

void LH_QtPlugin_NowPlaying::refresh_data() {
    if(!currentTrack->isRunning())
         currentTrack->run();
}