#include "VenvAction.hpp"
#include "venvDlgImpl.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif

VenvAction::VenvAction()
  : sdm::plugin::ActionFeatureHelper("Create venv...", "Python")
{
}

void VenvAction::execute() const
{
  wxInitialize();

  auto pDlg = new venvDlgImpl(nullptr/*m_pContainerWindow*/, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxSTAY_ON_TOP);

  pDlg->Show();
}
