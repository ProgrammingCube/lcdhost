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
#include "Internet.h"

LH_PLUGIN(LH_QtPlugin_NowPlaying)

CPlayer* player;
ArtworkCache* artworkCache;
bool isElevated;

#if !defined(TokenElevationType)
#define TokenElevationType 18
#endif

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
"<th class='head'>Album Artwork, Track<br/>"
                 "Times &amp; Progress Bar</th>"
"<th class='head'>Suffle &amp; Repeat<br/>Control</th>"
"</tr>"

"<tr>"
"<td class='info' rowspan='2'>"
"<table><tr>"
"<td style=\"padding-right:2px\"><img src=\":/images/player_iTunes.png\"/></td>"
"<td><a href=\"http://www.apple.com/itunes/\">iTunes</a></td>"
"</tr></table>"
"</td>"
"<td align='center' class='api'>iTunes<sup>1</sup></td>"
"<td align='center' class='yes'>Yes</td>"
"<td align='center' class='yes'>Yes</td>"
"<td align='center' class='yes'>Yes</td>"
"</tr>"
"<tr>"
"<td class='info' colspan='4'>Full, native support. Just run iTunes and it should work immediately.</td>"
"</tr>"

"<tr>"
"<td class='info' rowspan='2'>"
"<table><tr>"
"<td style=\"padding-right:2px\"><img src=\":/images/player_winamp.png\"/></td>"
"<td><a href=\"http://www.winamp.com/\">Winamp</a></td>"
"</tr></table>"
"</td>"
"<td align='center' class='api'>Winamp</td>"
"<td align='center' class='yes'>Yes</td>"
"<td align='center' class='yes'>Yes</td>"
"<td align='center' class='yes'>Yes</td>"
"</tr>"
"<tr>"
"<td class='info' colspan='4'>Full, native support. Just run Winamp and it should work immediately.</td>"
"</tr>"

"<tr>"
"<td class='info' rowspan='2'>"
"<table><tr>"
"<td style=\"padding-right:2px\"><img src=\":/images/player_foobar2000.png\"/></td>"
"<td><a href=\"http://www.foobar2000.org/\">foobar2000</a></td>"
"</tr></table>"
"</td>"
"<td align='center' class='api'>Winamp / Rainmeter</td>"
"<td align='center' class='yes'>Yes</td>"
"<td align='center' class='yes'>Yes</td>"
"<td align='center' class='yes'>Yes<sup>2</sup></td>"
"</tr>"
"<tr>"
"<td class='info' colspan='4'>Requires the Foobar_Winamp_Spam plugin (no specific home page but v0.98 is located <a href=\"http://home.comcast.net/~selyb/\">here</a>) or the <a href=\"http://code.google.com/p/foo-rainmeter/\">foo_rainmeter.dll</a> plugin</td>"
"</tr>"

"<tr>"
"<td class='info' rowspan='2'>"
"<table><tr>"
"<td style=\"padding-right:2px\"><img src=\":/images/player_vlc.png\"/></td>"
"<td><a href=\"http://www.videolan.org/vlc/\">VLC Player</a></td>"
"</tr></table>"
"</td>"
"<td align='center' class='api'>VLC Web</td>"
"<td align='center' class='yes'>Yes</td>"
"<td align='center' class='yes'>Yes</td>"
"<td align='center' class='yes'>Yes</td>"
"</tr>"
"<tr>"
"<td class='info' colspan='4'>Requires the \"Web Interface\" to be enabled, found under View &gt; Add Interface &gt; Web Interface.</td>"
"</tr>"

"<tr>"
"<td class='info' rowspan='2'>"
"<table><tr>"
"<td style=\"padding-right:2px\"><img src=\":/images/player_wmp.png\"/></td>"
"<td><a href='http://windows.microsoft.com/en-US/windows/products/windows-media-player'>Windows Media <br/>Player</a></td>"
"</tr></table>"
"</td>"
"<td align='center' class='api'>MSN<sup>3</sup></td>"
"<td align='center' class='yes'>Yes</td>"
"<td align='center' class='no'>No<sup>4</sup></td>"
"<td align='center' class='no'>No<sup>4</sup></td>"
"</tr>"
"<tr>"
"<td class='info' colspan='4'>Requires the \"Windows Live Messenger Music Plugin\", found under Plugins &gt; Background.</td>"
"</tr>"

