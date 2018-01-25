#pragma once

#include <gtkmm-3.0/gtkmm.h>
#include <io/network/WebSocketServer.h>

class Window : public Gtk::Window
{
  public:
    Window();
    virtual ~Window();

    bool on_draw(const ::Cairo::RefPtr<::Cairo::Context> & cr) override;

  private:
    void onFrameBufferMessageReceived(WebSocketServer::tMessage msg);
    void onPanelLEDsMessageReceived(WebSocketServer::tMessage msg);

    WebSocketServer::tMessage m_frameBuffer;

    std::array<uint8_t, 96> m_panelLEDs;
};

