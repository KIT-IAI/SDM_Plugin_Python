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

	m_staticText7 = new wxStaticText( this, wxID_ANY, _("Python version"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer3->Add( m_staticText7, 0, wxALL, 5 );

	m_textPythonVersion = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textPythonVersion->Enable( false );

	fgSizer3->Add( m_textPythonVersion, 1, wxALL|wxEXPAND, 5 );


	bSizer1->Add( fgSizer3, 0, wxEXPAND, 5 );

	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );

	m_collapsiblePane1 = new wxCollapsiblePane( this, wxID_ANY, _("Create plugin"), wxDefaultPosition, wxDefaultSize, wxCP_DEFAULT_STYLE );
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

	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );

	m_collapsiblePane2 = new wxCollapsiblePane( this, wxID_ANY, _("Manage plugin"), wxDefaultPosition, wxDefaultSize, wxCP_DEFAULT_STYLE );
	m_collapsiblePane2->Collapse( true );

	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 2, 0, 0 );
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

	m_buttonInstallRequirements = new wxButton( m_collapsiblePane2->GetPane(), wxID_ANY, _("Install requirements.txt"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonInstallRequirements->Enable( false );

	fgSizer2->Add( m_buttonInstallRequirements, 0, wxALL, 5 );


	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );

	m_buttonCreateRequirements = new wxButton( m_collapsiblePane2->GetPane(), wxID_ANY, _("Create requirements.txt"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonCreateRequirements->Enable( false );

	fgSizer2->Add( m_buttonCreateRequirements, 0, wxALL, 5 );


	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );

	m_buttonInstallPackage = new wxButton( m_collapsiblePane2->GetPane(), wxID_ANY, _("Install package"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonInstallPackage->Enable( false );

	fgSizer2->Add( m_buttonInstallPackage, 0, wxALL, 5 );

	m_textPackage = new wxTextCtrl( m_collapsiblePane2->GetPane(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textPackage->Enable( false );

	fgSizer2->Add( m_textPackage, 1, wxALL|wxEXPAND, 5 );

	m_buttonCreateZip = new wxButton( m_collapsiblePane2->GetPane(), wxID_ANY, _("Create archive"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonCreateZip->Enable( false );

	fgSizer2->Add( m_buttonCreateZip, 0, wxALL, 5 );


	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );

	m_buttonDeletePlugin = new wxButton( m_collapsiblePane2->GetPane(), wxID_ANY, _("Delete Plugin"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonDeletePlugin->Enable( false );

	fgSizer2->Add( m_buttonDeletePlugin, 0, wxALL, 5 );


	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );


	m_collapsiblePane2->GetPane()->SetSizer( fgSizer2 );
	m_collapsiblePane2->GetPane()->Layout();
	fgSizer2->Fit( m_collapsiblePane2->GetPane() );
	bSizer1->Add( m_collapsiblePane2, 0, wxALL|wxEXPAND, 5 );

	m_staticline3 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1->Add( m_staticline3, 0, wxEXPAND | wxALL, 5 );

	m_collapsiblePane3 = new wxCollapsiblePane( this, wxID_ANY, _("Install plugin"), wxDefaultPosition, wxDefaultSize, wxCP_DEFAULT_STYLE );
	m_collapsiblePane3->Collapse( true );

	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText9 = new wxStaticText( m_collapsiblePane3->GetPane(), wxID_ANY, _("Python plugin"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizer4->Add( m_staticText9, 0, wxALL, 5 );

	m_filePickerPythonArchive = new wxFilePickerCtrl( m_collapsiblePane3->GetPane(), wxID_ANY, wxEmptyString, _("Select a file"), _("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	fgSizer4->Add( m_filePickerPythonArchive, 0, wxALL|wxEXPAND, 5 );

	m_checkBoxValidZip = new wxCheckBox( m_collapsiblePane3->GetPane(), wxID_ANY, _("Valid Python plugin"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxValidZip->Enable( false );

	fgSizer4->Add( m_checkBoxValidZip, 0, wxALL, 5 );


	fgSizer4->Add( 0, 0, 1, wxEXPAND, 5 );

	m_checkBoxCreateVenv = new wxCheckBox( m_collapsiblePane3->GetPane(), wxID_ANY, _("Create venv"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxCreateVenv->Enable( false );

	fgSizer4->Add( m_checkBoxCreateVenv, 0, wxALL, 5 );


	fgSizer4->Add( 0, 0, 1, wxEXPAND, 5 );

	m_checkBoxInstallRequirements = new wxCheckBox( m_collapsiblePane3->GetPane(), wxID_ANY, _("Install requirements"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxInstallRequirements->Enable( false );

	fgSizer4->Add( m_checkBoxInstallRequirements, 0, wxALL, 5 );


	fgSizer4->Add( 0, 0, 1, wxEXPAND, 5 );

	m_buttonInstallPlugin = new wxButton( m_collapsiblePane3->GetPane(), wxID_ANY, _("Install plugin"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonInstallPlugin->Enable( false );

	fgSizer4->Add( m_buttonInstallPlugin, 0, wxALL, 5 );


	m_collapsiblePane3->GetPane()->SetSizer( fgSizer4 );
	m_collapsiblePane3->GetPane()->Layout();
	fgSizer4->Fit( m_collapsiblePane3->GetPane() );
	bSizer1->Add( m_collapsiblePane3, 1, wxEXPAND | wxALL, 5 );

	m_staticline4 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1->Add( m_staticline4, 0, wxEXPAND | wxALL, 5 );

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
	m_buttonInstallRequirements->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( venvDlg::OnInstallRequirements ), NULL, this );
	m_buttonCreateRequirements->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( venvDlg::OnCreateRequirements ), NULL, this );
	m_buttonInstallPackage->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( venvDlg::OnInstallPackage ), NULL, this );
	m_buttonCreateZip->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( venvDlg::OnCreateZip ), NULL, this );
	m_buttonDeletePlugin->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( venvDlg::OnDeletePlugin ), NULL, this );
	m_filePickerPythonArchive->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( venvDlg::OnZipFileChanged ), NULL, this );
	m_buttonInstallPlugin->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( venvDlg::OnInstallPlugin ), NULL, this );
}

venvDlg::~venvDlg()
{
}
