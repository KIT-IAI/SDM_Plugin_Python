#include "venvDlgImpl.h"
#include <wx/valnum.h>
#include <fstream>
#include <sstream>
#include <stdutils/executable.hpp>

IMPLEMENT_APP_NO_MAIN(MyApp)

bool MyApp::OnInit()
{
  return true;
}

int MyApp::OnExit()
{
  return 0;
}

///////////////////////////////////////////////////////////////////////////

venvDlgImpl::venvDlgImpl(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
  : venvDlg(parent, id, title, pos, size, style)
{
  SetSize(wxSize(379, 1026));
  Centre();

  SetDoubleBuffered(true);

  findDefaultDistribution();

  wxSize minSize = this->GetBestSize();
  this->SetSize(minSize);
}

venvDlgImpl::~venvDlgImpl()
{
}

void venvDlgImpl::OnDitributionFolderChanged(wxFileDirPickerEvent& event)
{
  std::filesystem::path distribPath(m_dirPickerDistribution->GetPath().ToStdString());

  if (checkDistribution(distribPath))
  {
    m_textCtrlLog->SetValue(L"Python distribution available");
  }
  else
  {
    m_textCtrlLog->SetValue(L"Missing Python distribution");
  }

  TransferDataToWindow();
  Update();
}

void venvDlgImpl::OnPluginNameText(wxCommandEvent& event)
{
  if (!m_textCtrlPluginName->IsModified())
    return;
  
  wxString pluginName = m_textCtrlPluginName->GetValue();

  wxString prevName(pluginName);
  prevName.RemoveLast(1);

  if (m_textCtrlMenuItem->GetValue() == prevName)
  {
    m_textCtrlMenuItem->ChangeValue(pluginName);

    TransferDataToWindow();
    Update();
  }

  if (m_textCtrlMenuPath->GetValue() == prevName)
  {
    m_textCtrlMenuPath->ChangeValue(pluginName);

    TransferDataToWindow();
    Update();
  }

  if (pluginName.length() > 6)
  {
    m_buttonCreatePlugin->Enable(true);
  }
  else
  {
    m_buttonCreatePlugin->Enable(false);
  }
}

void venvDlgImpl::OnCreateProject(wxCommandEvent& event)
{
  auto executableDir = stdutils::findExecutableFileName().parent_path();

  std::string pluginName(m_textCtrlPluginName->GetValue().ToStdString());
  std::filesystem::path pluginPath(executableDir/"plugins"/pluginName);

  if (createProject(pluginPath))
  {
    m_textCtrlLog->SetValue(L"New plugin created: " + pluginName);

    TransferDataToWindow();
    Update();

    std::filesystem::path distribPath(m_dirPickerDistribution->GetPath().ToStdString());

    m_textCtrlLog->AppendText(L"\nPlease wait, creating venv ...");

    TransferDataToWindow();
    Update();

    if (createVenv(pluginPath, distribPath))
    {
      m_textCtrlLog->AppendText(L"\nVirtual environment created: " + pluginName);
    }

    TransferDataToWindow();
    Update();
  }
}

void venvDlgImpl::OnPluginFolderChanged(wxFileDirPickerEvent& event)
{
  if (!m_dirPickerPlugin->GetPath().empty())
  {
    m_buttonCreateVenv->Enable(true);
  }
  else
  {
    m_buttonCreateVenv->Enable(false);
  }
}

void venvDlgImpl::OnCreateVenv(wxCommandEvent& event)
{
  std::filesystem::path distribPath(m_dirPickerDistribution->GetPath().ToStdString());
  std::filesystem::path pluginPath(m_dirPickerPlugin->GetPath().ToStdString());
  std::string pluginName = pluginPath.filename().string();

  if (checkDistribution(distribPath))
  {
    m_textCtrlLog->SetValue(L"Please wait, creating venv ...");

    TransferDataToWindow();
    Update();

    createVenv(pluginPath, distribPath);

    m_textCtrlLog->AppendText(L"\nVirtual environment created: " + pluginName);

    TransferDataToWindow();
    Update();
  }
}

bool venvDlgImpl::findDefaultDistribution()
{
  bool found(false);

  auto executableDir = stdutils::findExecutableFileName().parent_path();

  std::filesystem::path distribPath(executableDir/"plugins"/"ifcexplorer_python3"/"distrib");

  if (std::filesystem::exists(distribPath))
  {
    if (checkDistribution(distribPath))
    {
      m_textCtrlLog->SetValue(L"Default Python distribution available");
    }

    m_dirPickerDistribution->SetPath(distribPath.string());

    found = true;
  }

  if (!found)
  {
    m_textCtrlLog->SetValue(L"Default Python distribution not found!");
  }

  TransferDataToWindow();
  Update();

  return found;
}

bool venvDlgImpl::checkDistribution(const std::filesystem::path& distribPath)
{
  if (std::filesystem::exists(distribPath/"python.exe"))
  {
    return true;
  }

  return false;
}

bool venvDlgImpl::createProject(const std::filesystem::path& pluginPath)
{
  if (!std::filesystem::exists(pluginPath))
  {
    std::filesystem::create_directory(pluginPath);

    // create plugin_ignore file
    std::ofstream plugin_ignore(pluginPath/"plugin_ignore");
    plugin_ignore.close();

    // create python_plugin.manifest file
    std::ofstream plugin_manifest(pluginPath/"python_plugin.manifest");
    plugin_manifest << "ACTION=" << m_textCtrlMenuItem->GetValue().ToStdString() << std::endl;
    plugin_manifest << "POPUP=" << m_textCtrlMenuPath->GetValue().ToStdString() << std::endl;
    plugin_manifest << "SCRIPT=" << m_textCtrlScript->GetValue().ToStdString() << std::endl;
    plugin_manifest.close();

    // create script file
    std::ofstream script(pluginPath/m_textCtrlScript->GetValue().ToStdString());
    script << "import ifcdb" << std::endl;
    script << std::endl;
    script << "document = ifcdb.get_document()" << std::endl;
    script << "document.log_message('Plugin " << pluginPath.filename().string() << "')" << std::endl;
    script << std::endl;
    script.close();
  }

  return true;
}

bool venvDlgImpl::createVenv(const std::filesystem::path& pluginPath, const std::filesystem::path& distribPath)
{
  std::stringstream command;
  command << distribPath.string() << "\\python.exe -m venv " << pluginPath.string() << "\\venv";

  int state = std::system(command.str().c_str());

  if (state == 0)
  {
    std::stringstream activate;
    activate << pluginPath.string() << "\\venv\\Scripts\\activate.bat";

    state = std::system(activate.str().c_str());
  }

  //std::ofstream create_venv(pluginPath/"create_venv.cmd");
  //create_venv << distribPath.string() << "\\python.exe -m venv " << pluginPath.string() << std::endl;
  //create_venv << pluginPath.string() << "\\venv\\Scripts\\activate.bat" << std::endl;
  //create_venv.close();

  //std::filesystem::path createVenvPath(pluginPath/"create_venv.cmd");

  //int state = std::system(createVenvPath.string().c_str());

  return true;
}

bool venvDlgImpl::installPlugin(const std::filesystem::path& archivePath, const std::filesystem::path& distribPath)
{





  return true;
}
