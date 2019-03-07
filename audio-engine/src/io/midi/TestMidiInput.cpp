#include "TestMidiInput.h"

TestMidiInput::TestMidiInput(int distance, MidiInput::Callback cb)
    : MidiInput(cb)
{
  m_run = true;
  m_bgThread = std::thread([=]() { doBackgroundWork(distance); });
}

TestMidiInput::~TestMidiInput()
{
  m_run = false;

  if(m_bgThread.joinable())
    m_bgThread.join();
}

void TestMidiInput::doBackgroundWork(int distance)
{
  while(true)
  {
    for(int o = 0; o < 4; o++)
    {
      auto notes = { 48, 51, 55, 58, 60, 63, 67, -48, -51, -55, -58, -60, -63, -67, 0, 0 };

      for(auto n : notes)
      {
        uint8_t noteOnOff = (n > 0) ? 0x90 : 0x00;
        uint8_t pitch = static_cast<uint8_t>(std::abs(n));
        uint8_t velocity = 100;
        std::chrono::high_resolution_clock::time_point timestamp = {};

        MidiEvent event = { { noteOnOff, pitch, velocity }, timestamp };

        if(n != 0)
          getCallback()(event);

        std::this_thread::sleep_for(std::chrono::milliseconds(distance));

        if(!m_run)
          return;
      }
    }
  }
}