"<tr>"
"<td class='info' rowspan='2'>"
"<table><tr>"
"<td style=\"padding-right:2px\"><img src=\":/images/player_spotify.png\"/></td>"
"<td><a href=\"http://www.spotify.com/\">Spotify</a></td>"
"</tr></table>"
"</td>"
"<td align='center' class='api'>Title Bar Split</td>"
"<td align='center' class='part'>Partial<sup>5</sup></td>"
"<td align='center' class='no'>No</td>"
"<td align='center' class='no'>No</td>"
"</tr>"
"<tr>"
"<td class='info' colspan='4'>Partial, native support. Just run Spotify and it should work immediately.</td>"
"</tr>"

"</table>"
"<br/><br/>"
"...and any other player that supports the MSN Now Playing interface, such as <a href=\"http://www.last.fm/download/\">Last.fm</a>, <a href=\"http://getopenpandora.appspot.com/\">OpenPandora</a>, <a href=\"http://getsongbird.com/\">Songbird</a>(+<a href=\"http://addons.songbirdnest.com/addon/1204\">LiveTweeter</a>), <a href=\"http://www.zune.net\">Zune</a> and others."
"<br/><br/><sup>1</sup> If LCDHost is running as an administrator and iTunes is not (or visa versa) LH_NowPlaying will crash. Either run both as administrator or neither."
"<br/><br/><sup>2</sup> Foobar via Foo_Rainmeter.dll fully supports shuffle and repeat. The winamp spam dll supports sending shuffle and repeat toggle commands, but cannot read the values back."
"<br/><br/><sup>3</sup> Some users of 64 bit windows report that the MSN interface does not function with any application (i.e. the issue is unrelated to LCDHost); others have reported no problem."
"<br/><br/><sup>4</sup> The MSN interface only supports the seperate Artist, Album &amp; Track details, hence players using this system cannot display the progress bar or album art and cannot access or set shuffle/repeat data."
"<br/><br/><sup>5</sup> The Title bar interface only supports the seperate Artist &amp; Track details, not the album name. As with MSN, players using this system cannot display the progress bar or album art."
"</longdesc>"
"</lcdhostplugin>";

