#pragma once;

#if defined(wxUSE_GUI)
#include <wx/wxprec.h>
#include <wx/numformatter.h>

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#elif defined(QT_GUI_LIB)

#endif

#include <windows.h>

#include "xv/VideoPlayer.hpp"
#include "xv/ImagePanel.hpp"

#include <cassert>
#include <iostream>

using namespace xv;
using namespace std;

const int VideoPlayer::LABEL_UPDATE_INTERVAL = 750;
const char* VideoPlayer::LABEL_DEFAULT_TEXT = "-:-";

#if defined(wxUSE_GUI)
BEGIN_EVENT_TABLE(VideoPlayer, wxPanel)
	EVT_TIMER(wxID_HIGHEST + 1, VideoPlayer::onTimer)
END_EVENT_TABLE()
#endif

#if defined(wxUSE_GUI)
void VideoPlayer::onTimer(wxTimerEvent& event)
{
	showTimeLabel(m_positionLabel, m_videoCapture->get(cv::CAP_PROP_POS_FRAMES));
}

void VideoPlayer::sizeEvent(wxSizeEvent& evt)
{
	/// required because GUI thread is not aware the playback thread is
	/// updating the duration labels
	//m_sizer->RecalcSizes();
}

VideoPlayer::VideoPlayer(
	wxWindow *parent,
	wxWindowID id,
	const wxPoint &pos,
	const wxSize &size,
	long style,
	const wxString &name
	) 
	: m_timer(this, wxID_HIGHEST+1),
	wxPanel(parent, id, pos, size, style, name)
{
	init();
}

#endif

VideoPlayer::VideoPlayer(cv::VideoCapture* videoCapture)
	:m_videoCapture(videoCapture)
{
	assert(("Not implemented", false));
}

#if defined(wxUSE_GUI)
void VideoPlayer::init()
{

	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));

	m_videoCapture = new cv::VideoCapture();
	m_thread = new Thread(this);

	m_image = new ImagePanel(this);
	m_playButton = new wxButton(this, wxID_ANY, _("Play"));
	m_playButton->Bind(wxEVT_BUTTON, &VideoPlayer::onPlayClick, this);

	/// slider
	m_slider = new wxSlider(this, wxID_ANY, 0, 0, 1);
	m_slider->Bind(wxEVT_LEFT_DOWN, &VideoPlayer::onSliderClickDown, this);
	m_slider->Bind(wxEVT_LEFT_UP, &VideoPlayer::onSliderClickUp, this);
	m_slider->Bind(wxEVT_SCROLL_CHANGED, &VideoPlayer::onSliderMove, this);
	m_slider->Bind(wxEVT_SLIDER, &VideoPlayer::onMouseMove, this);

	m_positionLabel = new wxStaticText(this, wxID_ANY, LABEL_DEFAULT_TEXT);
	wxStaticText *separator = new wxStaticText(this, wxID_ANY, "/");
	m_durationLabel = new wxStaticText(this, wxID_ANY, LABEL_DEFAULT_TEXT); 

	m_sizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* controlSizer = new wxBoxSizer(wxHORIZONTAL);
	
	//controls
	controlSizer->Add(m_playButton);
	controlSizer->Add(m_slider, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
	controlSizer->Add(m_positionLabel, 0, wxALIGN_CENTER_VERTICAL);
	controlSizer->Add(separator, 0, wxALIGN_CENTER_VERTICAL);
	controlSizer->Add(m_durationLabel, 0, wxALIGN_CENTER_VERTICAL);

	//main layout
	m_sizer->Add(m_image, 1, wxALL | wxEXPAND);
	m_sizer->Add(controlSizer, 0, wxALL | wxEXPAND, 5);

	SetSizer(m_sizer);
}
#elif defined(QT_GUI_LIB)
void VideoPlayer::init()
{

}
#endif

VideoPlayer::~VideoPlayer()
{
#if defined(wxUSE_GUI)
	m_timer.DeletePendingEvents();
	if (m_thread)
		m_thread->Delete();
#endif
}

#if defined(wxUSE_GUI)
void VideoPlayer::onMouseMove(wxCommandEvent& evt)
{
	if (m_state == VideoPlayer::seeking)
		showTimeLabel(m_positionLabel, m_slider->GetValue());
	evt.Skip();
}

void VideoPlayer::onSliderMove(wxScrollEvent& evt)
{
	if (m_state == idle)
		return; ///can't seek if media not open
	seek(m_slider->GetValue());
}

void VideoPlayer::onSliderClickDown(wxMouseEvent& evt)
{
	m_timer.Stop(); ///don't update time labels while moving slider
	if (m_state != VideoPlayer::seeking){
		m_previousState = m_state;
		setState(VideoPlayer::seeking);
		pause();
	}
	evt.Skip();
}


void VideoPlayer::onSliderClickUp(wxMouseEvent& evt){
	setState(m_previousState); // return to the state it was before sliding
	if (m_previousState == playbackState::playing)
		play();
	else if (m_previousState == playbackState::paused)
		pause();
	evt.Skip();
	m_timer.Start();
}
#endif

