#pragma once

#include <filesystem>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif

#include "venvDlg.h"

///////////////////////////////////////////////////////////////////////////

class MyApp : public wxApp
{
  public:
    virtual bool OnInit();
    virtual int OnExit();
};

///////////////////////////////////////////////////////////////////////////

class venvDlgImpl : public venvDlg
{
  public:
    venvDlgImpl(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(390, 977), long style = wxDEFAULT_DIALOG_STYLE);
    ~venvDlgImpl();

  private:
    virtual void OnDitributionFolderChanged(wxFileDirPickerEvent& event);
    virtual void OnPluginNameText(wxCommandEvent& event);
    virtual void OnCreateProject(wxCommandEvent& event);
    virtual void OnPluginFolderChanged(wxFileDirPickerEvent& event);
    virtual void OnCreateVenv(wxCommandEvent& event);
    virtual void OnInstallRequirements(wxCommandEvent& event);
    virtual void OnCreateRequirements(wxCommandEvent& event);
    virtual void OnInstallPackage(wxCommandEvent& event);
    virtual void OnDeletePlugin(wxCommandEvent& event);
    virtual void OnCreateZip(wxCommandEvent& event);
    virtual void OnZipFileChanged(wxFileDirPickerEvent& event);
    virtual void OnInstallPlugin(wxCommandEvent& event);

    bool execute(const std::string& command, wxTextCtrl* = nullptr);
    bool findDefaultDistribution();
    bool checkDistribution(const std::filesystem::path& distribPath);
    bool createProject(const std::filesystem::path& pluginPath);
    bool createVenv(const std::filesystem::path& pluginPath, const std::filesystem::path& distribPath);
    bool installRequirements(const std::filesystem::path& pluginPath);
    bool createRequirements(const std::filesystem::path& pluginPath);
    bool installPackage(const std::filesystem::path& pluginPath, const std::wstring& package);
    bool validPluginZip(const std::filesystem::path& zipPath, bool& hasRequirements);
    bool installPlugin(const std::filesystem::path& zipPath);
    bool createPluginZip(const std::filesystem::path& sourceDir, const std::filesystem::path& zipPath);
};
