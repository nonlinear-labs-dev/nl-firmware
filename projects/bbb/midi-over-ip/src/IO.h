#pragma once

#include <algorithm>
#include <future>
#include <alsa/asoundlib.h>

namespace nltools
{
  namespace msg
  {
    namespace Midi
    {
      class SimpleMessage;
    }
  }
}

class IO
{
 protected:
  IO(snd_rawmidi_t *handle);
  IO(IO &&i);
  ~IO();

  snd_rawmidi_t *handle = nullptr;
};

class Input : public IO
{
 public:
  Input(Input &&i);
  Input(const std::string &name);
  ~Input();

 private:
  static snd_rawmidi_t *open(const std::string &name);
  void readMidi();

  std::future<void> m_bg;
  bool m_quit = false;
  int m_cancelPipe[2];
};

class Output : public IO
{
 public:
  Output(Output &&i);
  Output(const std::string &name);
  void send(const nltools::msg::Midi::SimpleMessage &msg);

 private:
  static snd_rawmidi_t *open(const std::string &name);
};