const char *LH_QtPlugin_NowPlaying::userInit() {
    CInternet::Initialize();

    artworkCache = new ArtworkCache;
    artworkCache->cachePath = QString::fromUtf8(state()->dir_data);
    Q_ASSERT( artworkCache->cachePath.endsWith('/') );

    player = NULL;
    clearPlayer();

    timer_.setInterval(500);
    timer_.start();
#ifdef ITUNES_AUTO_CLOSING
    elapsedTime_.start();
    forceClose_ = false;
#endif
    connect(&timer_, SIGNAL(timeout()), this, SLOT(refresh_data()));

    (setup_vlc_port_ = new LH_Qt_QString(this, "VLC Port","8080", LH_FLAG_NOSOURCE | LH_FLAG_NOSINK))->setHelp(
                "Specify the port the VLC web admin interface runs on (default is 8080).");

    new LH_Qt_QString(this, "~hr1", "<hr />", LH_FLAG_HIDETITLE | LH_FLAG_NOSINK | LH_FLAG_NOSOURCE | LH_FLAG_NOSAVE, lh_type_string_html);
    (new LH_Qt_QString(this, "~blurb1", "Media Key Bindings (Global):", LH_FLAG_HIDETITLE | LH_FLAG_NOSINK | LH_FLAG_NOSOURCE | LH_FLAG_NOSAVE, lh_type_string_html))->setHelp(
                "This allows you to use the media keys to control players that are not automatically compatible with them.");


    setup_input_play_pause_ = new LH_Qt_InputState(this, "Play & Pause", "", LH_FLAG_NOSOURCE | LH_FLAG_NOSINK);
    connect( setup_input_play_pause_, SIGNAL(input(QString,int,int)), this, SLOT(controlPlayPauseClick(QString,int,int)) );

    setup_input_stop_ = new LH_Qt_InputState(this, "Stop", "", LH_FLAG_NOSOURCE | LH_FLAG_NOSINK);
    connect( setup_input_stop_, SIGNAL(input(QString,int,int)), this, SLOT(controlStopClick(QString,int,int)) );

    setup_input_next_ = new LH_Qt_InputState(this, "Next Track", "", LH_FLAG_NOSOURCE | LH_FLAG_NOSINK);
    connect( setup_input_next_, SIGNAL(input(QString,int,int)), this, SLOT(controlNextClick(QString,int,int)) );

    setup_input_prev_ = new LH_Qt_InputState(this, "Previous Track", "", LH_FLAG_NOSOURCE | LH_FLAG_NOSINK);
    connect( setup_input_prev_, SIGNAL(input(QString,int,int)), this, SLOT(controlPrevClick(QString,int,int)) );

    setup_media_keys_iTunes_ = new LH_Qt_bool(this, "^Enable iTunes Control", false, LH_FLAG_NOSOURCE | LH_FLAG_NOSINK | LH_FLAG_HIDDEN);
    setup_media_keys_Winamp_ = new LH_Qt_bool(this, "^Enable Winamp Control", false, LH_FLAG_NOSOURCE | LH_FLAG_NOSINK | LH_FLAG_HIDDEN);
    setup_media_keys_Foobar_ = new LH_Qt_bool(this, "^Enable Foobar Control", false, LH_FLAG_NOSOURCE | LH_FLAG_NOSINK | LH_FLAG_HIDDEN);
    setup_media_keys_Spotify_ = new LH_Qt_bool(this, "^Enable Spotify Control", false, LH_FLAG_NOSOURCE | LH_FLAG_NOSINK | LH_FLAG_HIDDEN);
    setup_media_keys_VLC_ = new LH_Qt_bool(this, "^Enable VLC Player Control", true, LH_FLAG_NOSOURCE | LH_FLAG_NOSINK);
    setup_media_keys_WLM_ = new LH_Qt_bool(this, "^Enable MSN-Compatible Control", false, LH_FLAG_NOSOURCE | LH_FLAG_NOSINK | LH_FLAG_HIDDEN);

    new LH_Qt_QString(this, "~hr2", "<hr />", LH_FLAG_HIDETITLE | LH_FLAG_NOSINK | LH_FLAG_NOSOURCE | LH_FLAG_NOSAVE, lh_type_string_html);
    (new LH_Qt_QString(this, "~blurb2", "Advanced Key Bindings (Global):", LH_FLAG_HIDETITLE | LH_FLAG_NOSINK | LH_FLAG_NOSOURCE | LH_FLAG_NOSAVE, lh_type_string_html))->setHelp(
                "<p>This allows you to set up \"extra\" multimedia keys, e.g. bind \"Scroll Lock\" to repeat the current track.</p>"
                "<p>Keys configured here will <b>always</b> work, regardless of what layout is loaded. To make keys only work when a specific layout is loaded add the \"Remote Control\" object to the layout.</p>"
                "<p>Note that not all players are compatible with these features.</p>");

    setup_input_shuffle_ = new LH_Qt_InputState(this, "Shuffle", "", LH_FLAG_NOSOURCE | LH_FLAG_NOSINK);
    connect( setup_input_shuffle_, SIGNAL(input(QString,int,int)), this, SLOT(controlShuffleClick()) );

    setup_input_repeat_ = new LH_Qt_InputState(this, "Repeat", "", LH_FLAG_NOSOURCE | LH_FLAG_NOSINK);
    connect( setup_input_repeat_, SIGNAL(input(QString,int,int)), this, SLOT(controlRepeatClick()) );

    setup_input_close_ = new LH_Qt_InputState(this, "Close Player", "", LH_FLAG_NOSOURCE | LH_FLAG_NOSINK);
    connect( setup_input_close_, SIGNAL(input(QString,int,int)), this, SLOT(controlCloseClick()) );

    new LH_Qt_QString(this, "~hr3", "<hr />", LH_FLAG_HIDETITLE | LH_FLAG_NOSINK | LH_FLAG_NOSOURCE | LH_FLAG_NOSAVE, lh_type_string_html);
    (new LH_Qt_QString(this, "~blurb3", "Remote Controls:", LH_FLAG_HIDETITLE | LH_FLAG_NOSINK | LH_FLAG_NOSOURCE | LH_FLAG_NOSAVE, lh_type_string_html))->setHelp(
                "These buttons are provided for convenience and testing purposes.");

    setup_control_play_pause_ = new LH_Qt_QString(this, "Play_Pause Button", "Play/Pause", LH_FLAG_HIDETITLE | LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_NOSINK, lh_type_string_button );
    connect( setup_control_play_pause_, SIGNAL(changed()), this, SLOT(controlPlayPauseClick()) );

    setup_control_stop_ = new LH_Qt_QString(this, "Stop Button", "Stop", LH_FLAG_HIDETITLE | LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_NOSINK, lh_type_string_button );
    connect( setup_control_stop_, SIGNAL(changed()), this, SLOT(controlStopClick()) );

    setup_control_next_ = new LH_Qt_QString(this, "Next Button", "Next", LH_FLAG_HIDETITLE | LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_NOSINK, lh_type_string_button );
    connect( setup_control_next_, SIGNAL(changed()), this, SLOT(controlNextClick()) );

    setup_control_prev_ = new LH_Qt_QString(this, "Previous Button", "Previous", LH_FLAG_HIDETITLE | LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_NOSINK, lh_type_string_button );
    connect( setup_control_prev_, SIGNAL(changed()), this, SLOT(controlPrevClick()) );

    setup_control_repeat_ = new LH_Qt_QString(this, "Repeat Button", "Repeat", LH_FLAG_HIDETITLE | LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_NOSINK, lh_type_string_button );
    connect( setup_control_repeat_, SIGNAL(changed()), this, SLOT(controlRepeatClick()) );

    setup_control_shuffle_ = new LH_Qt_QString(this, "Shuffle Button", "Shuffle", LH_FLAG_HIDETITLE | LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_NOSINK, lh_type_string_button );
    connect( setup_control_shuffle_, SIGNAL(changed()), this, SLOT(controlShuffleClick()) );

    setup_control_close_ = new LH_Qt_QString(this, "Close Button", "Close Player", LH_FLAG_HIDETITLE | LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_NOSINK, lh_type_string_button );
    connect( setup_control_close_, SIGNAL(changed()), this, SLOT(controlCloseClick()) );

    isElevated = (GetElevationState(GetCurrentProcessId()) == ELEVATION_ELEVATED);
    //if(isElevated)
    //    qWarning() << "LCDHost is running as administrator. LH_NowPlaying will crash if iTunes is opened without also being run as administrator.";

    qDebug() << getWindowClass("VLC media player");
    return 0;
}

