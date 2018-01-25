#include <io/network/WebSocketReceiver.h>
#include <io/network/WebSocketServer.h>
#include <Application.h>

WebSocketReceiver::WebSocketReceiver(Domain d)
{
  Application::get().getWebsocketServer()->onMessageReceived(d, sigc::mem_fun(this, &WebSocketReceiver::onMessageReceived));
}

WebSocketReceiver::~WebSocketReceiver()
{
}

void WebSocketReceiver::onMessageReceived(tMessage msg)
{
  onDataReceived(msg);
}
