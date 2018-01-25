#include <Application.h>
#include <bbbb.h>
#include <ui/Window.h>

class WebSocketReceiver;

constexpr auto framebufferDimX = 256;
constexpr auto framebufferDimY = 96;

Window::Window()
{
  std::fill(m_panelLEDs.begin(), m_panelLEDs.end(), 0);

  set_default_size(framebufferDimX, framebufferDimY * 2);
  Application::get().getWebsocketServer()->onMessageReceived(Domain::Oled, sigc::mem_fun(this, &Window::onFrameBufferMessageReceived));
  Application::get().getWebsocketServer()->onMessageReceived(Domain::PanelLed, sigc::mem_fun(this, &Window::onPanelLEDsMessageReceived));
}

Window::~Window()
{
}

void Window::onFrameBufferMessageReceived(WebSocketServer::tMessage msg)
{
  m_frameBuffer = msg;
  queue_draw();
}

void Window::onPanelLEDsMessageReceived(WebSocketServer::tMessage msg)
{
  gsize len = 0;
  const int8_t *data = reinterpret_cast<const int8_t *>(msg->get_data(len));
  auto idx = data[0] & 0x7F;
  auto val = data[0] >> 7;
  m_panelLEDs[idx] = val;
  queue_draw();
}

bool Window::on_draw(const ::Cairo::RefPtr<::Cairo::Context> & cr)
{
  static std::map<uint8_t, std::tuple<uint8_t, uint8_t, uint8_t>> colorMap =
  {
   { 0x00, std::make_tuple(43, 32, 21)},
   { 0x02, std::make_tuple(77, 60, 10)},
   { 0x05, std::make_tuple(103, 81, 12)},
   { 0x06, std::make_tuple(128, 102, 16)},
   { 0x0A, std::make_tuple(179, 142, 21)},
   { 0x0B, std::make_tuple(204, 162, 24)},
   { 0x0F, std::make_tuple(255, 203, 31)}
  };

  if(m_frameBuffer)
  {
    gsize len = 0;
    const int8_t *data = reinterpret_cast<const int8_t *>(m_frameBuffer->get_data(len));

    for(int y = 0; y < framebufferDimY; y++)
    {
      for(int x = 0; x < framebufferDimX; x++)
      {
        int idx = y * framebufferDimX + x;

        auto &rgb = colorMap.at(data[idx]);
        cr->rectangle (x, y, 1, 1);
        cr->set_source_rgb (std::get<0>(rgb) / 256.0, std::get<1>(rgb) / 256.0, std::get<2>(rgb) / 256.0);
        cr->fill ();
      }
    }
  }

  const auto ledDim = framebufferDimX / 24;

  for(int y = 0; y < 4; y++)
  {
    for(int x = 0; x < 24; x++)
    {
      int idx = x * 4 + y;
      auto radius = ledDim / 2;
      cr->arc (radius + x * ledDim, radius + ledDim * y + framebufferDimY, radius, 0, 2 * M_PI);
      cr->set_source_rgb (m_panelLEDs[idx] ? 1.0 : 0.0, 0, 0);
      cr->fill_preserve ();
      cr->set_line_width(0.5);
      cr->set_source_rgb (0.2, 0.2, 0.2);
      cr->stroke ();
    }
  }

  return true;
}
