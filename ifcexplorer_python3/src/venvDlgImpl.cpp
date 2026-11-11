#include "venvDlgImpl.h"
#include <wx/valnum.h>
#include <fstream>
#include <sstream>
#include <stdutils/executable.hpp>
#include <kubazip/zip/zip.h>

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
    m_textCtrlLog->SetValue(L"Python distribution available\n");
  }
  else
  {
    m_textCtrlLog->SetValue(L"Missing Python distribution\n");
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
  std::filesystem::path pluginPath(executableDir / "plugins" / pluginName);

  if (createProject(pluginPath))
  {
    m_textCtrlLog->SetValue(L"New plugin created: " + pluginName + L"\n");

    TransferDataToWindow();
    Update();

    std::filesystem::path distribPath(m_dirPickerDistribution->GetPath().ToStdString());

    m_textCtrlLog->AppendText(L"Please wait, creating venv ...\n");

    TransferDataToWindow();
    Update();

    if (createVenv(pluginPath, distribPath))
    {
      m_textCtrlLog->AppendText(L"Virtual environment created: " + pluginName + L"\n");
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

    const auto pluginPath = std::filesystem::path(m_dirPickerPlugin->GetPath().ToStdString());

    if (std::filesystem::exists(pluginPath / "venv" / "pyvenv.cfg"))
    {
      if (std::filesystem::exists(pluginPath / "requirments.txt"))
      {
        m_buttonInstallRequirements->Enable(true);
      }

      m_buttonCreateRequirements->Enable(true);
      m_buttonInstallPackage->Enable(true);
      m_buttonCreateZip->Enable(true);
      m_buttonDeletePlugin->Enable(true);
    }
  }
  else
  {
    m_buttonCreateVenv->Enable(false);
    m_buttonInstallRequirements->Enable(false);
    m_buttonCreateRequirements->Enable(false);
    m_buttonInstallPackage->Enable(false);
    m_buttonCreateZip->Enable(false);
    m_buttonDeletePlugin->Enable(false);
  }
}

void venvDlgImpl::OnCreateVenv(wxCommandEvent& event)
{
  std::filesystem::path distribPath(m_dirPickerDistribution->GetPath().ToStdString());
  std::filesystem::path pluginPath(m_dirPickerPlugin->GetPath().ToStdString());
  std::string pluginName = pluginPath.filename().string();

  m_buttonInstallRequirements->Enable(false);
  m_buttonCreateRequirements->Enable(false);
  m_buttonInstallPackage->Enable(false);
  m_buttonCreateZip->Enable(false);

  if (checkDistribution(distribPath))
  {
    m_textCtrlLog->SetValue(L"Please wait, creating venv ...\n");

    TransferDataToWindow();
    Update();

    if (createVenv(pluginPath, distribPath))
    {
      m_textCtrlLog->AppendText(L"Virtual environment created: " + pluginName + L"\n");

      if (std::filesystem::exists(pluginPath / "requirments.txt"))
      {
        m_buttonInstallRequirements->Enable(true);
        m_buttonCreateRequirements->Enable(true);
      }

      m_buttonInstallPackage->Enable(true);
      m_textPackage->Enable(true);
      m_buttonCreateZip->Enable(true);
    }
    else
    {
      m_textCtrlLog->AppendText(L"Creation failed\n");
    }
  }
  else
  {
    m_textCtrlLog->AppendText(L"Distribution check failed\n");
  }

  TransferDataToWindow();
  Update();
}

void venvDlgImpl::OnInstallRequirements(wxCommandEvent& event)
{
  std::filesystem::path pluginPath(m_dirPickerPlugin->GetPath().ToStdString());
  std::string pluginName = pluginPath.filename().string();

  m_textCtrlLog->SetValue(L"Please wait, installing requirements.txt ...\n");

  TransferDataToWindow();
  Update();

  if (installRequirements(pluginPath))
  {
    m_textCtrlLog->AppendText(L"requirements.txt installed\n");
  }
  else
  {
    m_textCtrlLog->AppendText(L"Installation failed\n");
  }

  TransferDataToWindow();
  Update();
}

void venvDlgImpl::OnCreateRequirements(wxCommandEvent& event)
{
  std::filesystem::path pluginPath(m_dirPickerPlugin->GetPath().ToStdString());
  std::string pluginName = pluginPath.filename().string();

  m_textCtrlLog->SetValue(L"Please wait, creating requirements.txt ...\n");

  TransferDataToWindow();
  Update();

  if (createRequirements(pluginPath))
  {
    m_textCtrlLog->AppendText(L"requirements.txt created\n");
  }
  else
  {
    m_textCtrlLog->AppendText(L"Creation failed\n");
  }

  TransferDataToWindow();
  Update();
}

void venvDlgImpl::OnInstallPackage(wxCommandEvent& event)
{
  if (m_textPackage->GetValue().empty())
  {
    wxMessageBox("Please enter a Python package", "Message", wxOK | wxICON_INFORMATION, this);

    return;
  }

  std::filesystem::path pluginPath(m_dirPickerPlugin->GetPath().ToStdString());
  std::string pluginName = pluginPath.filename().string();

  m_textCtrlLog->SetValue(L"Please wait, installing package " + m_textPackage->GetValue() + L" ...\n");

  TransferDataToWindow();
  Update();

  if (installPackage(pluginPath, m_textPackage->GetValue().ToStdWstring()))
  {
    m_textCtrlLog->AppendText(L"Package installed: " + m_textPackage->GetValue() + L"\n");
  }
  else
  {
    m_textCtrlLog->AppendText(L"Installation failed\n");
  }

  TransferDataToWindow();
  Update();
}


void venvDlgImpl::OnDeletePlugin(wxCommandEvent& event)
{
  int state = wxMessageBox("Delete plugin?", "Message", wxYES_NO | wxICON_INFORMATION, this);

  if (state == wxYES)
  {
    std::filesystem::path pluginPath(m_dirPickerPlugin->GetPath().ToStdString());
    std::string pluginName = pluginPath.filename().string();

    m_textCtrlLog->SetValue(L"Please wait, deleting plugin ...\n");
    TransferDataToWindow();
    Update();

    if (std::filesystem::exists(pluginPath))
    {
      std::filesystem::remove_all(pluginPath);
      m_textCtrlLog->AppendText(L"Plugin deleted: " + pluginName + L"\n");
    }
    else
    {
      m_textCtrlLog->AppendText(L"Plugin not found\n");
    }

    TransferDataToWindow();
    Update();
  }
}

void venvDlgImpl::OnCreateZip(wxCommandEvent& event)
{
  std::filesystem::path pluginPath(m_dirPickerPlugin->GetPath().ToStdString());
  std::filesystem::path zipPath = pluginPath / (pluginPath.filename().string() + ".zip");

  m_textCtrlLog->SetValue(L"Please wait, creating zip file ...\n");

  TransferDataToWindow();
  Update();

  if (createPluginZip(pluginPath, zipPath))
  {
    m_textCtrlLog->AppendText(L"Zip file created: " + zipPath.wstring() + L"\n");
  }
  else
  {
    m_textCtrlLog->AppendText(L"Creation failed\n");
  }

  TransferDataToWindow();
  Update();
}

void venvDlgImpl::OnZipFileChanged(wxFileDirPickerEvent& event)
{
  std::filesystem::path zipPath(m_filePickerPythonArchive->GetPath().ToStdString());

  bool hasRequirements(false);

  if (validPluginZip(zipPath, hasRequirements))
  {
    m_checkBoxValidZip->SetValue(true);
    m_buttonInstallPlugin->Enable(true);
    m_checkBoxCreateVenv->Enable(true);
    m_checkBoxCreateVenv->SetValue(true);

    if (hasRequirements)
    {
      m_checkBoxInstallRequirements->SetValue(true);
      m_checkBoxInstallRequirements->Enable(true);
    }
  }
  else
  {
    m_checkBoxValidZip->SetValue(false);
    m_checkBoxInstallRequirements->SetValue(false);
    m_buttonInstallPlugin->Enable(false);
    m_checkBoxCreateVenv->Enable(false);
    m_checkBoxCreateVenv->SetValue(false);
  }
}

void venvDlgImpl::OnInstallPlugin(wxCommandEvent& event)
{
  std::filesystem::path zipPath(m_filePickerPythonArchive->GetPath().ToStdString());

  m_textCtrlLog->SetValue(L"Installing plugin ...\n");

  if (installPlugin(zipPath))
  {
    m_textCtrlLog->AppendText(L"Plugin successfully installed: " + zipPath.stem().string() + L"\n");

    if (m_checkBoxCreateVenv->GetValue())
    {
      std::filesystem::path distribPath(m_dirPickerDistribution->GetPath().ToStdString());

      if (checkDistribution(distribPath))
      {
        m_textCtrlLog->AppendText(L"Please wait, creating venv ...\n");

        TransferDataToWindow();
        Update();

        auto executableDir = stdutils::findExecutableFileName().parent_path();

        const std::string pluginName = zipPath.stem().string();
        std::filesystem::path pluginPath(executableDir / "plugins" / pluginName);

        if (createVenv(pluginPath, distribPath))
        {
          m_textCtrlLog->AppendText(L"Virtual environment created: " + pluginName + L"\n");

          TransferDataToWindow();
          Update();

          if (m_checkBoxInstallRequirements->GetValue())
          {
            m_textCtrlLog->AppendText(L"Installing requirements.txt ...\n");

            if (installRequirements(pluginPath))
            {
              m_textCtrlLog->AppendText(L"requirements.txt installed\n");
            }
            else
            {
              m_textCtrlLog->AppendText(L"Installation failed\n");
            }
          }
        }
        else
        {
          m_textCtrlLog->AppendText(L"Creation failed\n");
        }
      }
    }
  }
  else
  {
    m_textCtrlLog->AppendText(L"Plugin installation failed\n");
  }

  TransferDataToWindow();
  Update();
}

bool venvDlgImpl::execute(const std::string& command, wxTextCtrl* pTextCtrl)
{
  if (!pTextCtrl)
  {
    return (std::system(command.c_str()) == 0);
  }
  else
  {
    //pTextCtrl->AppendText(wxString::FromUTF8(command + "\n"));

    FILE* pipe = _popen((command + " 2>&1").c_str(), "r");

    if (!pipe)
    {
      return false;
    }

    char buffer[128];

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
    {
      pTextCtrl->AppendText(wxString::FromUTF8(buffer) + "\n");

      TransferDataToWindow();
      Update();
    }

    _pclose(pipe);

    return true;
  }
}

bool venvDlgImpl::findDefaultDistribution()
{
  bool found(false);

  auto executableDir = stdutils::findExecutableFileName().parent_path();

  std::filesystem::path distribPath(executableDir / "plugins" / "ifcexplorer_python3" / "distrib");

  if (std::filesystem::exists(distribPath))
  {
    if (checkDistribution(distribPath))
    {
      m_textCtrlLog->AppendText(L"Default Python distribution available\n");

      const auto pythonExe = distribPath / "python.exe";

      execute(pythonExe.string() + " -c \"import sys; print(sys.version)\"", m_textPythonVersion);
    }

    m_dirPickerDistribution->SetPath(distribPath.string());

    found = true;
  }

  if (!found)
  {
    m_textCtrlLog->AppendText(L"Default Python distribution not found!\n");
  }

  TransferDataToWindow();
  Update();

  return found;
}

bool venvDlgImpl::checkDistribution(const std::filesystem::path& distribPath)
{
  if (std::filesystem::exists(distribPath / "python.exe"))
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
    std::ofstream plugin_ignore(pluginPath / "plugin_ignore");
    plugin_ignore.close();

    // create python_plugin.manifest file
    std::ofstream plugin_manifest(pluginPath / "python_plugin.manifest");
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
  const auto pythonExe = distribPath / "python.exe";
  const auto venvPath = pluginPath / "venv";

  std::stringstream command;
  command << pythonExe.string() << " -m venv " << venvPath.string();

  if (execute(command.str().c_str()))
  {
    const auto activate = venvPath / "Scripts" / "activate.bat";

    return execute(activate.string().c_str());
  }

  return false;
}

bool venvDlgImpl::installRequirements(const std::filesystem::path& pluginPath)
{
  const auto pythonExe = pluginPath / "venv" / "Scripts" / "python.exe";
  const auto requirements = pluginPath / "requirements.txt";

  std::stringstream command;
  command << pythonExe.string() << " -m pip install -r " << requirements.string();

  return execute(command.str().c_str());
}

bool venvDlgImpl::createRequirements(const std::filesystem::path& pluginPath)
{
  const auto pythonExe = pluginPath / "venv" / "Scripts" / "python.exe";
  const auto requirements = pluginPath / "requirements.txt";

  std::stringstream command;
  command << pythonExe.string() << " -m pip freeze > " << requirements.string();

  return execute(command.str().c_str());
}

bool venvDlgImpl::installPackage(const std::filesystem::path& pluginPath, const std::wstring& package)
{
  const auto pythonExe = pluginPath / "venv" / "Scripts" / "python.exe";

  std::stringstream command;
  command << pythonExe.string() << " -m pip install " << package;

  return execute(command.str().c_str());
}

bool venvDlgImpl::validPluginZip(const std::filesystem::path& zipPath, bool& hasRequirements)
{
  bool validZip = false;

  zip_t* zip = zip_open(zipPath.string().c_str(), 0, 'r');

  if (!zip)
    return false;

  int result = zip_entry_open(zip, "python_plugin.manifest");

  if (result == 0)
  {
    validZip = true;
    zip_entry_close(zip);
  }

  result = zip_entry_open(zip, "requirements.txt");

  if (result == 0)
  {
    hasRequirements = true;
    zip_entry_close(zip);
  }

  zip_close(zip);

  return validZip;
}

bool venvDlgImpl::installPlugin(const std::filesystem::path& zipPath)
{
  auto executableDir = stdutils::findExecutableFileName().parent_path();

  const std::string pluginName = zipPath.stem().string();
  std::filesystem::path pluginPath(executableDir / "plugins" / pluginName);

  std::error_code ec;
  std::filesystem::create_directories(pluginPath, ec);

  if (ec)
    return false;

  int result = zip_extract(zipPath.string().c_str(), pluginPath.string().c_str(), nullptr, nullptr);

  return (result == 0);
}

bool venvDlgImpl::createPluginZip(const std::filesystem::path& sourceDir, const std::filesystem::path& zipPath)
{
  if (!std::filesystem::exists(sourceDir) || !std::filesystem::is_directory(sourceDir))
    return false;

  zip_t* zip = zip_open(zipPath.string().c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');

  if (!zip)
    return false;

  const std::filesystem::path absoluteZipPath = std::filesystem::absolute(zipPath).lexically_normal();

  bool success = true;

  std::filesystem::recursive_directory_iterator it(sourceDir, std::filesystem::directory_options::skip_permission_denied);

  const std::filesystem::recursive_directory_iterator end;

  for (; it != end; ++it)
  {
    const std::filesystem::path currentPath = it->path();
    const std::string directoryName = currentPath.filename().string();

    if (!directoryName.empty() && directoryName.front() == '.')
    {
      if (it->is_directory())
        it.disable_recursion_pending();

      continue;
    }

    if (it->is_directory())
    {
      if (directoryName == "venv" ||
          directoryName == "__pycache__")
      {
        it.disable_recursion_pending();
        continue;
      }

      continue;
    }

    if (!it->is_regular_file())
      continue;

    if (std::filesystem::absolute(currentPath).lexically_normal() == absoluteZipPath)
      continue;

    const std::filesystem::path relativePath = std::filesystem::relative(currentPath, sourceDir);

    const std::string entryName = relativePath.generic_string();

    if (zip_entry_open(zip, entryName.c_str()) != 0)
    {
      success = false;
      break;
    }

    if (zip_entry_fwrite(zip, currentPath.string().c_str()) != 0)
    {
      zip_entry_close(zip);
      success = false;
      break;
    }

    if (zip_entry_close(zip) != 0)
    {
      success = false;
      break;
    }
  }

  zip_close(zip);

  return success;
}
