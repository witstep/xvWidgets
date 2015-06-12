#pragma once

#include <functional>
#include <opencv2/highgui.hpp>
#include <wx/panel.h>
#include <wx/timer.h>
#include <wx/stattext.h>
#include <wx/button.h>

namespace xv{

	class ImagePanel;

/** @brief Class to control the playback and processing of media obtained from VideoCapture
*/
class VideoPlayer : public wxPanel
{
	friend class Thread;
public:
	/// Constructor inherited from wxPanel
	VideoPlayer(wxWindow * parent,
		wxWindowID id = wxID_ANY,
		const wxPoint &pos = wxDefaultPosition,
		const wxSize &size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL,
		const wxString &name = wxPanelNameStr
		);

	/// Construct a video player and use videoCapture as media source
	VideoPlayer(cv::VideoCapture* videoCapture);

	virtual ~VideoPlayer();

	/// Returns the last frame number read from the media
	int getCurrentFrameIdx();

	/// Get the xv::Image instance used for rendering
	ImagePanel& getImage();

#pragma region control

	/// Try to open the named file
	virtual bool open(const cv::String &filename);

	/// Try to open device
	virtual bool open(int device);

	/// Start media loop
	virtual void play(); 
	
	/// Start media loop and execute callback for each frame
	virtual void play(
		std::function<void(cv::Mat &)> callback,
		std::function<void(cv::Mat &)> postProcessCallback = [](cv::Mat &){});

	/// Pause media loop 
	virtual void pause();

	/// Stops the playback thread
	virtual void stop();

	/// Move playback to frame
	virtual void seek(int);

#pragma endregion control

#pragma region event handling
	/// The user dragged the slider
	virtual void onSliderMove(wxScrollEvent&);

	/// The user clicked the slider
	virtual void onSliderClickDown(wxMouseEvent&);

	/// The user released the mouse button after clicking the slider
	virtual void onSliderClickUp(wxMouseEvent&);

	/// The user clicked the playback control button
	virtual void onPlayClick(wxCommandEvent&);

	/// The user moved the mouse
	virtual void onMouseMove(wxCommandEvent& evt);
#pragma endregion event handling

private:
	/// Shared implementation for int and cv::String
	template <typename _Tp>
	bool open_(const _Tp &deviceOrFilename);

	/// How often to update the timer labels
	static const int LABEL_UPDATE_INTERVAL;

	/// Displayed in timer labels while duration is not available
	static const char* LABEL_DEFAULT_TEXT;

	std::function<void(cv::Mat &)>
		m_preProcessCallback = [](cv::Mat &){},
		m_postProcessCallback = [](cv::Mat &){};

	int 
		m_fps = 0,
		m_frameCount = 0;

	wxTimer m_timer;

	/// used to handle redraw after a resize
	void sizeEvent(wxSizeEvent&);
	void onTimer(wxTimerEvent&);

	/// populate the label with hh::mm::ss corresponding to the frame
	void showTimeLabel(wxStaticText* label,int frame);

	std::function<void(cv::Mat &)> m_processCallback = [](cv::Mat &){};

	enum playbackState{
		idle,   //no video loaded
		playing,//ongoing playback
		paused, //video is paused
		seeking //looking for a frame
	} m_state = paused;

	
	cv::VideoCapture* m_videoCapture = NULL;
	ImagePanel* m_image;
	wxButton* m_playButton;
	wxSlider* m_slider;
	wxStaticText
		*m_positionLabel,
		*m_durationLabel;
	wxSizer *m_sizer;

	wxMutex m_mutex;
	class Thread : public wxThread{
	public:
		Thread(VideoPlayer* const);
		virtual void *Entry();
	private:
		VideoPlayer *m_player;
	} *m_thread;

	void init();

	DECLARE_EVENT_TABLE()
};

}//namespace