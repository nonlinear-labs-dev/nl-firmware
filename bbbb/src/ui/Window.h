#pragma once

#ifdef _DEVELOPMENT_PC

#include <gtkmm-3.0/gtkmm.h>
#include <io/network/WebSocketServer.h>
#include "PanelUnit.h"
#include "PlayPanel.h"
#include "Ribbon.h"
#include "LowerRibbon.h"

namespace nltools
{
  namespace msg
  {
    struct SetPanelLEDMessage;
    struct SetOLEDMessage;
  }
}

class Window : public Gtk::Window
{
 public:
  Window();
  virtual ~Window();

 private:
  void onFrameBufferMessageReceived(const nltools::msg::SetOLEDMessage &msg);
  void onPanelLEDsMessageReceived(const nltools::msg::SetPanelLEDMessage &msg);
  WebSocketServer::tMessage m_frameBuffer;

  Gtk::VBox m_box;
  Gtk::VBox m_ribbonBox;
  PlayPanel m_playPanel;
  PanelUnit m_editPanel;
  Ribbon m_ribbonUp;
  LowerRibbon m_ribbonDown;
};

#endif
