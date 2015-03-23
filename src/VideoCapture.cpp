#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/sizer.h>

#include "../include/VideoCapture.hpp"
#include <sstream>

using namespace cvWidgets;
using namespace std;
using namespace std::tr2;

const string VideoCapture::CLASS_NAME("VideoCapture");

const map <int, string > VideoCapture::PROPERTY_MAP({
	pair < int, string >(cv::CAP_PROP_FPS, "CAP_PROP_FPS"),
	pair < int, string >(cv::CAP_PROP_FRAME_COUNT, "CAP_PROP_FRAME_COUNT"),
	pair < int, string >(cv::CAP_PROP_FRAME_WIDTH, "CAP_PROP_FRAME_WIDTH"),
	pair < int, string >(cv::CAP_PROP_FRAME_HEIGHT, "CAP_PROP_FRAME_HEIGHT")
});

VideoCapture::VideoCapture(wxWindow* parent) :
	wxPanel(parent),
	m_selfHosted(false)
{
	init();
}

VideoCapture::VideoCapture() :
	m_selfHosted(true)
{
	init();
	static_cast<wxDialog*>(m_parent)->ShowModal();
}

VideoCapture::VideoCapture(const std::string& filename) :
	m_filename(filename)
{
	m_videoCapture.open(m_filename.string());

	init();
}

VideoCapture::VideoCapture(int device) :
	m_device(device)
{
	stringstream ss;
	ss << "device " << m_device;
	m_filename = ss.str();

	m_videoCapture.open(device);

	init();
}

bool VideoCapture::open(const string& filename)
{
	m_filename = filename;
	return m_videoCapture.open(filename);
}


#pragma region layout
void VideoCapture::init()
{
	if (m_parent == NULL)
		this->SetParent(new wxDialog(NULL, -1, _T("VideoCapture"),wxDefaultPosition,wxDefaultSize));

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	m_parent->SetSizeHints(wxDefaultSize, wxDefaultSize);
	
	initFilePicker(mainSizer);
	initPropertyGrid(mainSizer);

	if (m_selfHosted)
		initButtons(mainSizer);//will have OK/Cancel buttons

	m_parent->SetSizer(mainSizer);
	m_parent->Layout();

	m_parent->Centre(wxBOTH);
}

void VideoCapture::initFilePicker(wxSizer* mainSizer)
{
	m_filePickerCtrl = new wxFilePickerCtrl(
		this->m_parent,
		wxID_ANY,
		m_filename.string(),
		_("Open Video"),
		"Video Files (*.avi; *.mov, *mp4)|*.avi; *.mov; *.mp4",
		wxDefaultPosition,
		wxDefaultSize,
		wxFLP_DEFAULT_STYLE
		);

	m_filePickerCtrl->Bind(wxEVT_FILEPICKER_CHANGED, &VideoCapture::evtFilePickerChanged, this);
	mainSizer->Add(m_filePickerCtrl, 0, wxALL | wxEXPAND, 5);
}

void VideoCapture::initPropertyGrid(wxSizer* mainSizer)
{
	m_propertyGrid = new wxPropertyGrid(this->m_parent, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	m_propertyGrid->Append(new wxPropertyCategory("", CLASS_NAME));
	for (auto property : PROPERTY_MAP)
		m_propertyGrid->Append(new wxFloatProperty(property.second, property.second, m_videoCapture.get(property.first)))
		->Enable(false);

	mainSizer->Add(m_propertyGrid, 1, wxALL | wxEXPAND, 5);
}

void VideoCapture::initButtons(wxSizer* mainSizer)
{
	wxButton *okButton = new wxButton(this->m_parent, wxID_OK);
	wxButton *cancelButton = new wxButton(this->m_parent, wxID_CANCEL);
	wxStdDialogButtonSizer* buttonSizer = new wxStdDialogButtonSizer;

	buttonSizer->SetAffirmativeButton(okButton);
	buttonSizer->SetNegativeButton(cancelButton);

	buttonSizer->Realize();

	//okButton->Bind(wxEVT_BUTTON, &VideoCapture::evtOK, this);
	//cancelButton->Bind(wxEVT_BUTTON, &VideoCapture::evtCancel, this);

	mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxALL, 5);
}
#pragma endregion layout

#pragma region event handling
void VideoCapture::evtFilePickerChanged(wxFileDirPickerEvent& event){
	m_filename = event.GetPath();

	if (!sys::exists(m_filename)){
		clearProperties();
		return;
	}

	loadProperties();
	
	static_cast<wxFrame*>(m_parent)->GetEventHandler()->AddPendingEvent(
		event
	);
}

void VideoCapture::evtOK(wxCommandEvent& WXUNUSED(event))
{
	m_parent->GetEventHandler()->AddPendingEvent(wxCloseEvent());
}

void VideoCapture::evtCancel(wxCommandEvent& WXUNUSED(event))
{
	m_filename = "";
	m_videoCapture.open(cv::String());
	m_parent->GetEventHandler()->AddPendingEvent(wxCloseEvent());
}

#pragma endregion event handling

void VideoCapture::loadProperties()
{
	if (!m_videoCapture.open(m_filename.string())){
		wxMessageBox(_("File could not be opened!"), _("Error"), wxOK);
		return;
	}

	m_propertyGrid->GetProperty(CLASS_NAME)
		->SetLabel(m_filename.filename());

	for (auto property : PROPERTY_MAP)
		m_propertyGrid->GetProperty(property.second)
			->SetValue(m_videoCapture.get(property.first));

	m_propertyGrid->Refresh();
}

void VideoCapture::clearProperties()
{
	m_propertyGrid->GetProperty(CLASS_NAME)
		->SetLabel("");

	for (auto property : PROPERTY_MAP)
		m_propertyGrid->GetProperty(property.second)
		->SetValue(NULL);

	m_propertyGrid->Refresh();
}

