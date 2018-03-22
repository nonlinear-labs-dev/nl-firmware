#include <Application.h>
#include <device-settings/DebugLevel.h>
#include <device-settings/EncoderAcceleration.h>
#include <device-settings/Settings.h>
#include <proxies/hwui/panel-unit/RotaryEncoder.h>
#include <testing/TestDriver.h>
#include <proxies/lpc/LPCProxy.h>

static const char* s_rotaryDevFile = "/dev/espi_encoder";

static TestDriver<RotaryEncoder> tester;

RotaryEncoder::RotaryEncoder() :
    m_readersCancel(Gio::Cancellable::create()),
    m_throttler(chrono::milliseconds(25))
{
  open();
}

RotaryEncoder::~RotaryEncoder()
{
  m_stress.disconnect();
}

void RotaryEncoder::open()
{
  RefPtr < Gio::File > rotaryFile = Gio::File::create_for_path(s_rotaryDevFile);
  rotaryFile->read_async(sigc::bind(sigc::mem_fun(this, &RotaryEncoder::onRotaryFileOpened), rotaryFile), m_readersCancel);
}

void RotaryEncoder::onRotaryFileOpened(Glib::RefPtr<Gio::AsyncResult>& result, RefPtr<Gio::File> rotaryFile)
{
  try
  {
    DebugLevel::gassy("RotaryEncoder::open file");
    Glib::RefPtr < Gio::FileInputStream > stream = rotaryFile->read_finish(result);
    readRotary (stream);
  }
  catch(Gio::Error &error)
  {
    DebugLevel::warning("Could not read from rotary input stream");
  }
}

void RotaryEncoder::readRotary(Glib::RefPtr<Gio::FileInputStream> stream)
{
  DebugLevel::gassy("RotaryEncoder::readRotary");
  stream->read_bytes_async(1, sigc::bind(sigc::mem_fun(this, &RotaryEncoder::onRotaryFileRead), stream), m_readersCancel);
}

void RotaryEncoder::onRotaryFileRead(Glib::RefPtr<Gio::AsyncResult>& result, Glib::RefPtr<Gio::FileInputStream> stream)
{
  Glib::RefPtr < Glib::Bytes > bytes = stream->read_bytes_finish(result);

  gsize numBytes = 0;
  const tIncrement* buffer = (const tIncrement*) bytes->get_data(numBytes);

  if(numBytes > 0)
  {
    tIncrement currentInc = buffer[0];
    m_signalRotaryChanged.send(currentInc);

    if((currentInc < 0) != (m_accumulatedIncs < 0))
      m_accumulatedIncs = 0;

    m_accumulatedIncs += currentInc;

    m_throttler.doTask([this]()
    {
      if (abs(m_accumulatedIncs) > 0)
      {
        m_accumulatedIncs = std::min(m_accumulatedIncs, 10);
        m_accumulatedIncs = std::max(m_accumulatedIncs, -10);
        auto factor = Application::get().getSettings()->getSetting<EncoderAcceleration>()->get();
        int sign = m_accumulatedIncs < 0 ? -1 : 1;
        m_signalRotaryChanged.send (factor * sign * m_accumulatedIncs * m_accumulatedIncs);
      }
      m_accumulatedIncs = 0;
    });

    readRotary(stream);
  }
  else
  {
    open();
  }
}

void RotaryEncoder::fake(tIncrement i)
{
  m_signalRotaryChanged.send(i);
}

sigc::connection RotaryEncoder::onRotaryChanged(function<void(tIncrement)> slot)
{
  return m_signalRotaryChanged.connect(slot);
}

void RotaryEncoder::registerTests()
{
}
