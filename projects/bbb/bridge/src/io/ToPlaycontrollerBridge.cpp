#include <io/ToPlaycontrollerBridge.h>
#include <Application.h>
#include <BBBBOptions.h>
#include "files/FileIOSender.h"
#include <nltools/messaging/Message.h>

ToPlaycontrollerBridge::ToPlaycontrollerBridge()
    : super(new FileIOSender(Application::get().getOptions()->getToPlaycontrollerPath().c_str()), nullptr)
{
  nltools::msg::receive<nltools::msg::PlaycontrollerMessage>(nltools::msg::EndPoint::Playcontroller,
                                                             sigc::mem_fun(this, &ToPlaycontrollerBridge::onMessage));
}

void ToPlaycontrollerBridge::onMessage(const nltools::msg::PlaycontrollerMessage &msg)
{
  gsize numBytes = 0;
  auto data = reinterpret_cast<const char *>(msg.message->get_data(numBytes));
  static_cast<FileIOSender *>(m_sender.get())->write(data, numBytes);
}
