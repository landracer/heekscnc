// CToolDlg.cpp
// Copyright (c) 2010, Dan Heeks
// This program is released under the BSD license. See the file COPYING for details.

#include "stdafx.h"
#include "CToolDlg.h"
#include "interface/PictureFrame.h"
#include "interface/NiceTextCtrl.h"
#include "CTool.h"

enum
{
    ID_TITLE_TYPE = 100,
    ID_MATERIAL,
    ID_TOOL_TYPE,
    ID_MAX_ADVANCE_PER_REVOLUTION,
    ID_DIAMETER,
    ID_TOOL_LENGTH_OFFSET,
    ID_FLAT_RADIUS,
    ID_CORNER_RADIUS,
    ID_CUTTING_EDGE_ANGLE,
    ID_CUTTING_EDGE_HEIGHT,
    ID_GRADIENT,
    ID_TITLE,
    ID_VISIBLE,
    ID_XOFFSET,
    ID_FRONTANGLE,
    ID_TOOLANGLE,
    ID_BACKANGLE,
    ID_ORIENTATION,
    ID_PROBEOFFSETX,
    ID_PROBEOFFSETY,
    ID_EXTRUSIONMATERIAL,
    ID_FEEDRATE,
    ID_LAYERHEIGHT,
    ID_WIDTHOVERTHICKNESS,
    ID_TEMPERATURE,
    ID_FLOWRATE,
    ID_FILAMENTDIAMETER,
	
};

BEGIN_EVENT_TABLE(CToolDlg, wxDialog)
    EVT_CHILD_FOCUS(CToolDlg::OnChildFocus)
    EVT_COMBOBOX(ID_TITLE_TYPE,CToolDlg::OnComboTitleType)
    EVT_COMBOBOX(ID_TOOL_TYPE, CToolDlg::OnComboToolType)
    EVT_COMBOBOX(ID_MATERIAL, CToolDlg::OnComboMaterial)
    EVT_COMBOBOX(ID_EXTRUSIONMATERIAL, CToolDlg::OnComboExtrusionMaterial)
END_EVENT_TABLE()

