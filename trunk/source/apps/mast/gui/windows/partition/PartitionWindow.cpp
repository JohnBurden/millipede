/***
 * millipede: PartitionWindow.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "PartitionWindow.h"

#include <wx/sizer.h>
#include <wx/stattext.h>

#include <common/dicom/volumes/Volume.h>
#include <common/dicom/volumes/VolumeTextureSet.h>
#include <mast/util/StringConversion.h>
#include "PartitionCanvas.h"
#include "StratumCanvas.h"
#include "ViewedVolumeModel.h"

namespace mp {

//#################### CONSTRUCTORS ####################
PartitionWindow::PartitionWindow(wxWindow *parent, const std::string& title, const Volume_Ptr& volume, const VolumeChoice& volumeChoice, wxGLContext *context)
:	wxFrame(parent, -1, string_to_wxString(title), wxDefaultPosition, wxSize(100,100)),
	m_model(new ViewedVolumeModel), m_oldViewLocation(-1, -1), m_volumeChoice(volumeChoice)
{
	m_model->m_viewLocation.reset(new ViewedVolumeModel::ViewLocation(-1, -1));
	m_model->m_volume = volume;

	calculate_canvas_size();

	Show();

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);

	//~~~~~~~~~~~~~~~~~~~~~~~
	// Construct the top half
	//~~~~~~~~~~~~~~~~~~~~~~~

	wxPanel *top = new wxPanel(this);
	wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
	top->SetSizer(topSizer);
	sizer->Add(top, 0, wxALIGN_CENTER_HORIZONTAL);

	int attribList[] =
	{
		WX_GL_RGBA,
		WX_GL_DEPTH_SIZE,
		16,
		WX_GL_DOUBLEBUFFER
	};

	// Top left
	m_stratumCanvas = new StratumCanvas(top, context, attribList, wxID_ANY, wxDefaultPosition, wxSize(m_canvasWidth, m_canvasHeight));
	topSizer->Add(m_stratumCanvas);

	// Top middle
	wxPanel *middle = new wxPanel(top);
	wxBoxSizer *middleSizer = new wxBoxSizer(wxVERTICAL);
	middle->SetSizer(middleSizer);
		wxStaticText *stratumText = new wxStaticText(middle, wxID_ANY, wxT("Stratum"));
		middleSizer->Add(stratumText, 0, wxALIGN_CENTER_HORIZONTAL);
		m_stratumSlider = new wxSlider(middle, wxID_ANY, 999, 999, 999, wxDefaultPosition, wxDefaultSize, wxVERTICAL|wxSL_AUTOTICKS|wxSL_LABELS|wxSL_LEFT);
		middleSizer->Add(m_stratumSlider, 0, wxALIGN_CENTER_HORIZONTAL);
	topSizer->Add(middle, 0, wxALIGN_CENTER_VERTICAL);

	// Top right
	wxPanel *right = new wxPanel(top);
	wxBoxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);
	right->SetSizer(rightSizer);
		m_partitionCanvas = new PartitionCanvas(right, get_context(), attribList, wxID_ANY, wxDefaultPosition, wxSize(m_canvasWidth, m_canvasHeight));
		rightSizer->Add(m_partitionCanvas);

		wxPanel *bottom = new wxPanel(right);
		wxBoxSizer *bottomSizer = new wxBoxSizer(wxVERTICAL);
		bottom->SetSizer(bottomSizer);
			wxStaticText *layerText = new wxStaticText(bottom, wxID_ANY, wxT("Layer"));
			bottomSizer->Add(layerText, 0, wxALIGN_CENTER_HORIZONTAL);
			m_layerSlider = new wxSlider(bottom, wxID_ANY, 999, 999, 999, wxDefaultPosition, wxDefaultSize, wxHORIZONTAL|wxSL_AUTOTICKS|wxSL_LABELS|wxSL_TOP);
			bottomSizer->Add(m_layerSlider, 0, wxALIGN_CENTER_HORIZONTAL);
		rightSizer->Add(bottom, 0, wxALIGN_CENTER_HORIZONTAL);
	topSizer->Add(right);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Construct the bottom half
	//~~~~~~~~~~~~~~~~~~~~~~~~~~

	// TODO

	m_model->m_textureSet.reset(new VolumeTextureSet(m_model->m_volume, m_volumeChoice.windowSettings));

	sizer->Fit(this);

	m_stratumCanvas->setup(m_model);
	m_partitionCanvas->setup(m_model);
}

//#################### PUBLIC METHODS ####################
wxGLContext *PartitionWindow::get_context() const
{
	return m_stratumCanvas->GetContext();
}

//#################### PRIVATE METHODS ####################
void PartitionWindow::calculate_canvas_size()
{
	// We want our canvases to be at least 512x512, but beyond that their size should be dictated by the sizes
	// of the images. We want to be able to show the images in axial (X-Y), sagittal (Y-Z) and coronal (X-Z)
	// orientations, which dictates which dimensions we need to take into account for the canvas sizes.

	Volume::Size volumeSize = m_model->m_volume->size();
	m_canvasWidth = std::max<int>(512, std::max(volumeSize[0], volumeSize[1]));
	m_canvasHeight = std::max<int>(512, std::max(volumeSize[1], volumeSize[2]));
}

}