bool LH_QtPlugin_NowPlaying::playerControlCheck()
{
    if(!player)
        return false;
    QString playerName = player->GetPlayer();
    if(playerName.startsWith("iTunes"))
        return setup_media_keys_iTunes_->value();
    if(playerName.startsWith("Winamp"))
        return setup_media_keys_Winamp_->value();
    if(playerName.startsWith("Foobar"))
        return setup_media_keys_Foobar_->value();
    if(playerName.startsWith("Spotify"))
        return setup_media_keys_Spotify_->value();
    if(playerName.startsWith("VLC"))
        return setup_media_keys_VLC_->value();
    if(playerName.startsWith(CPlayerWLM::WLMPlayerName()))
        return setup_media_keys_WLM_->value();
    return false;
}

void LH_QtPlugin_NowPlaying::controlPlayPauseClick(QString key,int flags,int value)
{
    Q_UNUSED(flags)
    Q_UNUSED(value)
    if(key=="" || playerControlCheck())
    {
        if(!player)
            return;
        if(player->GetState()==PLAYER_PLAYING)
            player->Pause();
        else
            player->Play();
    }
}

void LH_QtPlugin_NowPlaying::controlStopClick(QString key,int flags,int value)
{
    Q_UNUSED(flags)
    Q_UNUSED(value)
    if(key=="" || playerControlCheck())
        if(player)
            player->Stop();;
}