CToolDlg::CToolDlg(wxWindow *parent, CTool* object)
             : wxDialog(parent, wxID_ANY, wxString(_T("Tool Definition")))
{
	m_ignore_event_functions = true;
    wxBoxSizer *sizerMain = new wxBoxSizer(wxHORIZONTAL);

	m_general_bitmap = wxBitmap(wxImage(theApp.GetResFolder() + _T("/bitmaps/ctool/general.png"), wxBITMAP_TYPE_PNG));
	m_diameter_bitmap= wxBitmap(wxImage(theApp.GetResFolder() + _T("/bitmaps/ctool/diameter.png"), wxBITMAP_TYPE_PNG));
	m_tool_length_offset_bitmap = wxBitmap(wxImage(theApp.GetResFolder() + _T("/bitmaps/ctool/toollengthoffset.png"), wxBITMAP_TYPE_PNG));
	m_flat_radius_bitmap = wxBitmap(wxImage(theApp.GetResFolder() + _T("/bitmaps/ctool/flatradius.png"), wxBITMAP_TYPE_PNG));
	m_corner_radius_bitmap = wxBitmap(wxImage(theApp.GetResFolder() + _T("/bitmaps/ctool/cornerradius.png"), wxBITMAP_TYPE_PNG));
	m_cutting_edge_angle_bitmap = wxBitmap(wxImage(theApp.GetResFolder() + _T("/bitmaps/ctool/cuttingedgeangle.png"), wxBITMAP_TYPE_PNG));
	m_cutting_edge_height_bitmap = wxBitmap(wxImage(theApp.GetResFolder() + _T("/bitmaps/ctool/cuttingedgeheight.png"), wxBITMAP_TYPE_PNG));
	m_x_offset_bitmap = wxBitmap(wxImage(theApp.GetResFolder() + _T("/bitmaps/ctool/xoffset.png"), wxBITMAP_TYPE_PNG));
	m_front_angle_bitmap = wxBitmap(wxImage(theApp.GetResFolder() + _T("/bitmaps/ctool/frontangle.png"), wxBITMAP_TYPE_PNG));
	m_tool_angle_bitmap = wxBitmap(wxImage(theApp.GetResFolder() + _T("/bitmaps/ctool/toolangle.png"), wxBITMAP_TYPE_PNG));
	m_back_angle_bitmap = wxBitmap(wxImage(theApp.GetResFolder() + _T("/bitmaps/ctool/backangle.png"), wxBITMAP_TYPE_PNG));
	m_orientation_bitmap = wxBitmap(wxImage(theApp.GetResFolder() + _T("/bitmaps/ctool/orientation.png"), wxBITMAP_TYPE_PNG));
	m_probe_offset_x_bitmap = wxBitmap(wxImage(theApp.GetResFolder() + _T("/bitmaps/ctool/probeoffsetx.png"), wxBITMAP_TYPE_PNG));
	m_probe_offset_y_bitmap = wxBitmap(wxImage(theApp.GetResFolder() + _T("/bitmaps/ctool/probeoffsety.png"), wxBITMAP_TYPE_PNG));
	m_layer_height_bitmap = wxBitmap(wxImage(theApp.GetResFolder() + _T("/bitmaps/ctool/layerheight.png"), wxBITMAP_TYPE_PNG));
	m_width_over_thickness_bitmap = wxBitmap(wxImage(theApp.GetResFolder() + _T("/bitmaps/ctool/wovert.png"), wxBITMAP_TYPE_PNG));
	m_temperature_bitmap = wxBitmap(wxImage(theApp.GetResFolder() + _T("/bitmaps/ctool/temperature.png"), wxBITMAP_TYPE_PNG));
	m_filament_diameter_bitmap = wxBitmap(wxImage(theApp.GetResFolder() + _T("/bitmaps/ctool/filament.png"), wxBITMAP_TYPE_PNG));

	// add left sizer
    wxBoxSizer *sizerLeft = new wxBoxSizer(wxVERTICAL);
    sizerMain->Add( sizerLeft, 0, wxALL, 5 );

	// add right sizer
    wxBoxSizer *sizerRight = new wxBoxSizer(wxVERTICAL);
    sizerMain->Add( sizerRight, 0, wxALL, 5 );

	// add picture to right side
	m_picture = new PictureWindow(this, m_general_bitmap);
	wxBoxSizer *pictureSizer = new wxBoxSizer(wxVERTICAL);
	pictureSizer->Add(m_picture, 1, wxGROW);
    sizerRight->Add( pictureSizer, 0, wxALL, 5 );

	// add OK and Cancel to right side
    wxBoxSizer *sizerOKCancel = new wxBoxSizer(wxVERTICAL);
	sizerRight->Add( sizerOKCancel, 0, wxALL | wxALIGN_RIGHT | wxALIGN_BOTTOM, 5 );
    wxButton* buttonOK = new wxButton(this, wxID_OK, _("OK"));
	sizerOKCancel->Add( buttonOK, 0, wxALL | wxDOWN, 5 );
    wxButton* buttonCancel = new wxButton(this, wxID_CANCEL, _("Cancel"));
	sizerOKCancel->Add( buttonCancel, 0, wxALL | wxUP, 5 );
    buttonOK->SetDefault();

	// add all the controls to the left side
	wxString title_choices[] = {_("Leave manually assigned title"), _("Automatically Generate Title")};
	AddLabelAndControl(sizerLeft, _("Title Type"), m_cmbTitleType = new wxComboBox(this, ID_TITLE_TYPE, _T(""), wxDefaultPosition, wxDefaultSize, 2, title_choices));

	wxString materials[] = {_("High Speed Steel"),_("Carbide") };
	AddLabelAndControl(sizerLeft, _("Tool Material"), m_cmbMaterial = new wxComboBox(this, ID_MATERIAL, _T(""), wxDefaultPosition, wxDefaultSize, 2, materials));

	wxString tool_types[] = {_("Drill Bit"), _("Centre Drill Bit"), _("End Mill"), _("Slot Cutter"), _("Ball End Mill"), _("Chamfer"), _("Turning Tool"), _("Touch Probe"), _("Tool Length Switch"), _("Extrusion")};
	AddLabelAndControl(sizerLeft, _("Tool Type"), m_cmbToolType = new wxComboBox(this, ID_TOOL_TYPE, _T(""), wxDefaultPosition, wxDefaultSize, 10, tool_types));

	AddLabelAndControl(sizerLeft, _("Max advance per revolution"), m_dblMaxAdvancePerRevolution = new CDoubleCtrl(this, ID_MAX_ADVANCE_PER_REVOLUTION));
	AddLabelAndControl(sizerLeft, _("Diameter"), m_dblDiameter = new CDoubleCtrl(this, ID_DIAMETER));
	AddLabelAndControl(sizerLeft, _("Tool length offset"), m_dblToolLengthOffset = new CDoubleCtrl(this, ID_TOOL_LENGTH_OFFSET));
	AddLabelAndControl(sizerLeft, _("Flat radius"), m_dblFlatRadius = new CDoubleCtrl(this, ID_FLAT_RADIUS));
	AddLabelAndControl(sizerLeft, _("Corner radius"), m_dblCornerRadius = new CDoubleCtrl(this, ID_CORNER_RADIUS));
	AddLabelAndControl(sizerLeft, _("Cutting edge angle"), m_dblCuttingEdgeAngle = new CDoubleCtrl(this, ID_CUTTING_EDGE_ANGLE));
	AddLabelAndControl(sizerLeft, _("Cutting edge height"), m_dblCuttingEdgeHeight = new CDoubleCtrl(this, ID_CUTTING_EDGE_HEIGHT));
	AddLabelAndControl(sizerLeft, _("gradient"), m_dblGradient = new CDoubleCtrl(this, ID_GRADIENT));


	// The following are all for lathe tools and should be hidden for all others
	 AddLabelAndControl(sizerLeft, _("X Offset"),m_dblXOffset = new CDoubleCtrl(this, ID_MAX_ADVANCE_PER_REVOLUTION));
	 AddLabelAndControl(sizerLeft, _("Front Angle"),m_dblFrontAngle = new CDoubleCtrl(this, ID_MAX_ADVANCE_PER_REVOLUTION));
	 AddLabelAndControl(sizerLeft, _("Tool Angle"),m_dblToolAngle = new CDoubleCtrl(this, ID_MAX_ADVANCE_PER_REVOLUTION));
	 AddLabelAndControl(sizerLeft, _("Back Angle"),m_dblBackAngle = new CDoubleCtrl(this, ID_MAX_ADVANCE_PER_REVOLUTION));
	 AddLabelAndControl(sizerLeft, _("Orientation"),m_lgthorientation = new CLengthCtrl(this, ID_MAX_ADVANCE_PER_REVOLUTION));

	// The following are for probe tools and should be hidden for all others
	
	 AddLabelAndControl(sizerLeft, _("Probe Offset X"),m_dblProbeOffsetX = new CDoubleCtrl(this, ID_MAX_ADVANCE_PER_REVOLUTION));
	 AddLabelAndControl(sizerLeft, _("Probe Offset Y"),m_dblProbeOffsetY = new CDoubleCtrl(this, ID_MAX_ADVANCE_PER_REVOLUTION));
	
	
	// The following are for extrusion and should be hidden for all others
	 wxString extrusionmaterials[] = {_("ABS Plastic"),_("PLA Plastic"),_("HDPE Plastic"),_("Other") };
	 AddLabelAndControl(sizerLeft, _("Extrusion Material"),m_cmbExtrusionMaterial = new wxComboBox(this, ID_EXTRUSIONMATERIAL, _T(""), wxDefaultPosition, wxDefaultSize, 4, extrusionmaterials));
	 AddLabelAndControl(sizerLeft, _("Feed Rate"),m_dblFeedRate = new CDoubleCtrl(this, ID_MAX_ADVANCE_PER_REVOLUTION));
	 AddLabelAndControl(sizerLeft, _("Layer Height"),m_dblLayerHeight = new CDoubleCtrl(this, ID_MAX_ADVANCE_PER_REVOLUTION));
	 AddLabelAndControl(sizerLeft, _("Width Over Thickness ratio"),m_dblWidthOverThickness = new CDoubleCtrl(this, ID_MAX_ADVANCE_PER_REVOLUTION));
	 AddLabelAndControl(sizerLeft, _("Temperature"),m_dblTemperature = new CDoubleCtrl(this, ID_MAX_ADVANCE_PER_REVOLUTION));
	 AddLabelAndControl(sizerLeft, _("Flow Rate"),m_dblFlowrate = new CDoubleCtrl(this, ID_MAX_ADVANCE_PER_REVOLUTION));
	 AddLabelAndControl(sizerLeft, _("Filament Diameter"),m_dblFilamentDiameter = new CDoubleCtrl(this, ID_MAX_ADVANCE_PER_REVOLUTION));
	 
	AddLabelAndControl(sizerLeft, _("Title"), m_txtTitle = new wxTextCtrl(this, ID_TITLE));
	sizerLeft->Add( m_chkVisible = new wxCheckBox( this, ID_VISIBLE, _("Visible") ), 0, wxALL, 5 );	
	
//	AddLabelAndControl(sizerLeft, _("sketches"), m_idsSketches = new CObjectIdsCtrl(this, ID_SKETCHES));
//	wxString starting_place_choices[] = {_("boundary"), _("center")};
//	AddLabelAndControl(sizerLeft, _("starting place"), m_cmbStartingPlace = new wxComboBox(this, ID_STARTING_PLACE, _T(""), wxDefaultPosition, wxDefaultSize, 2, starting_place_choices));
//	sizerLeft->Add( m_chkUseZigZag = new wxCheckBox( this, ID_USE_ZIG_ZAG, _("use zig zag") ), 0, wxALL, 5 );
//	AddLabelAndControl(sizerLeft, _("zig zag angle"), m_dblZigAngle = new CDoubleCtrl(this, ID_ZIG_ANGLE));
//	AddLabelAndControl(sizerLeft, _("clearance height"), m_lgthClearanceHeight = new CLengthCtrl(this, ID_CLEARANCE_HEIGHT));
//	AddLabelAndControl(sizerLeft, _("comment"), m_txtComment = new wxTextCtrl(this, ID_COMMENT));
//	sizerLeft->Add( m_chkActive = new wxCheckBox( this, ID_ACTIVE, _("active") ), 0, wxALL, 5 );

	SetFromData(object);

    SetSizer( sizerMain );
    sizerMain->SetSizeHints(this);
	sizerMain->Fit(this);

//    m_idsSketches->SetFocus();

	m_ignore_event_functions = false;

	SetPicture();
}

