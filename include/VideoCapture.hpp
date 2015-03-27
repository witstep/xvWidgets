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

#include "Mat.hpp"

namespace xvWidgets{

	class VideoCapture : public wxPanel
	{

	public:
		static const std::string CLASS_NAME;
		VideoCapture(wxWindow * parent,
			wxWindowID id = wxID_ANY,
			const wxPoint &pos = wxDefaultPosition,
			const wxSize &size = wxSize(720, 480),
			long style = wxTAB_TRAVERSAL,
			const wxString &name = wxPanelNameStr
		);
		VideoCapture();
		VideoCapture(const std::string& filename);
		VideoCapture(int device);
		const static std::map <int, std::string> PROPERTY_MAP;
		const std::map < int, wxPGProperty*>  m_properties;

#pragma region cv::VideoCapture
		bool open(const std::string& filename);
		bool isOpened(){ return m_videoCapture.isOpened(); };
		void release(){ return m_videoCapture.release(); };
		bool grab(){ return m_videoCapture.grab(); }
		bool retrieve(cv::Mat& image, int channel = 0);
		bool read(cv::Mat& image){ return m_videoCapture.read(image); };
		double get(int propId){ return m_videoCapture.get(propId); };
		bool set(int propId, double value){ return m_videoCapture.set(propId, value); };
		void VideoCapture::operator>>(cv::Mat& mat){ m_videoCapture >> mat; };
		friend void operator>>(VideoCapture&, Mat&);

#pragma endregion cv::VideoCapture

	private:
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

