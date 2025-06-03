///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "venvDlg.h"

///////////////////////////////////////////////////////////////////////////

venvDlg::venvDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer3->AddGrowableCol( 1 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText5 = new wxStaticText( this, wxID_ANY, _("Python distribution"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer3->Add( m_staticText5, 0, wxALL, 5 );

	m_dirPickerDistribution = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, _("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	fgSizer3->Add( m_dirPickerDistribution, 0, wxALL|wxEXPAND, 5 );


	bSizer1->Add( fgSizer3, 0, wxEXPAND, 5 );

	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );

	m_collapsiblePane1 = new wxCollapsiblePane( this, wxID_ANY, _("Create Python plugin"), wxDefaultPosition, wxDefaultSize, wxCP_DEFAULT_STYLE );
	m_collapsiblePane1->Collapse( true );

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 5, 2, 0, 0 );
	fgSizer1->AddGrowableCol( 1 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText2 = new wxStaticText( m_collapsiblePane1->GetPane(), wxID_ANY, _("Plugin name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer1->Add( m_staticText2, 0, wxALL, 5 );

	m_textCtrlPluginName = new wxTextCtrl( m_collapsiblePane1->GetPane(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_textCtrlPluginName, 0, wxALL|wxEXPAND, 5 );

	m_staticText3 = new wxStaticText( m_collapsiblePane1->GetPane(), wxID_ANY, _("Menu item"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer1->Add( m_staticText3, 0, wxALL, 5 );

	m_textCtrlMenuItem = new wxTextCtrl( m_collapsiblePane1->GetPane(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_textCtrlMenuItem, 0, wxALL|wxEXPAND, 5 );

	m_staticText4 = new wxStaticText( m_collapsiblePane1->GetPane(), wxID_ANY, _("Menu path"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	fgSizer1->Add( m_staticText4, 0, wxALL, 5 );

	m_textCtrlMenuPath = new wxTextCtrl( m_collapsiblePane1->GetPane(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_textCtrlMenuPath, 0, wxALL|wxEXPAND, 5 );

	m_staticScript = new wxStaticText( m_collapsiblePane1->GetPane(), wxID_ANY, _("Python script"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticScript->Wrap( -1 );
	fgSizer1->Add( m_staticScript, 0, wxALL, 5 );

	m_textCtrlScript = new wxTextCtrl( m_collapsiblePane1->GetPane(), wxID_ANY, _("main.py"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_textCtrlScript, 0, wxALL|wxEXPAND, 5 );

	m_buttonCreatePlugin = new wxButton( m_collapsiblePane1->GetPane(), wxID_ANY, _("Create plugin"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonCreatePlugin->Enable( false );

	fgSizer1->Add( m_buttonCreatePlugin, 0, wxALL, 5 );


	fgSizer1->Add( 0, 0, 1, wxEXPAND, 5 );


	m_collapsiblePane1->GetPane()->SetSizer( fgSizer1 );
	m_collapsiblePane1->GetPane()->Layout();
	fgSizer1->Fit( m_collapsiblePane1->GetPane() );
	bSizer1->Add( m_collapsiblePane1, 0, wxALL|wxEXPAND, 5 );

	m_collapsiblePane2 = new wxCollapsiblePane( this, wxID_ANY, _("Create virtual environment"), wxDefaultPosition, wxDefaultSize, wxCP_DEFAULT_STYLE );
	m_collapsiblePane2->Collapse( true );

	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer2->AddGrowableCol( 1 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText6 = new wxStaticText( m_collapsiblePane2->GetPane(), wxID_ANY, _("Plugin folder"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer2->Add( m_staticText6, 0, wxALL, 5 );

	m_dirPickerPlugin = new wxDirPickerCtrl( m_collapsiblePane2->GetPane(), wxID_ANY, wxEmptyString, _("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	fgSizer2->Add( m_dirPickerPlugin, 0, wxALL|wxEXPAND, 5 );

	m_buttonCreateVenv = new wxButton( m_collapsiblePane2->GetPane(), wxID_ANY, _("Create venv"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonCreateVenv->Enable( false );

	fgSizer2->Add( m_buttonCreateVenv, 0, wxALL, 5 );


	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );


	m_collapsiblePane2->GetPane()->SetSizer( fgSizer2 );
	m_collapsiblePane2->GetPane()->Layout();
	fgSizer2->Fit( m_collapsiblePane2->GetPane() );
	bSizer1->Add( m_collapsiblePane2, 0, wxALL|wxEXPAND, 5 );

	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );

	m_textCtrlLog = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	bSizer1->Add( m_textCtrlLog, 1, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_dirPickerDistribution->Connect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( venvDlg::OnDitributionFolderChanged ), NULL, this );
	m_textCtrlPluginName->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( venvDlg::OnPluginNameText ), NULL, this );
	m_buttonCreatePlugin->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( venvDlg::OnCreateProject ), NULL, this );
	m_dirPickerPlugin->Connect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( venvDlg::OnPluginFolderChanged ), NULL, this );
	m_buttonCreateVenv->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( venvDlg::OnCreateVenv ), NULL, this );
}

venvDlg::~venvDlg()
{
}