void CToolDlg::GetData(CTool* object)
{
	if(m_ignore_event_functions)return;
	m_ignore_event_functions = true;

	//need to deal with the combo box for title type
	//need to deal with the combo box for material
	//need to deal with the combo box for Tool Type.
	
	object->m_params.m_max_advance_per_revolution = m_dblMaxAdvancePerRevolution->GetValue();
	object->m_params.m_diameter = m_dblDiameter->GetValue();
	object->m_params.m_tool_length_offset = m_dblToolLengthOffset->GetValue();	
	object->m_params.m_flat_radius = m_dblFlatRadius->GetValue();
	object->m_params.m_corner_radius = m_dblCornerRadius->GetValue();
	object->m_params.m_cutting_edge_angle = m_dblCuttingEdgeAngle->GetValue();
	object->m_params.m_cutting_edge_height = m_dblCuttingEdgeHeight->GetValue();

	// The following are all for lathe tools.  They become relevant when the m_type = eTurningTool

	// The following are for extrusion


	 object->m_params.m_x_offset = m_dblXOffset->GetValue();
	 object->m_params.m_front_angle = m_dblFrontAngle->GetValue();
	 object->m_params.m_tool_angle = m_dblToolAngle->GetValue();
	 object->m_params.m_back_angle = m_dblBackAngle->GetValue();
	 object->m_params.m_orientation = m_lgthorientation->GetValue();

	// The following are for probe tools
	
	 object->m_params.m_probe_offset_x = m_dblProbeOffsetX->GetValue();
	 object->m_params.m_probe_offset_y = m_dblProbeOffsetY->GetValue();
	
	
	// The following are for extrusion
	
	//need to deal with the combo box for title type
	 //object->m_params.m_extrusion_material = m_cmbExtrusionMaterial->GetValue();
	 object->m_params.m_feedrate = m_dblFeedRate->GetValue();
	 object->m_params.m_layer_height = m_dblLayerHeight->GetValue();
	 object->m_params.m_width_over_thickness = m_dblWidthOverThickness->GetValue();
	 object->m_params.m_temperature = m_dblTemperature->GetValue();
	 object->m_params.m_flowrate = m_dblFlowrate->GetValue();
	 object->m_params.m_filament_diameter = m_dblFilamentDiameter->GetValue();
	
	//need to deal with the check box for visible
	
	object->m_title = m_txtTitle->GetValue();

	m_ignore_event_functions = false;
}

