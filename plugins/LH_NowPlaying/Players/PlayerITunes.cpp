/*
  Copyright (C) 2011 Birunthan Mohanathas (www.poiru.net)

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "PlayerITunes.h"
#include <QDebug>
#include <QFile>

CPlayer* CPlayerITunes::c_Player = NULL;
 HINSTANCE g_Instance;

/*
** CEventHandler
**
** Constructor.
**
*/
CPlayerITunes::CEventHandler::CEventHandler(CPlayerITunes* player) :
    m_RefCount(0),
    m_Player(player),
	m_ConnectionPoint(),
	m_ConnectionCookie()
{
	IConnectionPointContainer* icpc;
	m_Player->m_iTunes->QueryInterface(IID_IConnectionPointContainer, (void**)&icpc);
	icpc->FindConnectionPoint(DIID__IiTunesEvents, &m_ConnectionPoint);
	m_ConnectionPoint->Advise(this, &m_ConnectionCookie);
	icpc->Release();
}

/*
** ~CEventHandler
**
** Destructor.
**
*/
CPlayerITunes::CEventHandler::~CEventHandler()
{
	if (m_ConnectionPoint)
	{
		m_ConnectionPoint->Unadvise(m_ConnectionCookie);
		m_ConnectionPoint->Release();
	}
}

HRESULT STDMETHODCALLTYPE CPlayerITunes::CEventHandler::QueryInterface(REFIID iid, void** ppvObject)
{
	if (iid == IID_IUnknown || iid == IID_IUnknown || iid == DIID__IiTunesEvents)
	{
		++m_RefCount;
		*ppvObject = this;
		return S_OK;
	}

	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE CPlayerITunes::CEventHandler::AddRef()
{
	return ++m_RefCount;
}

ULONG STDMETHODCALLTYPE CPlayerITunes::CEventHandler::Release()
{
	return --m_RefCount;
}

HRESULT STDMETHODCALLTYPE CPlayerITunes::CEventHandler::Invoke(DISPID dispidMember, REFIID, LCID, WORD, DISPPARAMS* dispParams, VARIANT*, EXCEPINFO*, UINT*)
{
	switch (dispidMember)
	{
	case ITEventPlayerPlay:
		m_Player->OnStateChange(true);
		m_Player->OnTrackChange();
		break;

	case ITEventPlayerStop:
		m_Player->OnStateChange(false);
		break;

	case ITEventPlayerPlayingTrackChanged:
		m_Player->OnTrackChange();
		break;

	case ITEventSoundVolumeChanged:
		m_Player->OnVolumeChange(dispParams->rgvarg[0].intVal);
		break;

	case ITEventAboutToPromptUserToQuit:
		PostMessage(m_Player->m_CallbackWindow, WM_USER, ITEventAboutToPromptUserToQuit, 0);
		SetTimer(m_Player->m_CallbackWindow, TIMER_CHECKACTIVE, 500, NULL);
		break;
	}

	return S_OK;
}

/*
** CPlayerITunes
**
** Constructor.
**
*/
CPlayerITunes::CPlayerITunes() : CPlayer(),
	m_CallbackWindow(),
	m_LastCheckTime(0),
	m_iTunesActive(false),
	m_iTunes(),
	m_iTunesEvent()
{
	// Create windows class
    WNDCLASS wc;
    memset(&wc, 0, sizeof(wc));
    wc.hInstance = g_Instance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = L"NowPlayingITunesClass";
	RegisterClass(&wc);

	// Create callback window
	m_CallbackWindow = CreateWindow(L"NowPlayingITunesClass",
									L"CallbackWindow",
									WS_DISABLED,
									CW_USEDEFAULT,
									CW_USEDEFAULT,
									CW_USEDEFAULT,
									CW_USEDEFAULT,
									HWND_MESSAGE,
									NULL,
									g_Instance,
									this);
}

/*
** ~CPlayerITunes
**
** Destructor.
**
*/
CPlayerITunes::~CPlayerITunes()
{
	c_Player = NULL;

	DestroyWindow(m_CallbackWindow);
	UnregisterClass(L"NowPlayingITunesClass", g_Instance);

	Uninitialize();
}

/*
** Create
**
** Creates a shared class object.
**
*/
CPlayer* CPlayerITunes::Create()
{
	if (!c_Player)
	{
		c_Player = new CPlayerITunes();
	}

	return c_Player;
}

/*
** Initialize
**
** Initialize iTunes COM interface and event handler.
**
*/
void CPlayerITunes::Initialize()
{
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
	while (true)
	{
        HRESULT hr = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID*)&m_iTunes);
        if (hr == CO_E_SERVER_EXEC_FAILURE)
        {
            qDebug() << "iTunes appears to be busy.";
            // This seems to happen if there is a modal dialog being shown in iTunes
            // or some other delay has occurred. Retrying should do the trick.
            continue;
        }
        else if (hr != S_OK)
        {
            qWarning() << "Failed to get hold of iTunes instance via COM (" << hr << ")";
            CoUninitialize();
            m_iTunes = NULL;
        }
        //else qDebug() << "iTunes aquired via COM";
        break;
	}

	if (m_iTunes)
	{
		m_Initialized = true;

		// Set up event handler
		m_iTunesEvent = new CEventHandler(this);

		// Try getting track info and player state
		ITPlayerState state;
		if (SUCCEEDED(m_iTunes->get_PlayerState(&state)))
		{
			if (state == ITPlayerStateStopped)
			{
				// Determine if paused of stopped
				long position;
				m_iTunes->get_PlayerPosition(&position);

				if (position != 0)
				{
					m_State = PLAYER_PAUSED;
					OnTrackChange();
				}
			}
			else if (state == ITPlayerStatePlaying)
			{
				m_State = PLAYER_PLAYING;
				OnTrackChange();
			}

                        long volume = 0;
                        //m_iTunes->get_SoundVolume(&volume);
                        m_Volume = (UINT)volume;
		}
	}
	else
	{
		m_Initialized = false;
	}
}

