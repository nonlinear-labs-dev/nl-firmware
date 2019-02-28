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
      auto notes = { 48, 51, 55, 58, 60, 58, 55, 51 };

      for(auto n : notes)
      {
        snd_seq_event_t event;
        event.type = SND_SEQ_EVENT_NOTEON;
        event.data.note.channel = 0x90;
        event.data.note.note = n + o * 12;
        event.data.note.velocity = 100;

        getCallback()(event);

        std::this_thread::sleep_for(std::chrono::milliseconds(distance));

        if(!m_run)
          return;
      }
    }
  }
}