void CToolDlg::SetFromData(CTool* object)
{
	m_ignore_event_functions = true;

	//need to deal with the combo box for title type
	//need to deal with the combo box for material
	//need to deal with the combo box for Tool Type.
	
	m_dblMaxAdvancePerRevolution->SetValue(object->m_params.m_max_advance_per_revolution);
	m_dblDiameter->SetValue(object->m_params.m_diameter);
	m_dblToolLengthOffset->SetValue(object->m_params.m_tool_length_offset);
	m_dblFlatRadius->SetValue(object->m_params.m_flat_radius);
	m_dblCornerRadius->SetValue(object->m_params.m_corner_radius);
	m_dblCuttingEdgeAngle->SetValue(object->m_params.m_cutting_edge_angle);
	m_dblCuttingEdgeHeight->SetValue(object->m_params.m_cutting_edge_height);

	 m_dblXOffset ->SetValue(object->m_params.m_x_offset);
	 m_dblFrontAngle->SetValue(object->m_params.m_front_angle);
	 m_dblToolAngle->SetValue(object->m_params.m_tool_angle);
	 m_dblBackAngle->SetValue(object->m_params.m_back_angle);
	 m_lgthorientation->SetValue(object->m_params.m_orientation);

	// The following are for probe tools
	
	 m_dblProbeOffsetX->SetValue(object->m_params.m_probe_offset_x);
	 m_dblProbeOffsetY->SetValue(object->m_params.m_probe_offset_y);
	
	
	// The following are for extrusion
	 //m_cmbExtrusionMaterial->SetValue(object->m_params.m_max_advance_per_revolution);
	 m_dblFeedRate->SetValue(object->m_params.m_feedrate);
	 m_dblLayerHeight->SetValue(object->m_params.m_layer_height);
	 m_dblWidthOverThickness->SetValue(object->m_params.m_width_over_thickness);
	 m_dblTemperature->SetValue(object->m_params.m_temperature);
	 m_dblFlowrate->SetValue(object->m_params.m_flowrate);
	 m_dblFilamentDiameter->SetValue(object->m_params.m_filament_diameter);
	

	//need to deal with the text box for title
	//need to deal with the check box for visible

	m_ignore_event_functions = false;
}

