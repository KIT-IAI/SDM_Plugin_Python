///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/filepicker.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/collpane.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class venvDlg
///////////////////////////////////////////////////////////////////////////////
class venvDlg : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticText5;
		wxDirPickerCtrl* m_dirPickerDistribution;
		wxStaticLine* m_staticline1;
		wxCollapsiblePane* m_collapsiblePane1;
		wxStaticText* m_staticText2;
		wxTextCtrl* m_textCtrlPluginName;
		wxStaticText* m_staticText3;
		wxTextCtrl* m_textCtrlMenuItem;
		wxStaticText* m_staticText4;
		wxTextCtrl* m_textCtrlMenuPath;
		wxStaticText* m_staticScript;
		wxTextCtrl* m_textCtrlScript;
		wxButton* m_buttonCreatePlugin;
		wxCollapsiblePane* m_collapsiblePane2;
		wxStaticText* m_staticText6;
		wxDirPickerCtrl* m_dirPickerPlugin;
		wxButton* m_buttonCreateVenv;
		wxStaticLine* m_staticline2;
		wxTextCtrl* m_textCtrlLog;

		// Virtual event handlers, override them in your derived class
		virtual void OnDitributionFolderChanged( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void OnPluginNameText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCreateProject( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPluginFolderChanged( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void OnCreateVenv( wxCommandEvent& event ) { event.Skip(); }


	public:

		venvDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Python virtual environment dialog"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 457,434 ), long style = wxDEFAULT_DIALOG_STYLE );

		~venvDlg();

};