void LH_QtPlugin_NowPlaying::controlNextClick(QString key,int flags,int value)
{
    Q_UNUSED(flags)
    Q_UNUSED(value)
    if(key=="" || playerControlCheck())
        if(player)
            player->Next();
}

void LH_QtPlugin_NowPlaying::controlPrevClick(QString key,int flags,int value)
{
    Q_UNUSED(flags)
    Q_UNUSED(value)
    if(key=="" || playerControlCheck())
        if(player)
            player->Previous();
}

void LH_QtPlugin_NowPlaying::controlCloseClick()
{
    if(player)
        player->ClosePlayer();
}

void LH_QtPlugin_NowPlaying::controlRepeatClick()
{
    if(player)
        player->SetRepeat(!player->GetRepeat());
}

void LH_QtPlugin_NowPlaying::controlShuffleClick()
{
    if(player)
       player->SetShuffle(!player->GetShuffle());
}

void LH_QtPlugin_NowPlaying::userTerm() {
    timer_.stop();
    if(player) delete player;
    if(artworkCache) delete artworkCache;
    CInternet::Finalize();
}

void LH_QtPlugin_NowPlaying::refresh_data() {
    if(player)
    {
        if(player->GetPlayer()=="")
        {
            bool elevationsMatch = false;
            HWND hWnd_iTunes = FindWindowA("iTunes", NULL);
            HWND hWnd_Foobar = FindWindowA("{97E27FAA-C0B3-4b8e-A693-ED7881E99FC1}", NULL);

            if (hWnd_iTunes)
            {
                unsigned long procID;
                GetWindowThreadProcessId(hWnd_iTunes, &procID);
                elevationState iTunes_Elevation = GetElevationState(procID);
                switch( iTunes_Elevation )
                {
                case ELEVATION_UNKNOWN:
                    // returned when iTunes is elevated & LCDHost is not
                    elevationsMatch = false;
                    break;
                case ELEVATION_NORMAL:
                    elevationsMatch = !isElevated;
                    break;
                case ELEVATION_ELEVATED:
                    elevationsMatch = isElevated;
                    break;
                }
            }

            if (hWnd_iTunes && elevationsMatch)
            {
                if(player) delete player;
                player = CPlayerITunes::Create();
                player->SetPlayer("iTunes");
            } else
            if (hWnd_Foobar && FindWindowA("foo_rainmeter_class", NULL)) //Foobar with Rainmeter plugin
            {
                if(player) delete player;
                player = CPlayerFoobar::Create();
                player->SetPlayer("Foobar (RAIN)");
            } else
            if (hWnd_Foobar && FindWindowA("Winamp v1.x", NULL)) //Foobar with Winamp_Spam plugin
            {
                if(player) delete player;
                player = CPlayerWinamp::Create(WA_WINAMP);
                player->SetPlayer("Foobar (WAMP)");
            } else
            if (FindWindowA("Winamp v1.x", NULL))
            {
                if(player) delete player;
                player = CPlayerWinamp::Create(WA_WINAMP);
                player->SetPlayer("Winamp");
            } else
            if (FindWindowA("SpotifyMainWindow", NULL))
            {
                if(player) delete player;
                player = CPlayerSpotify::Create();
                player->SetPlayer("Spotify");
            } else
            if (CPlayerVLC::TestUrl(setup_vlc_port_->value().toStdWString()))
            {
                if(player) delete player;
                player = CPlayerVLC::Create(setup_vlc_port_->value().toStdWString());
                player->SetPlayer("VLC");
            } else
            if (hWnd_iTunes && !elevationsMatch)
            {
#ifdef ITUNES_AUTO_CLOSING
                if(isElevated && elapsedTime_.elapsed() <= 30*1000 && forceClose_)
                {
                    qDebug() << "Extraneous iTunes spotted. Head shot to process.";
                    CloseWindow(hWnd_iTunes);
                    forceClose_ = false;
                } else
#endif
                if(hWnd_iTunes_warn_cache_ != hWnd_iTunes)
                {
                    hWnd_iTunes_warn_cache_ = hWnd_iTunes;
                    qWarning() << "LH_NowPlaying: iTunes detected, but running with a different elevation level.";
                }
            } else
            if (hWnd_Foobar) //Foobar, no assistant plugin
            {
                if(hWnd_Foobar_warn_cache_ != hWnd_Foobar)
                {
                    hWnd_Foobar_warn_cache_ = hWnd_Foobar;
                    qWarning() << "LH_NowPlaying: Foobar detected with no API plugin loaded.";
                }
            }

            if(player)
            {
                player->AddMeasure(MEASURE_COVER);
                player->AddMeasure(MEASURE_LYRICS);
                player->setArtworkCachePath( artworkCache );
                player->AddInstance();
            }
        } else {
            if(player->GetPlayer()=="iTunes")
                if (!FindWindowA("iTunes", NULL))
                {
#ifdef ITUNES_AUTO_CLOSING
                    elapsedTime_.restart();
                    forceClose_ = true;
#endif
                    return clearPlayer();
                }
            if(player->GetPlayer()=="Winamp")
                if (!FindWindowA("Winamp v1.x", NULL))
                    return clearPlayer();
            if(player->GetPlayer().startsWith("Foobar"))
                if (!FindWindowA("{97E27FAA-C0B3-4b8e-A693-ED7881E99FC1}", NULL))
                    return clearPlayer();
            if(player->GetPlayer()=="Spotify")
                if (!FindWindowA("SpotifyMainWindow", NULL))
                    return clearPlayer();
            if(player->GetPlayer()=="VLC")
                if (!CPlayerVLC::TestUrl(setup_vlc_port_->value().toStdWString()))
                    return clearPlayer();
        }

        player->UpdateData();
        emit updated_data();
    }
}