void CToolDlg::SetPicture()
{
	wxWindow* w = FindFocus();

	if(w == m_dblDiameter)m_picture->SetPicture(m_diameter_bitmap);
	else if(w == m_dblToolLengthOffset)m_picture->SetPicture(m_tool_length_offset_bitmap);
	else if(w == m_dblFlatRadius)m_picture->SetPicture(m_flat_radius_bitmap);
	else if(w == m_dblCornerRadius)m_picture->SetPicture(m_corner_radius_bitmap);
	else if(w == m_dblCuttingEdgeAngle)m_picture->SetPicture(m_cutting_edge_angle_bitmap);
	else if(w == m_dblCuttingEdgeHeight)m_picture->SetPicture(m_cutting_edge_height_bitmap);
	

	// The following are all for lathe tools.  They become relevant when the m_type = eTurningTool
	 else if(w == m_dblXOffset)m_picture->SetPicture(m_x_offset_bitmap);
	 else if(w == m_dblFrontAngle)m_picture->SetPicture(m_front_angle_bitmap);
	 else if(w == m_dblToolAngle)m_picture->SetPicture(m_tool_angle_bitmap);
	 else if(w == m_dblBackAngle)m_picture->SetPicture(m_back_angle_bitmap);
	 else if(w == m_lgthorientation)m_picture->SetPicture(m_orientation_bitmap);

	// The following are for probe tools
	
	 else if(w == m_dblProbeOffsetX)m_picture->SetPicture(m_probe_offset_x_bitmap);
	 else if(w == m_dblProbeOffsetY)m_picture->SetPicture(m_probe_offset_y_bitmap);
	
	
	// The following are for extrusion
	 else if(w == m_dblLayerHeight)m_picture->SetPicture(m_layer_height_bitmap);
	 else if(w == m_dblWidthOverThickness)m_picture->SetPicture(m_width_over_thickness_bitmap);
	 else if(w == m_dblTemperature)m_picture->SetPicture(m_temperature_bitmap);
	 else if(w == m_dblFilamentDiameter)m_picture->SetPicture(m_filament_diameter_bitmap);
	 
	else m_picture->SetPicture(m_general_bitmap);	
	
}

void CToolDlg::OnChildFocus(wxChildFocusEvent& event)
{
	if(m_ignore_event_functions)return;
	if(event.GetWindow())
	{
		SetPicture();
	}
}

void CToolDlg::OnComboTitleType( wxCommandEvent& event )
{
	if(m_ignore_event_functions)return;
//	SetPicture();
}

void CToolDlg::OnComboMaterial( wxCommandEvent& event )
{
	if(m_ignore_event_functions)return;
//	//SetPicture();
}

void CToolDlg::OnComboToolType(wxCommandEvent& event)
{
	if(m_ignore_event_functions)return;
//	SetPicture();
}

void CToolDlg::OnComboExtrusionMaterial(wxCommandEvent& event)
{
	if(m_ignore_event_functions)return;
//	SetPicture();
}

void CToolDlg::AddLabelAndControl(wxBoxSizer* sizer, const wxString& label, wxWindow* control)
{
    wxBoxSizer *sizer_horizontal = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *static_label = new wxStaticText(this, wxID_ANY, label);
	sizer_horizontal->Add( static_label, 0, wxRIGHT | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5 );
	sizer_horizontal->Add( control, 0, wxLEFT | wxALIGN_RIGHT | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5 );
	sizer->Add( sizer_horizontal, 0, wxALL, 5 );
}