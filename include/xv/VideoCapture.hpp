#pragma once

#include <filesystem>

#include "wx/wx.h"
#include <wx/panel.h>
#include <wx/filepicker.h>
#include <wx/propgrid/propgrid.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <map>

#include "xv/Image.hpp"

namespace xv{

	/** @brief Analogue to cv::VideoCapture but providing a GUI with file picker and display of properties */
	class VideoCapture : public wxPanel
	{

	public:
		/// Default constructor
		VideoCapture();

		/// Constructor inherited from wxPanel
		VideoCapture(wxWindow * parent,
			wxWindowID id = wxID_ANY,
			const wxPoint &pos = wxDefaultPosition,
			const wxSize &size = wxDefaultSize,
			long style = wxTAB_TRAVERSAL,
			const wxString &name = wxPanelNameStr
		);

		/// Constructor inherited from cv::VideoCapture
		VideoCapture(const std::string& filename);

		/// Constructor inherited from cv::VideoCapture
		VideoCapture(int device);
		
#pragma region cv::VideoCapture	
		/// Open video file or a capturing device for video capturing.
		bool open(const std::string& filename);
		/// Returns true if video capturing has been initialized already.
		bool isOpened(){ return m_videoCapture.isOpened(); };

		/// Closes video file or capturing device.
		void release(){ return m_videoCapture.release(); };

		/// Grabs the next frame from video file or capturing device
		bool grab(){ return m_videoCapture.grab(); }

		/// Decodes and returns the grabbed video frame.
		bool retrieve(cv::Mat& image, int channel = 0);

		/// Grabs, decodes and returns the next video frame.
		bool read(cv::Mat& image){ return m_videoCapture.read(image); };

		/// Returns the specified VideoCapture property
		double get(int propId){ return m_videoCapture.get(propId); }; 

		/// Sets a property in the VideoCapture.
		bool set(int propId, double value){ return m_videoCapture.set(propId, value); };

		/// Retrieves a image frame from the opened media into a cv::Mat.
		void VideoCapture::operator>>(cv::Mat& mat){ m_videoCapture >> mat; };

		/// Retrieves a image frame from the opened media into an xv::Image
		friend void operator>>(VideoCapture&, Image_<int>&);

#pragma endregion cv::VideoCapture

	private:
		const static std::map <int, std::string> PROPERTY_MAP;
		const std::map < int, wxPGProperty*>  m_properties;
		static const std::string CLASS_NAME;
		cv::VideoCapture m_videoCapture;

#pragma region layout
		void init();
		void initFilePicker(wxSizer*);
		void initPropertyGrid(wxSizer*);
		void initButtons(wxSizer*);
#pragma endregion layout

#pragma region event handling
		void evtFilePickerChanged(wxFileDirPickerEvent&);
		void evtOK(wxCommandEvent&);
		void evtCancel(wxCommandEvent&);
#pragma endregion event handling
		
		wxFilePickerCtrl* m_filePickerCtrl;
		wxPropertyGrid* m_propertyGrid;
		std::tr2::sys::path m_filename;
		int m_device;
		bool m_selfHosted;//created its own parent window or embbeded in another control

		void loadProperties();
		void clearProperties();
	};

}