void LH_QtPlugin_NowPlaying::clearPlayer()
{
    if(player)
    {
        delete player;
        player = NULL;
    }

    player = CPlayerWLM::Create();
    player->SetPlayer(CPlayerWLM::WLMPlayerName());

    if(artworkCache)
    {
        artworkCache->album = "//XX-DEAD//NODATA";
        artworkCache->artist = "//XX-DEAD//NODATA";
        artworkCache->cacheFile = "";
    }
    emit updated_data();
}

LH_QtPlugin_NowPlaying::elevationState LH_QtPlugin_NowPlaying::GetElevationState(DWORD PID)    // Returns 0 if process is elevated, 1 if process is not elevated or -1 if a function fails.
{
    elevationState result = ELEVATION_UNKNOWN;
    TOKEN_ELEVATION_TYPE ElevationType = TokenElevationTypeFull;
    HANDLE hToken = 0;
    HANDLE hProcess = 0;
    DWORD SizeReturned = 0;

    if (PID)
    {
        if ((hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, PID)))
        {
            if(OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
            {
                if (GetTokenInformation(hToken, (TOKEN_INFORMATION_CLASS)TokenElevationType, &ElevationType, sizeof(ElevationType), &SizeReturned))
                {
                    if (ElevationType == TokenElevationTypeFull)
                        result = ELEVATION_ELEVATED;
                    else
                        result = ELEVATION_NORMAL;
                }
                //else qDebug() << "GetTokenInformation function call failed: " << getLastErrorMessage();
            }
            //else qDebug() << "OpenProcessToken function call failed: " << getLastErrorMessage();
            CloseHandle(hToken);
        }
        //else qDebug() << "OpenProcess function call failed: " << getLastErrorMessage();
        CloseHandle(hProcess);
    }
    //qDebug() << "GetCurrentProcessID function call failed: " << getLastErrorMessage();
    return result;
}
