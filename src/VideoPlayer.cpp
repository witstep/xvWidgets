#include <wx/wxprec.h>
#include <wx/numformatter.h>

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <windows.h>

#include "xv/VideoPlayer.hpp"

using namespace xv;

const int VideoPlayer::LABEL_UPDATE_INTERVAL = 750;
const char* VideoPlayer::LABEL_DEFAULT_TEXT = "--:--";

BEGIN_EVENT_TABLE(VideoPlayer, wxPanel)
	EVT_TIMER(wxID_HIGHEST + 1, VideoPlayer::onTimer)
END_EVENT_TABLE()

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

VideoPlayer::VideoPlayer(cv::VideoCapture* videoCapture)
	:m_videoCapture(videoCapture)
{
	assert(("Not implemented", false));
}

void VideoPlayer::init()
{

	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));

	m_videoCapture = new cv::VideoCapture();
	m_thread = new Thread(this);

	m_image = new Image(this);
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
	m_slider->Bind(wxEVT_LEFT_DOWN, &VideoPlayer::onSliderClickDown, this);

	controlSizer->Add(m_slider, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
	controlSizer->Add(m_positionLabel, 0, wxALIGN_CENTER_VERTICAL);
	controlSizer->Add(separator, 0, wxALIGN_CENTER_VERTICAL);
	controlSizer->Add(m_durationLabel, 0, wxALIGN_CENTER_VERTICAL);

	//main layout
	m_sizer->Add(m_image, 1, wxALL | wxEXPAND);
	m_sizer->Add(controlSizer, 0, wxALL | wxEXPAND, 5);

	SetSizer(m_sizer);
}

VideoPlayer::~VideoPlayer()
{

}

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

	m_mutex.Lock();

	wxBeginBusyCursor();
	m_videoCapture->set(cv::CAP_PROP_POS_FRAMES, m_slider->GetValue());
	wxEndBusyCursor();

	m_mutex.Unlock();
}

void VideoPlayer::onSliderClickDown(wxMouseEvent& evt)
{
	m_state = VideoPlayer::seeking;
	evt.Skip();
	m_timer.Stop(); ///don't update time labels while moving slider
}


void VideoPlayer::onSliderClickUp(wxMouseEvent& evt){
	m_state = VideoPlayer::playing;
	evt.Skip();
	m_timer.Start();
}

template <typename _Tp>
bool VideoPlayer::open_(const _Tp &deviceOrFilename)
{
	wxBusyCursor busy;
	m_slider->SetValue(0); /// rewind to first frame
	m_durationLabel->SetLabel(LABEL_DEFAULT_TEXT);

	if (m_videoCapture->open(deviceOrFilename)){
		m_fps = m_videoCapture->get(cv::CAP_PROP_FPS);
		m_frameCount = m_videoCapture->get(cv::CAP_PROP_FRAME_COUNT);
		showTimeLabel(m_durationLabel, m_frameCount);
		m_slider->SetRange(1, (int)m_frameCount);
		m_timer.Start(LABEL_UPDATE_INTERVAL);
		m_state = playbackState::paused;
		return true;
	}
	m_timer.Stop();
	m_slider->SetRange(0, 1);
	m_state = playbackState::idle;
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
	wxBusyCursor busy;
	if ( m_state == playbackState::idle )
		return; /// can't play if media not open
	///launch the playback thread, if not running
	if (!m_thread->IsRunning())
		m_thread->Run();

	m_state = VideoPlayer::playing;
	m_playButton->SetLabel(_("Pause"));
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
	if (m_state == playbackState::idle)
		return; /// can't pause if media not open
	m_state = VideoPlayer::paused;
	m_playButton->SetLabel(_("Play"));
}

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

	int totalSeconds = videoFrame / m_fps;

	int hours = totalSeconds / 3600;
	int totalMinutes = totalSeconds / 60;
	int minutes = totalMinutes % 60;
	int seconds = totalSeconds % 60;

	if (hours > 0) /// more than an hour
		label->SetLabel(
			wxString::Format("%02d:%02d:%02d",
			hours,
			minutes,
			seconds)
		);
	else
		label->SetLabel(
			wxString::Format("%02d:%02d",
			minutes,
			seconds)
		);
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


		m_player->m_slider->SetValue(i);

		m_player->m_mutex.Lock();

		*m_player->m_videoCapture >> mat;
		m_player->m_preProcessCallback(mat);
		*m_player->m_image << mat;
		m_player->m_postProcessCallback(mat);

		m_player->m_mutex.Unlock();

		if (i == 0) 
			wxEndBusyCursor();

		while (m_player->m_state == VideoPlayer::seeking || m_player->m_state == VideoPlayer::paused)
			Sleep(1000); /// to-do: why not lock a mutex here or use Pause()?
	}
	return NULL;
}
