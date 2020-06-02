#include <io/ToLPCBridge.h>
#include <Application.h>
#include <BBBBOptions.h>
#include "files/FileIOSender.h"
#include <nltools/messaging/Message.h>

ToLPCBridge::ToLPCBridge()
    : super(new FileIOSender(Application::get().getOptions()->getToLPCPath().c_str()), nullptr)
{
  nltools::msg::receive<nltools::msg::LPCMessage>(nltools::msg::EndPoint::Lpc,
                                                  sigc::mem_fun(this, &ToLPCBridge::onMessage));
}

void ToLPCBridge::onMessage(const nltools::msg::LPCMessage &msg)
{
  gsize numBytes = 0;
  auto data = reinterpret_cast<const char *>(msg.message->get_data(numBytes));
  static_cast<FileIOSender *>(m_sender.get())->write(data, numBytes);
}