template <typename _Tp>
bool VideoPlayer::open_(const _Tp &deviceOrFilename)
{
#if defined(wxUSE_GUI)
	wxBusyCursor busy;
	m_slider->SetValue(0); /// rewind to first frame
	m_durationLabel->SetLabel(LABEL_DEFAULT_TEXT);

	if (m_videoCapture->open(deviceOrFilename)){
		m_fps = m_videoCapture->get(cv::CAP_PROP_FPS);
		m_frameCount = m_videoCapture->get(cv::CAP_PROP_FRAME_COUNT);
		showTimeLabel(m_durationLabel, m_frameCount);
		m_slider->SetRange(1, (int)m_frameCount);
		m_timer.Start(LABEL_UPDATE_INTERVAL);
		setState(playbackState::paused);
		return true;
	}
	m_timer.Stop();
	m_slider->SetRange(0, 1);
	setState(playbackState::idle);
#endif
	return false;
}

bool VideoPlayer::open(const cv::String &filename)
{
	return open_<cv::String>(filename);
}

bool VideoPlayer::open(int device)
{
	return open_<int>(device);
}


void VideoPlayer::play()
{
#if defined(wxUSE_GUI)
	wxBusyCursor busy;

	if ( m_state == playbackState::idle )
		return; /// can't play if media not open
	///launch the playback thread, if not running
	if (!m_thread->IsRunning())
		m_thread->Run();

	setState(VideoPlayer::playing);
	m_playButton->SetLabel(_("Pause"));
#endif
}

void VideoPlayer::play(
	std::function<void(cv::Mat &)> preProcessCallback,
	std::function<void(cv::Mat &)> postProcessCallback)
{
	m_preProcessCallback = preProcessCallback;
	m_postProcessCallback = postProcessCallback;
	play();
}

void VideoPlayer::pause()
{
#if defined(wxUSE_GUI)
	if (m_state == playbackState::idle)
		return; /// can't pause if media not open
	setState(VideoPlayer::paused);
	m_playButton->SetLabel(_("Play"));
#endif
}

void VideoPlayer::stop()
{
#if defined(wxUSE_GUI)
	m_thread->Delete();
	m_thread = NULL;
	Sleep(250);
#endif
}

void VideoPlayer::seek(int idx)
{
#if defined(wxUSE_GUI)

#ifdef _DEBUG
	cout << "VideoPlayer::seek - locking mutex:" << idx << endl;
#endif

	wxBeginBusyCursor();
	m_mutex.Lock();
	m_videoCapture->set(cv::CAP_PROP_POS_FRAMES, idx);
	m_mutex.Unlock();
	wxEndBusyCursor();

#ifdef _DEBUG
	cout << "VideoPlayer::seek - unlocking mutex:" << idx << endl;
#endif

#endif
}

#if defined(wxUSE_GUI)
void VideoPlayer::onPlayClick(wxCommandEvent& evt)
{
	if (m_state == VideoPlayer::playing){
		pause();
	} else if (m_state == VideoPlayer::paused){
		play();
	}
}


void VideoPlayer::showTimeLabel(wxStaticText* label, int videoFrame)
{

	
	if (m_fps < 1)
		return; /// property no available

	wxString text;
	bool sizeChanged = false;
	int totalSeconds = videoFrame / m_fps;

	int hours = totalSeconds / 3600;
	int totalMinutes = totalSeconds / 60;
	int minutes = totalMinutes % 60;
	int seconds = totalSeconds % 60;



	if (hours > 0) /// more than an hour
		text = wxString::Format("%02d:%02d:%02d",
			hours,
			minutes,
			seconds
		);
	else
		text = wxString::Format("%02d:%02d",
			minutes,
			seconds
		);

	//sizer needs to reflow the controls
	sizeChanged = label->GetLabel().Len() != text.Len();

	label->SetLabel(text);

	if (sizeChanged)
		m_sizer->RecalcSizes();
}



VideoPlayer::Thread::Thread(VideoPlayer* player)
	:m_player(player)
{
	
}

void* VideoPlayer::Thread::Entry()
{
	cv::Mat mat;
	while (!TestDestroy()){
		int i = (int) m_player->m_videoCapture->get(cv::CAP_PROP_POS_FRAMES);
		if (i == 0) /// the first frame of a new media source can be loaded with a delay
			wxBeginBusyCursor();

		m_player->m_mutex.Lock();
		if (TestDestroy())
			break;

		*m_player->m_videoCapture >> mat;
		if (mat.empty())
			continue;
		m_player->m_preProcessCallback(mat);
		*m_player->m_image << mat;
		m_player->m_postProcessCallback(mat);

		m_player->m_mutex.Unlock();

		if (i == 0) 
			wxEndBusyCursor();

		while (m_player->m_state == VideoPlayer::seeking || m_player->m_state == VideoPlayer::paused)
			if (!TestDestroy())
				Sleep(1000); /// to-do: why not lock a mutex here or use Pause()?
			else
				break;

		m_player->m_slider->SetValue(i);
	}

	return NULL;
}
#endif

int VideoPlayer::getCurrentFrameIdx()
{
	return m_videoCapture->get(cv::CAP_PROP_POS_FRAMES);
}

ImagePanel& VideoPlayer::getImage()
{
	return *m_image;
}

void VideoPlayer::setState(playbackState state)
{
	m_state = state;
}