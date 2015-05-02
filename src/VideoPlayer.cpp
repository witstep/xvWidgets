#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <windows.h>

#include "xv/VideoPlayer.hpp"

using namespace xv;

VideoPlayer::VideoPlayer(
	wxWindow *parent,
	wxWindowID id,
	const wxPoint &pos,
	const wxSize &size,
	long style,
	const wxString &name
	) 
	: wxPanel(parent, id, pos, size, style, name)
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
	m_videoCapture = new cv::VideoCapture();
	m_playbackThread = new VideoPlaybackThread(this);

	m_image = new Image(this);
	m_playButton = new wxButton(this, wxID_ANY, _("Play"));
	m_playButton->Bind(wxEVT_BUTTON, &VideoPlayer::onPlayClick, this);

	/// slider
	m_slider = new wxSlider(this, wxID_ANY, 0, 0, 1);
	m_slider->Bind(wxEVT_LEFT_DOWN, &VideoPlayer::onSliderClickDown, this);
	m_slider->Bind(wxEVT_LEFT_UP, &VideoPlayer::onSliderClickUp, this);
	m_slider->Bind(wxEVT_SCROLL_CHANGED, &VideoPlayer::onSliderMove, this);

	m_durationLabel = new wxStaticText(this, wxID_ANY, "00:00/00:00");
	m_filenameLabel = new wxStaticText(this, wxID_ANY, "");

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* controlSizer = new wxBoxSizer(wxHORIZONTAL);

	//controls
	controlSizer->Add(m_playButton);
	m_slider->Bind(wxEVT_LEFT_DOWN, &VideoPlayer::onSliderClickDown, this);

	controlSizer->Add(m_slider, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);
	controlSizer->Add(m_durationLabel, 0, wxALIGN_CENTER_VERTICAL);
	controlSizer->Add(m_filenameLabel, 0, wxALIGN_CENTER_VERTICAL);

	//main layout
	mainSizer->Add(m_image, 1, wxALL | wxEXPAND);
	mainSizer->Add(controlSizer,0,wxALL | wxEXPAND,5);

	SetSizer(mainSizer);
}

VideoPlayer::~VideoPlayer()
{

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
}


void VideoPlayer::onSliderClickUp(wxMouseEvent& evt){
	m_state = VideoPlayer::playing;
	m_image->SetFocus(); //avoid ugly focus around slider
	evt.Skip();
}

void VideoPlayer::process()
{
	m_processCallback(*m_image);
}

bool VideoPlayer::open(const cv::String &filename)
{
	wxBusyCursor busy;
	m_slider->SetValue(0); /// rewind to first frame

	if (m_videoCapture->open(filename)){
		m_slider->SetRange(1, (int)m_videoCapture->get(cv::CAP_PROP_FRAME_COUNT));
		m_state = playbackState::paused;
		return true;
	}

	m_slider->SetRange(0, 1);
	m_state = playbackState::idle;
	return false;
}

bool VideoPlayer::open(int device)
{
	wxBusyCursor busy;
	m_slider->SetValue(0); /// rewind to first frame

	if (m_videoCapture->open(device)){
		m_slider->SetRange(1, (int)m_videoCapture->get(cv::CAP_PROP_FRAME_COUNT));
		m_state = playbackState::paused;
		return true;
	}

	m_slider->SetRange(0, 1);
	m_state = playbackState::idle;
	return false;
}

void VideoPlayer::play()
{
	wxBusyCursor busy;
	if ( m_state == playbackState::idle )
		return; /// can't play if media not open
	///launch the playback thread, if not running
	if (!m_playbackThread->IsRunning())
		m_playbackThread->Run();

	m_state = VideoPlayer::playing;
	m_playButton->SetLabel(_("Pause"));
}

void VideoPlayer::play(std::function<void(cv::Mat &)> callback)
{
	m_image->setPreProcessing(callback);
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

VideoPlaybackThread::VideoPlaybackThread(VideoPlayer* player)
	:m_player(player)
{
	
}

void* VideoPlaybackThread::Entry()
{
	while (!TestDestroy()){
		int i = (int) m_player->m_videoCapture->get(cv::CAP_PROP_POS_FRAMES);

		if (i == 0) /// the first frame of a new media source can be loaded with a delay
			wxBeginBusyCursor();

		m_player->m_slider->SetValue(i);
		m_player->m_mutex.Lock();
		*m_player->m_videoCapture >> *m_player->m_image;
		m_player->m_mutex.Unlock();
		m_player->process();

		if (i == 0) 
			wxEndBusyCursor();

		while (m_player->m_state == VideoPlayer::seeking || m_player->m_state == VideoPlayer::paused)
			Sleep(1000); /// to-do: why not lock a mutex here or use Pause()?
	}
	return NULL;
}