/*
** Uninitialize
**
** Close iTunes COM interface.
**
*/
void CPlayerITunes::Uninitialize()
{
	if (m_Initialized)
	{
		m_Initialized = false;
		ClearData();

		m_iTunes->Release();
		delete m_iTunesEvent;
        CoUninitialize();
	}
}

/*
** WndProc
**
** Window procedure for the callback window.
**
*/
LRESULT CALLBACK CPlayerITunes::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static CPlayerITunes* player;
	switch (msg)
	{
	case WM_CREATE:
		// Get pointer to the CPlayerITunes class from the CreateWindow call
		player = (CPlayerITunes*)(((CREATESTRUCT*)lParam)->lpCreateParams);
		return 0;

	case WM_USER:
		if (wParam == ITEventAboutToPromptUserToQuit)
		{
			// Event handler calls this through a PostMessage when iTunes quits
			player->Uninitialize();
		}
		return 0;

	case WM_TIMER:
		if (wParam == TIMER_CHECKACTIVE)
		{
			if (!FindWindow(L"iTunesApp", L"iTunes") && !FindWindow(L"iTunes", L"iTunes"))
			{
				player->m_iTunesActive = false;
				KillTimer(hwnd, TIMER_CHECKACTIVE);
			}
		}
		return 0;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

/*
** CheckWindow
**
** Try to find iTunes periodically.
**
*/
bool CPlayerITunes::CheckWindow()
{
	DWORD time = GetTickCount();
	if (time - m_LastCheckTime > 5000)
	{
		m_LastCheckTime = time;

		if ((FindWindow(L"iTunesApp", L"iTunes") || FindWindow(L"iTunes", L"iTunes")) && !m_iTunesActive)
		{
                        m_iTunesActive = true;
			Initialize();
		}
	}

	return m_Initialized;
}

/*
** UpdateData
**
** Called during each update of the main measure.
**
*/
void CPlayerITunes::UpdateData()
{
	if ((m_Initialized || CheckWindow()) && m_State != PLAYER_STOPPED)
	{
		long position;
		m_iTunes->get_PlayerPosition(&position);
		m_Position = (UINT)position;
	}
}

/*
** OnTrackChange
**
** Called by iTunes event handler on track change.
**
*/
void CPlayerITunes::OnTrackChange()
{
	IITTrack* track;
	HRESULT hr = m_iTunes->get_CurrentTrack(&track);
	if (SUCCEEDED(hr) && track)
	{
                BSTR tmpStr;
		long tmpVal;

		// Get metadata
		track->get_Artist(&tmpStr);
                if(tmpStr) m_Artist = tmpStr; else m_Artist.clear();

		track->get_Name(&tmpStr);
                if(tmpStr) m_Title = tmpStr; else m_Title.clear();

		track->get_Album(&tmpStr);
                if(tmpStr) m_Album = tmpStr; else m_Album.clear();

		track->get_Duration(&tmpVal);
		m_Duration = (UINT)tmpVal;

		// Rating is 0 - 100, divide to 0 - 5
		track->get_Rating(&tmpVal);
		tmpVal /= 20L;
		m_Rating = (UINT)tmpVal;

		IITPlaylist* playlist;
		hr = track->get_Playlist(&playlist);
		if (SUCCEEDED(hr))
		{
			VARIANT_BOOL shuffle;
			hr = playlist->get_Shuffle(&shuffle);
			if (SUCCEEDED(hr))
			{
				m_Shuffle = (bool)shuffle;
			}

			ITPlaylistRepeatMode repeat;
			hr = playlist->get_SongRepeat(&repeat);
			if (SUCCEEDED(hr))
			{
				m_Repeat = (bool)repeat;
			}

			playlist->Release();
		}

		IITFileOrCDTrack* file;
                hr = track->QueryInterface(IID_IITFileOrCDTrack, (void**)&file);
		if (SUCCEEDED(hr))
		{
			file->get_Location(&tmpStr);
			file->Release();
			if (tmpStr && wcscmp(tmpStr, m_FilePath.c_str()) != 0)
			{
				++m_TrackCount;
				m_FilePath = tmpStr;

                                if ((m_Measures & MEASURE_COVER) && !CachedArtworkValid())
				{
                                        m_CoverPath.clear();

					// Check for embedded art through iTunes interface
					IITArtworkCollection* artworkCollection;
					hr = track->get_Artwork(&artworkCollection);
					if (SUCCEEDED(hr))
					{
                                                long count;
						artworkCollection->get_Count(&count);

						if (count > 0)
						{
                                                        IITArtwork* artwork;
							hr = artworkCollection->get_Item(1, &artwork);
                                                        if (SUCCEEDED(hr))
							{
                                                                ITArtworkFormat fmt;
                                                                QString extension = "png";

                                                                switch(artwork->get_Format(&fmt))
                                                                {
                                                                case S_FALSE:
                                                                    #ifdef iTunes_debug
                                                                        qDebug() << "iTunes: artwork: format unavailable";
                                                                    #endif
                                                                    break;
                                                                case S_OK:
                                                                    switch(fmt)
                                                                    {
                                                                    case ITArtworkFormatJPEG:
                                                                        extension = "jpg";
                                                                        break;
                                                                    case ITArtworkFormatPNG:
                                                                        extension = "png";
                                                                        break;
                                                                    case ITArtworkFormatBMP:
                                                                        extension = "png";
                                                                        break;
                                                                    case ITArtworkFormatUnknown:
                                                                    default:
                                                                        extension = "unknown";
                                                                        break;
                                                                    }
                                                                    #ifdef iTunes_debug
                                                                        qDebug() << "iTunes: artwork: format is " << extension;
                                                                    #endif
                                                                    if (extension=="unknown") extension = "jpg";
                                                                    break;
                                                                }
                                                                if(QFile::exists(m_artworkCache->cacheFile))
                                                                    QFile::remove(m_artworkCache->cacheFile);
                                                                UpdateCachedArtwork( QString("%0%1art.%2")
                                                                        .arg(m_artworkCache->cachePath)
                                                                        .arg(m_artworkCache->cachePath.endsWith("/")? "" : "/")
                                                                        .arg(extension) );

                                                                tmpStr = ::SysAllocString((const OLECHAR*)(m_artworkCache->cacheFile.replace("/","\\")).utf16());
                                                                hr = artwork->SaveArtworkToFile(tmpStr);
                                                                if (SUCCEEDED(hr))
                                                                {
                                                                        m_CoverPath = m_artworkCache->cacheFile.toStdWString();
                                                                }

                                                                artwork->Release();
                                                        }
						}

						artworkCollection->Release();
					}
				}

				if (m_Measures & MEASURE_LYRICS)
				{
					FindLyrics();
				}
			}
		}

		track->Release();
	}
	else
	{
		ClearData();
	}
}

/*
** OnStateChange
**
** Called by iTunes event handler on player state change.
**
*/
void CPlayerITunes::OnStateChange(bool playing)
{
	if (playing)
	{
		m_State = PLAYER_PLAYING;
	}
	else
	{
		// Guess if paused or stopped from track time
		m_State = (m_Position == 0) ? PLAYER_STOPPED : PLAYER_PAUSED;
	}
}

/*
** OnVolumeChange
**
** Called by iTunes event handler on volume change.
**
*/
void CPlayerITunes::OnVolumeChange(int volume)
{
	m_Volume = volume;
}

/*
** Pause
**
** Handles the Pause bang.
**
*/
void CPlayerITunes::Pause()
{
	m_iTunes->Pause();
}

/*
** Play
**
** Handles the Play bang.
**
*/
void CPlayerITunes::Play()
{
	m_iTunes->Play();
}

/*
** Stop
**
** Handles the Stop bang.
**
*/
void CPlayerITunes::Stop() 
{
	m_iTunes->Stop();
}

/*
** Next
**
** Handles the Next bang.
**
*/
void CPlayerITunes::Next() 
{
	m_iTunes->NextTrack();
}

/*
** Previous
**
** Handles the Previous bang.
**
*/
void CPlayerITunes::Previous() 
{
	m_iTunes->PreviousTrack();
}

/*
** SetPosition
**
** Handles the SetPosition bang.
**
*/
void CPlayerITunes::SetPosition(int position)
{
	m_iTunes->put_PlayerPosition((long)position);
}

/*
** SetRating
**
** Handles the SetRating bang.
**
*/
void CPlayerITunes::SetRating(int rating)
{
	IITTrack* track;
	HRESULT hr = m_iTunes->get_CurrentTrack(&track);
	if (SUCCEEDED(hr) && track)
	{
		rating *= 20;
		track->put_Rating((long)rating);
		track->Release();
	}
}

/*
** SetVolume
**
** Handles the SetVolume bang.
**
*/
void CPlayerITunes::SetVolume(int volume)
{
	m_iTunes->put_SoundVolume((long)volume);
}

/*
** SetShuffle
**
** Handles the SetShuffle bang.
**
*/
void CPlayerITunes::SetShuffle(bool state)
{
	IITTrack* track;
	HRESULT hr = m_iTunes->get_CurrentTrack(&track);
	if (SUCCEEDED(hr) && track)
	{
		IITPlaylist* playlist;
		hr = track->get_Playlist(&playlist);
		if (SUCCEEDED(hr))
		{
			m_Shuffle = state;
			VARIANT_BOOL shuffle = m_Shuffle ? VARIANT_TRUE : VARIANT_FALSE;
			playlist->put_Shuffle(shuffle);

			playlist->Release();
		}

		track->Release();
	}
}

/*
** SetRepeat
**
** Handles the SetRepeat bang.
**
*/
void CPlayerITunes::SetRepeat(bool state)
{
	IITTrack* track;
	HRESULT hr = m_iTunes->get_CurrentTrack(&track);
	if (SUCCEEDED(hr) && track)
	{
		IITPlaylist* playlist;
		hr = track->get_Playlist(&playlist);
		if (SUCCEEDED(hr))
		{
			m_Repeat = state;
			playlist->put_SongRepeat((ITPlaylistRepeatMode)m_Repeat);

			playlist->Release();
		}

		track->Release();
	}
}

/*
** ClosePlayer
**
** Handles the ClosePlayer bang.
**
*/
void CPlayerITunes::ClosePlayer()
{
	m_iTunes->Quit();
	Uninitialize();
	SetTimer(m_CallbackWindow, TIMER_CHECKACTIVE, 500, NULL);
}

/*
** OpenPlayer
**
** Handles the OpenPlayer bang.
**
*/
void CPlayerITunes::OpenPlayer(std::wstring& path)
{
	ShellExecute(NULL, L"open", path.empty() ? L"iTunes.exe" : path.c_str(), NULL, NULL, SW_SHOW);
}
