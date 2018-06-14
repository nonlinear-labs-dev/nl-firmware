#pragma once

#ifdef _DEVELOPMENT_PC

#include <gtkmm-3.0/gtkmm.h>
#include <io/network/WebSocketServer.h>
#include "Boled.h"
#include "PanelUnit.h"
#include "PlayPanel.h"
#include "Ribbon.h"
#include "LowerRibbon.h"

class Window : public Gtk::Window
{
  public:
    Window();
    virtual ~Window();

  private:
    void onFrameBufferMessageReceived(WebSocketServer::tMessage msg);
    void onPanelLEDsMessageReceived(WebSocketServer::tMessage msg);
    WebSocketServer::tMessage m_frameBuffer;

    Gtk::VBox m_box;
#ifdef _WINDOW_CONTROLS
    Gtk::VBox m_ribbonBox;
    PlayPanel m_playPanel;
    PanelUnit m_editPanel;
    Ribbon m_ribbonUp;
    LowerRibbon m_ribbonDown;
#else
    Boled m_boled;
#endif
};

#endif
